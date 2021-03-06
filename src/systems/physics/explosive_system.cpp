#include "explosive_system.hpp"
#include "../../main/game_pause.hpp"
#include "../../components/position.hpp"
#include "../../components/explosion_t.hpp"
#include "../ai/movement_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../messages/emit_particles_message.hpp"
#include "../gui/particle_system.hpp"
#include "../../main/game_rng.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/inflict_damage_message.hpp"
#include "../ai/visibility_system.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/raws.hpp"
#include "../../messages/perform_mining.hpp"
#include "../../main/game_designations.hpp"
#include "../../messages/tick_message.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../components/renderable.hpp"
#include "../../messages/renderables_changed_message.hpp"
#include "../../messages/vegetation_damage_message.hpp"
#include <unordered_set>
#include <tuple>

using namespace region;

void explosive_system::configure() {
    system_name = "Explosive System";
    subscribe<tick_message>([this] (tick_message &msg) {
        this->tick = true;
    });
}

inline void internal_boom_to(position_t &pos, int radius, int x, int y, int z, std::unordered_set<int> &targets) {
    const float dist_square = radius * radius;

    int last_z = pos.z;
    line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x+x, pos.y+y, pos.z+z, [&targets, &pos, &dist_square, &last_z] (int X, int Y, int Z) {
        if (X < 1 || X > REGION_WIDTH-1 || Y < 1 || Y > REGION_HEIGHT-1 || Z < 1 || Z > REGION_DEPTH-1)
        {
            return false;
        } else {
            const auto idx = mapidx(X, Y, Z);
            bool blocked = solid(idx);
            if (blocked_visibility.find(idx) != blocked_visibility.end()) blocked = true;
            if (!blocked && last_z != Z) {
                //std::cout << "Last Z: " << last_z << ", Z: " << Z << "\n";
                // Check for ceilings and floors
                if (last_z > Z) {
                    if (region::tile_type(idx) == tile_type::FLOOR) {
                        blocked = true;
                        //std::cout << "Ceiling block\n";
                    }
                } else if (last_z < Z) {
                    if (region::tile_type(mapidx(X, Y, last_z)) == tile_type::FLOOR) {
                        blocked = true;
                        //std::cout << "Floor block\n";
                    }
                }
            }
            emit(emit_particles_message{PARTICLE_BOOM, X, Y, Z});
            if (targets.find(idx) == targets.end()) {
                targets.insert(idx);
            }
            const float distance = distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
            if (distance > dist_square) {
                return false;
            }
            last_z = Z;
            return !blocked;
        }
    });
}

