#pragma once

#include <rltk.hpp>

const double MS_PER_TICK = 2.0;

class calendar_system : public rltk::base_system {
public:
	virtual void configure() override final;
	virtual void update(const double duration_ms) override final;
private:
	double time_count = 100.0;
	int slow_tick_count = 0;
};
