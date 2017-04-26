#include "ai_work_time.hpp"
#include "../../components/ai_tag_work_shift.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/position.hpp"
#include "distance_map_system.hpp"
#include "../../components/ai_mode_idle.hpp"
#include "../../components/ai_tag_work_lumberjack.hpp"
#include "../../components/ai_tag_work_mining.hpp"
#include "../../components/ai_tag_work_guarding.hpp"
#include "../damage/weapons_helpers.hpp"
#include "mining_system.hpp"
#include <map>
#include "../../main/game_designations.hpp"
#include "job_board.cpp"

namespace jobs_board {    

    std::map<int, job_type_t> job_evaluations(entity_t &e, position_t &pos) {
        job_board_t board;
        evaluate(board, e, pos);

        return board;
    }
}

void ai_work_time::configure() {

}

void ai_work_time::update(const double duration_ms) {

    each<settler_ai_t, ai_tag_work_shift_t, position_t>([] (entity_t &e, settler_ai_t &ai, ai_tag_work_shift_t &work, position_t &pos) {
        // Do we already have a job? If so, then return to doing it!
        if (e.component<ai_tag_work_lumberjack>() != nullptr) return;
        if (e.component<ai_tag_work_miner>() != nullptr) return;
        if (e.component<ai_tag_work_guarding>() != nullptr) return;

        // Build a job candidates list, goal is to pick the easiest job to complete.
        auto available_jobs = jobs_board::job_evaluations(e, pos);

        if (available_jobs.empty()) {
            std::cout << "No available jobs\n";
            // There is nothing to do!
            e.assign(ai_mode_idle_t{});
            return;
        } else {
            delete_component<ai_tag_work_shift_t>(e.id);
        }

        auto job_type = available_jobs.begin()->second;
        switch (job_type) {
            case jobs_board::LUMBERJACK : { e.assign(ai_tag_work_lumberjack{}); } break;
            case jobs_board::MINING : { e.assign(ai_tag_work_miner{}); } break;
            case jobs_board::GUARD : { e.assign(ai_tag_work_guarding{}); } break;
        }
    });
}