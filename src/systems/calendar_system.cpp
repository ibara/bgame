#include "calendar_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/messages.hpp"

void calendar_system::configure() {
	system_name = "Calendar";

	subscribe<pause_requested_message>([this] (pause_requested_message &msg) { requested_pause = true; });
	subscribe<one_step_requested_message>([this] (one_step_requested_message &msg) { requested_step = true; });
}

void calendar_system::update(const double duration_ms) {
	if (pause_mode != PAUSED) {
		time_count += duration_ms;

		if (time_count > MS_PER_TICK) {
			time_count = 0.0;

			auto hour = calendar->hour;
			auto day = calendar->day;
			calendar->next_minute();
			emit(tick_message{});
			++slow_tick_count;
			if (slow_tick_count > 9) {
				slow_tick_count = 0;
				emit(slow_tick_message{});
			}
			if (calendar->hour != hour) emit_deferred(hour_elapsed_message{});
			if (calendar->day != day) emit_deferred(day_elapsed_message{});

			if (pause_mode == ONE_STEP) {
				pause_mode = PAUSED;
			}
		}
	}

	if (game_master_mode == PLAY) {
		if (requested_pause) {
			if (pause_mode == RUNNING) {
				pause_mode = PAUSED;
			} else {
				pause_mode = RUNNING;
			}
		}
		if (requested_step) {
			pause_mode = ONE_STEP;
		}
	}

	requested_pause = false;
	requested_step = false;
}
