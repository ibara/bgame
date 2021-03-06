#pragma once

#include <rltk.hpp>

class wish_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    void make_wish(const std::string &wish);
	char wish_command[255] = "";
};
