#include "ai_scheduler.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../../../components/ai_tags/ai_tag_leisure_shift.hpp"
#include "../../../components/ai_tags/ai_tag_sleep_shift.hpp"
#include "../../../components/ai_tags/ai_tag_work_shift.hpp"
#include "../../../components/sleep_clock_t.hpp"
#include "../../../components/ai_tags/ai_tag_work_guarding.hpp"
#include "../../../main/game_calendar.hpp"
#include "../../../main/game_designations.hpp"
#include "job_board.hpp"

void ai_scheduler::configure() {
    system_name = "AI Scheduler";
}

void ai_scheduler::update(const double duration_ms)
{
    // We're idle, so we need to determine what time it is and engage the appropriate AI tags
    each<ai_tag_my_turn_t, settler_ai_t, sleep_clock_t>([] (entity_t &e, ai_tag_my_turn_t &turn, settler_ai_t &ai, sleep_clock_t &sleep) {
        const int shift_id = ai.shift_id;
        const int hour_of_day = calendar->hour;
        const auto current_schedule = calendar->defined_shifts[shift_id].hours[hour_of_day];

        delete_component<ai_mode_idle_t>(e.id);
        delete_component<ai_tag_leisure_shift_t>(e.id);
        delete_component<ai_tag_work_shift_t>(e.id);
        delete_component<ai_tag_sleep_shift_t>(e.id);

        if (jobs_board::is_working(e)) return; // Don't interrupt ongoing jobs

        auto guard = e.component<ai_tag_work_guarding>();
        if (current_schedule != WORK_SHIFT && guard) {
            for (auto &g : designations->guard_points) {
                if (g.second == guard->guard_post) g.first = false;
            }
            delete_component<ai_tag_work_guarding>(e.id);
        }

        switch (current_schedule) {
            case SLEEP_SHIFT : { e.assign(ai_tag_sleep_shift_t{}); } break;
            case LEISURE_SHIFT : { e.assign(ai_tag_leisure_shift_t{}); } break;
            case WORK_SHIFT : { e.assign(ai_tag_work_shift_t{}); } break;
        }
    });
}