#include "ai_work_harvest.hpp"
#include "job_board.hpp"
#include "../../../components/ai_tags/ai_tag_work_harvest.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/item.hpp"
#include "../distance_map_system.hpp"
#include "../../../utils/telemetry.hpp"
#include "ai_work_template.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/raws.hpp"
#include "../../../planet/region/region.hpp"
#include "../../../raws/defs/item_def_t.hpp"
#include "../../../raws/defs/plant_t.hpp"

using namespace region;

namespace jobs_board {
    void evaluate_harvest(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->harvest.empty()) return; // Nothing to cut down

        auto harvest_distance = harvest_map.get(mapidx(pos));
        if (harvest_distance > MAX_DIJSTRA_DISTANCE-1) return; // Nothing to harvest

        board.insert(std::make_pair(harvest_distance, jt));
    }
}

void ai_work_harvest::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_harvest>(jobs_board::evaluate_harvest);
}

void ai_work_harvest::update(const double duration_ms) {
    ai_work_template<ai_tag_work_harvest> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_harvest &h, ai_tag_my_turn_t &t, position_t &pos) {
        work.set_status(e, "Harvesting");
        if (h.step == ai_tag_work_harvest::harvest_steps::FIND_HARVEST) {
            // Path towards the harvest
            work.folllow_path(harvest_map, pos, e, [&e, &work]() {
                // On cancel
                work.cancel_work_tag(e);
                return;
            }, [&e, this, &pos, &h, &work] {
                // On success
                h.step = ai_tag_work_harvest::harvest_steps::DO_HARVEST;
            });
            return;
        } else if (h.step == ai_tag_work_harvest::harvest_steps::DO_HARVEST) {
            const int idx = mapidx(pos);
            designations->harvest.erase(std::remove_if(
                    designations->harvest.begin(),
                    designations->harvest.end(),
                    [&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
                                        ),
                                        designations->harvest.end());

            // Create the harvesting result
            if (veg_type(idx) == 0) {
                work.cancel_work_tag(e);
                return;
            }
            const auto plant = get_plant_def(veg_type(idx));
            const std::string result = plant->provides[veg_lifecycle(idx)];
            if (result != "none") {
                std::string mat_type = "organic";
                auto item_finder = get_item_def(result);
                if (item_finder != nullptr) {
                    if (item_finder->categories.test(ITEM_FOOD)) mat_type="food";
                    if (item_finder->categories.test(ITEM_SPICE)) mat_type="spice";
                }
                auto item = spawn_item_on_ground_ret(pos.x, pos.y, pos.z, result, get_material_by_tag(mat_type));
                item->component<item_t>()->item_name = plant->name;
            }

            // Knock tile back to germination
            set_veg_lifecycle(idx, 0);
            set_veg_ticker(idx, 0);

            // Become idle - done
            call_home("harvest", result);
            work.cancel_work_tag(e);
            return;
        }
    });
}
