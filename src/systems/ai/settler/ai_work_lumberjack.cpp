#include "ai_work_lumberjack.hpp"
#include "../../../components/ai_tags/ai_tag_work_lumberjack.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../distance_map_system.hpp"
#include "../../../messages/inventory_changed_message.hpp"
#include "../../../components/item.hpp"
#include "../path_finding.hpp"
#include "../../../components/game_stats.hpp"
#include "../../../messages/map_dirty_message.hpp"
#include "../../../messages/inflict_damage_message.hpp"
#include "ai_work_template.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../main/game_rng.hpp"
#include "job_board.hpp"
#include "../../../utils/telemetry.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/defs/item_def_t.hpp"

using namespace region;

namespace jobs_board {
    void evaluate_lumberjacking(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->chopping.empty()) return; // Nothing to cut down

        auto axe_distance = axe_map.get(mapidx(pos));
        if (axe_distance > MAX_DIJSTRA_DISTANCE-1) return; // No axe available

        // Evaluate the closest tree to chop
        std::size_t i = 0;
        float distance = std::numeric_limits<float>().max();
        std::size_t selected = 0;
        for (const auto &chop : designations->chopping) {
            const float d = distance3d(pos.x, pos.y, pos.z, chop.second.x, chop.second.y, chop.second.z);
            if (d < distance) {
                distance = d;
                selected = i;
            }
            ++i;
        }

        board.insert(std::make_pair(distance + axe_distance, jt));
    }
}

void ai_work_lumberjack::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_lumberjack>(jobs_board::evaluate_lumberjacking);
}

void ai_work_lumberjack::update(const double duration_ms)
{
    ai_work_template<ai_tag_work_lumberjack> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_lumberjack &lj, ai_tag_my_turn_t &t, position_t &pos) {
        work.set_status(e, "Lumberjacking");
        if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GET_AXE) {
            work.folllow_path(axe_map, pos, e, [&e]() {
                // On cancel
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }, [&e, this, &pos, &lj, &work] {
                // On success
                work.pickup_tool<axemap_changed_message>(e, pos, TOOL_CHOPPING, lj.current_axe, [&e, &lj]() {
                    // On cancel
                    delete_component<ai_tag_work_lumberjack>(e.id);
                    return;
                }, [&lj]() {
                    // On success
                    lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                });
            });
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE) {
            // Check that we're still a go
            if (designations->chopping.empty()) {
                // There is no tree - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                delete_component<ai_tag_work_lumberjack>(e.id);
                return;
            }

            // Find a tree position
            std::set<int> claimed_trees;
            each<ai_tag_work_lumberjack>([&claimed_trees, &e] (entity_t &E, ai_tag_work_lumberjack &LJ) {
                if (e.id != E.id && LJ.target_tree > 0) claimed_trees.insert(E.id);
            });

            std::size_t tree_id = 0;
            float distance = std::numeric_limits<float>::max();
            for (auto it= designations->chopping.begin(); it!=designations->chopping.end(); ++it) {
                auto dupe_finder = claimed_trees.find(it->first);
                if (dupe_finder == claimed_trees.end()) {
                    const float d = distance3d(pos.x, pos.y, pos.z, it->second.x, it->second.y, it->second.z);
                    if (d < distance) {
                        tree_id = it->first;
                        distance = d;
                    }
                }
            }

            if (tree_id == 0) {
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                work.cancel_work_tag(e);
                return;
            }

            auto f = designations->chopping.find(tree_id);

            position_t tree_pos = f->second;
            lj.target_tree = f->first;
            lj.target_x = tree_pos.x;
            lj.target_y = tree_pos.y;
            lj.target_z = tree_pos.z;

            std::array<position_t, 4> target;
            target[0] = position_t{ tree_pos.x, tree_pos.y-1, tree_pos.z };
            target[1] = position_t{ tree_pos.x, tree_pos.y+1, tree_pos.z };
            target[2] = position_t{ tree_pos.x-1, tree_pos.y, tree_pos.z };
            target[3] = position_t{ tree_pos.x+1, tree_pos.y, tree_pos.z };

            int n = 0;
            while (!lj.current_path && n<4) {
                lj.current_path = find_path(pos, target[n]);
                if (!lj.current_path->success) {
                    lj.current_path.reset();
                }
                ++n;
            }

            // Are we good to go?
            if (!lj.current_path) {
                // There is no path - cancel
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                work.cancel_work_tag(e);
                return;
            } else {
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE;
                return;
            }
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::GOTO_TREE) {
            // Check that it is still a valid tree
            auto tree_finder = designations->chopping.find(lj.target_tree);
            if (tree_finder == designations->chopping.end()) {
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                return;
            }

            // Go there
            work.follow_path(lj, pos, e, [&lj] () {
                // Cancel
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                return;
            }, [&lj] () {
                // We've arrived
                lj.current_path.reset();
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::CHOP;
                return;
            });
            return;
        }else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::CHOP) {
            //std::cout << "Chop\n";
            // Check that it is still a valid tree
            auto tree_finder = designations->chopping.find(lj.target_tree);
            if (tree_finder == designations->chopping.end()) {
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::FIND_TREE;
                return;
            }

            auto stats = e.component<game_stats_t>();
            if (!stats) {
                emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
                emit(axemap_changed_message{});
                work.cancel_work_tag(e);
                return;
            }

            auto skill_check = skill_roll(e.id, *stats, rng, "Lumberjacking", DIFICULTY_TOUGH);

            if (skill_check >= SUCCESS) {
                call_home("tree_chop");

                // Tree is going down!
                int number_of_logs = 0;
                int tree_idx = 0;
                int lowest_z = 1000;
                for (int z=0; z<REGION_DEPTH; ++z) {
                    for (int y=0; y<REGION_HEIGHT; ++y) {
                        for (int x=0; x<REGION_WIDTH; ++x) {
                            const auto idx = mapidx(x,y,z);
                            if (tree_id(idx) == lj.target_tree) {
                                if (z < lowest_z) {
                                    lowest_z = z;
                                    tree_idx = idx;
                                }

                                make_open_space(idx);
                                tile_calculate(x,y,z);
                                ++number_of_logs;
                            }
                        }
                    }
                }
                make_floor(tree_idx);
                tile_calculate(lj.target_x, lj.target_y, lj.target_z);
                int tx,ty,tz;
                std::tie(tx,ty,tz) = idxmap(tree_idx);

                // Spawn wooden logs
                number_of_logs = (number_of_logs/20)+1;
                for (int i=0; i<number_of_logs; ++i) {
                    spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"));
                }

                // Update pathing
                for (int Z=-2; Z<10; ++Z) {
                    for (int Y=-10; Y<10; ++Y) {
                        for (int X=-10; X<10; ++X) {
                            tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                            tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
                        }
                    }
                }

                // Remove the tree from the designations list
                designations->chopping.erase(lj.target_tree);

                // Change status to drop axe or continue
                lj.step = ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS;

            } else if (skill_check == CRITICAL_FAIL) {
                // Damage yourself
                emit_deferred(inflict_damage_message{e.id, 1, "Lumberjacking Accident"});
            }
            return;
        } else if (lj.step == ai_tag_work_lumberjack::lumberjack_steps::DROP_TOOLS) {
            emit(drop_item_message{lj.current_axe, pos.x, pos.y, pos.z});
            emit(axemap_changed_message{});
            work.cancel_work_tag(e);
            return;
        }
    });
}
