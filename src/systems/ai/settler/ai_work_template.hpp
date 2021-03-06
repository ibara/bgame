#pragma once

#include <rltk.hpp>
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/position.hpp"
#include "../../../components/item.hpp"
#include "../../../components/item_stored.hpp"
#include "../../../utils/dijkstra_map.hpp"
#include "../../../messages/entity_moved_message.hpp"
#include "../../../messages/inventory_changed_message.hpp"
#include "../../../messages/renderables_changed_message.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../main/game_pause.hpp"
#include "../../../planet/region/region.hpp"

template<typename TAG>
class ai_work_template {
public:
    ai_work_template() { }

    template <typename F>
    void do_ai(const F &&f)
    {
        if (pause_mode == PAUSED) return;

        each<TAG, ai_tag_my_turn_t, position_t>([&f] (entity_t &e, TAG &tag, ai_tag_my_turn_t &turn, position_t &pos) {
            delete_component<ai_tag_my_turn_t>(e.id); // It's not my turn anymore

            f(e, tag, turn, pos);

            // If not tagged for this work type, go idle
            if (e.component<TAG>() == nullptr) {
                e.assign(ai_mode_idle_t{});
            }
        });

    }

    void move_to(const std::size_t &entity_id, const position_t &destination) const {
        emit_deferred(entity_wants_to_move_message{entity_id, destination});
    }

    template <typename CANCEL, typename SUCCESS>
    void folllow_path(dijkstra_map &map, position_t &pos, entity_t &e, const CANCEL &cancel, const SUCCESS &success) const {
        const auto d = map.get(mapidx(pos));
        if (d > MAX_DIJSTRA_DISTANCE-1) {
            // We can't get to it
            cancel();
        }  else if (d < 1) {
            // We're here!
            success();
        } else {
            // Path towards it
            position_t destination = map.find_destination(pos);
            move_to(e.id, destination);
        }
    }

    template <typename MSG, typename CANCEL, typename SUCCESS>
    void pickup_tool(const entity_t &e, position_t &pos, const int &category, std::size_t &out_tool, const CANCEL &cancel, const SUCCESS &success) const {
        std::size_t tool_id = 0;
        each<item_t>([&tool_id, &pos, &category] (entity_t &tool, item_t &item) {
            if (!item.category.test(category)) return; // Not an axe

            auto tool_pos = tool.component<position_t>();
            if (tool_pos != nullptr && *tool_pos == pos) {
                tool_id = tool.id;
            } else {
                auto stored = tool.component<item_stored_t>();
                if (stored != nullptr) {
                    auto spos = entity(stored->stored_in)->component<position_t>();
                    if (spos != nullptr && *spos == pos) {
                        tool_id = tool.id;
                    }
                }
            }
        });
        if (tool_id > 0) {
            out_tool = tool_id;
            emit(pickup_item_message{out_tool, e.id});
            emit(MSG{});
            success();
        } else {
            // We've failed to get the tool!
            cancel();
        }
    }

    template <typename CANCEL, typename ARRIVED>
    void follow_path(TAG &tag, position_t &pos, entity_t &e, const CANCEL &&cancel, const ARRIVED &&arrived) {
        if (!tag.current_path || tag.current_path->success == false) {
            cancel();
            return;
        }
        if (pos == tag.current_path->destination || tag.current_path->steps.empty()) {
            arrived();
            return;
        }

        const position_t next_step = tag.current_path->steps.front();
        tag.current_path->steps.pop_front();
        if (next_step.x > 0 && next_step.x < REGION_WIDTH && next_step.y > 0 &&
            next_step.y < REGION_HEIGHT && next_step.z > 0 && next_step.z < REGION_DEPTH
            && region::flag(mapidx(next_step), CAN_STAND_HERE))
        {
            move_to(e.id, next_step);
        } else {
            // We couldn't get there
            tag.current_path.reset();
            cancel();
        }
    }

    inline void cancel_work_tag(entity_t &e) {
        delete_component<TAG>(e.id);
        set_status(e, "Idle");
    }

    inline void set_status(entity_t &e, const std::string &&status) {
        auto ai = e.component<settler_ai_t>();
        if (ai) {
            ai->job_status = status;
        }
    }
private:
};
