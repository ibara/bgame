#pragma once

#include <rltk.hpp>

class corpse_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