void explosive_system::update(const double duration_ms) {
    if (pause_mode == PAUSED || !tick) return;

    each<explosion_t, position_t>([] (entity_t &e, explosion_t &boom, position_t &pos) {
        if (boom.fuse_timer > 0) {
            --boom.fuse_timer;
            auto renderable = e.component<renderable_t>();
            emit(emit_particles_message{PARTICLE_BOOM_FUSE, pos.x, pos.y, pos.z});
            return;
        }

        // New explosions get a radius equal to their size
        if (boom.blast_timer == 254) boom.blast_timer = 1;

        // If the explosion has expired, delete it
        if (boom.blast_timer > boom.blast_radius) {
            entity_octree.remove_node(octree_location_t{pos.x, pos.y, pos.z, e.id});
            delete_entity(e.id);
            return;
        }

        if (boom.blast_timer < boom.blast_radius+1) {
            //std::cout << "Boom - radius " << +boom.blast_timer << "\n";

            // Do a Line-of-Sight traversal over the boom radius
            std::unordered_set<int> exploding_tiles;
            exploding_tiles.insert(mapidx(pos)); // Always hit the origin
            for (int z = (0 - boom.blast_timer); z < boom.blast_timer; ++z) {
                for (double angle=0.0; angle<360.0; angle += 1.0) {
                    auto dest = project_angle(0, 0, boom.blast_timer, angle*0.0174533);
                    internal_boom_to(pos, boom.blast_radius, dest.first, dest.second, z, exploding_tiles);
                }
            }

            // Affect each blasted tile
            for (auto boomidx : exploding_tiles) {
                if (boom.tiles_hit.find(boomidx) == boom.tiles_hit.end()) {
                    boom.tiles_hit.insert(boomidx);

                    int x, y, z;
                    std::tie(x, y, z) = idxmap(boomidx);

                    // Create a smoke effect
                    emit(emit_particles_message{PARTICLE_SMOKE, x, y, z});

                    // Damage to the tile - change it to use the material properties
                    if (solid(boomidx) || region::tile_type(boomidx) == tile_type::FLOOR) {
                        // We need to damage the tile
                        const int damage_base = rng.roll_dice(boom.damage_dice, boom.damage_dice_type);
                        const int damage = damage_base / 10;
                        emit(vegetation_damage_message{boomidx, damage});
                        if (damage > tile_hit_points(boomidx)) {
                            damage_tile(boomidx, damage);
                            // Destroyed
                            if (tree_id(boomidx) > 0) {
                                // Destroy the tree and make wood
                                const auto tid = tree_id(boomidx);
                                for (int Z = 0; Z < REGION_DEPTH; ++Z) {
                                    for (int Y = 0; Y < REGION_HEIGHT; ++Y) {
                                        for (int X = 0; X < REGION_WIDTH; ++X) {
                                            const int idx = mapidx(X, Y, Z);
                                            if (tree_id(idx) == tid) {
                                                make_open_space(idx);
                                                if (rng.roll_dice(1, 4) > 2) spawn_item_on_ground(X, Y, Z, "wood_log",
                                                                                                  get_material_by_tag(
                                                                                                          "wood"));
                                            }
                                        }
                                    }
                                }
                                delete_tree(tid);
                                designations->chopping.erase(tid);
                                for (int Z = -2; Z < 10; ++Z) {
                                    for (int Y = -10; Y < 10; ++Y) {
                                        for (int X = -10; X < 10; ++X) {
                                            if (x + X > 0 && x + X < REGION_WIDTH && y + Y > 0 &&
                                                y + Y < REGION_HEIGHT && z + Z > 0 && z + Z < REGION_DEPTH) {
                                                tile_calculate(x + X, y + Y, z + Z);
                                                tile_calculate(x + X, y + Y, z + Z);
                                            }
                                        }
                                    }
                                }
                            } else {
                                // Mine out the tile
                                emit(perform_mining_message{boomidx, 1, x, y, z});
                                if (region::tile_type(boomidx) == tile_type::FLOOR) {
                                    make_open_space(boomidx);
                                    tile_calculate(x, y, z);
                                }
                            }
                        } else {
                            damage_tile(boomidx, damage);
                        }
                    }

                    // Damage everyone/everything present
                    const auto targets = entity_octree.find_by_loc(octree_location_t{x, y, z});

                    float vx = 0.0f, vy = 0.0f, vz = 0.0f;

                    vx = pos.x - x;
                    vy = pos.y - y;
                    vz = pos.z - z;
                    const float distance = distance3d(x, y, z, (int) vx, (int) vy, (int) vz);
                    if (distance > 0) {
                        vx /= distance;
                        vy /= distance;
                        vz /= distance;
                    }
                    position_t dest{x + vx, y + vy, z + vz};
                    if (dest.x < 1) dest.x = 1;
                    if (dest.x > REGION_WIDTH - 1) dest.x = REGION_WIDTH - 1;
                    if (dest.y < 1) dest.y = 1;
                    if (dest.y > REGION_HEIGHT - 1) dest.y = REGION_HEIGHT - 1;
                    if (dest.z < 1) dest.z = 1;
                    if (dest.z > REGION_DEPTH - 1) dest.z = REGION_DEPTH - 1;
                    const bool dest_solid = solid(mapidx(dest));

                    for (const auto &target : targets) {
                        emit(
                                inflict_damage_message{target,
                                                       rng.roll_dice(boom.damage_dice, boom.damage_dice_type),
                                                       "explosion"});
                        //if (!dest_solid && distance > 0.9f && ) {
                        //    emit_deferred(entity_wants_to_move_message{target, dest});
                        //}
                    }
                }
            }
        }

        ++boom.blast_timer; // Decrease the boom effect
    });
    tick = false;
}
