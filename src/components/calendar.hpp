#pragma once

#include <rltk.hpp>
#include <string>
#include <vector>
#include "helpers/shift.hpp"

using namespace rltk;

struct calendar_t {
	std::size_t serialization_identity = 3;

	uint16_t year = 2525;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	std::vector<shift_t> defined_shifts;

	std::string get_date_time() const;
	void next_minute();
	void save(std::ostream &lbfile);
	static calendar_t load(std::istream &lbfile);

	inline float sun_arc_percent() { 
		if (hour < 12) {
			return (hour/12.0F) + ((minute/60.0F)/1000.0F);
		} else {
			return ((24.0F-hour)/12.0F) - ((minute/60.0F)/1000.0F);
		}
	}
};