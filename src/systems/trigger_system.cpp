#include "trigger_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../components/entry_trigger.hpp"
#include "../components/position.hpp"
#include "../components/grazer_ai.hpp"
#include "../components/sentient_ai.hpp"
#include "../components/settler_ai.hpp"
#include "../messages/log_message.hpp"
#include "../components/name.hpp"
#include "../components/building.hpp"
#include "../components/item_stored.hpp"
#include "../raws/raws.hpp"
#include "../components/logger.hpp"
#include "../raws/materials.hpp"
#include "../messages/inflict_damage_message.hpp"
#include "../messages/inventory_changed_message.hpp"
#include "../main/game_globals.hpp"

void trigger_system::configure() {
    system_name = "Trigger System";
    subscribe_mbox<triggers_changes_message>();
    subscribe_mbox<entity_moved_message>();
    subscribe_mbox<request_lever_pull_message>();
    subscribe_mbox<trigger_details_requested>();
}

void trigger_system::update(const double duration_ms) {
    each_mbox<triggers_changes_message>([this] (const triggers_changes_message &msg) {
        std::cout << "Received trigger notification\n";
        this->dirty = true;
    });

    if (dirty) {
        std::cout << "Rebuilding trigger list\n";
        triggers.clear();
        each<entry_trigger_t, position_t>([this] (entity_t &e, entry_trigger_t &trigger, position_t &pos) {
            std::cout << "Found a trigger!\n";
            triggers.insert(std::make_pair(mapidx(pos), e.id));
        });
        dirty = false;
    }

    each_mbox<request_lever_pull_message>([this] (const request_lever_pull_message &msg) {
        // Add to the to-do list!
    });

    each_mbox<trigger_details_requested>([this] (const trigger_details_requested &msg) {
        trigger_id = msg.lever_id;
        pause_mode = PAUSED;
    });

    each_mbox<entity_moved_message>([this] (const entity_moved_message &msg) {
        //std::cout << "Received an entity move message. There are " << triggers.size() << " triggers.\n";
        const int tile_index = mapidx(msg.destination);
        auto finder = triggers.find(tile_index);
        if (finder != triggers.end()) {
            //std::cout << "Found a trigger\n";
            auto trigger_entity = entity(finder->second);
            if (trigger_entity) {
                auto trigger_def = trigger_entity->component<entry_trigger_t>();
                if (trigger_def) {
                    if (trigger_def->active) {
                        //std::cout << "Trigger is active\n";
                        // Does the trigger apply to the entity type
                        auto target_entity = entity(msg.entity_id);
                        if (target_entity) {
                            auto grazer = target_entity->component<grazer_ai>();
                            auto sentient = target_entity->component<sentient_ai>();
                            auto settler = target_entity->component<settler_ai_t>();

                            //if (grazer) std::cout << "Target grazes\n";
                            //if (sentient) std::cout << "Target is sentient\n";
                            //if (settler) std::cout << "Target a settler - probably ignored\n";

                            // Cages only affect hostiles and beasts
                            if (trigger_def->type == trigger_cage && (grazer || (sentient && sentient->hostile) )) {
                                //std::cout << "Cage triggered\n";
                                auto name = target_entity->component<name_t>();
                                if (name) {
                                    LOG ss;
                                    ss.other_name(msg.entity_id)->text(" is trapped in a cage!");
                                    emit_deferred(log_message{ss.chars});
                                }

                                // TODO: Add a random chance with some dex involved
                                // Spawn a cage object
                                auto building = trigger_entity->component<building_t>();
                                std::size_t material = get_material_by_tag(building->built_with[0].first).get();
                                int x,y,z;
                                std::tie(x,y,z) = idxmap(tile_index);
                                auto new_cage = spawn_item_on_ground_ret(x, y, z, "cage", material);

                                // Add a stored component
                                target_entity->assign<item_stored_t>(new_cage->id);

                                // Remove the position component
                                delete_component<position_t>(msg.entity_id);

                                // Remove the trap
                                delete_entity(finder->second);
                                emit(triggers_changes_message{});
                            } else if (trigger_def->type == trigger_stonefall && (grazer || (sentient && sentient->hostile) )) {
                                //std::cout << "Stonefall triggered\n";
                                // Stonefalls only affect hostiles
                                auto name = target_entity->component<name_t>();
                                if (name) {
                                    LOG ss;
                                    ss.other_name(msg.entity_id)->text(" is hit by a falling rock trap!");
                                    emit_deferred(log_message{ss.chars});
                                }

                                // TODO: Add a random chance with some dex involved
                                // Spawn some damage!
                                emit(inflict_damage_message{msg.entity_id, rng.roll_dice(3,6), "falling rocks"});

                                // Remove the trap
                                delete_entity(finder->second);
                                emit(triggers_changes_message{});
                            } else if (trigger_def->type == trigger_blade && (grazer || (sentient && sentient->hostile) )) {
                                //std::cout << "Blade trap triggered\n";
                                // Blades only affect hostiles, and don't auto-destruct
                                auto name = target_entity->component<name_t>();
                                if (name) {
                                    LOG ss;
                                    ss.other_name(msg.entity_id)->text(" is hit by a blade trap!");
                                    emit_deferred(log_message{ss.chars});
                                }

                                // TODO: Add a random chance with some dex involved
                                // Spawn some damage!
                                emit(inflict_damage_message{msg.entity_id, rng.roll_dice(3,8), "spinning blades"});
                            }
                        }
                    }
                }
            }
        }
    });
}