#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"
#include "../../main/game_building.hpp"

struct build_request_message : public rltk::base_message_t {
    int x, y, z;
    available_building_t building;
	build_request_message() {}
    build_request_message(const int X, const int Y, const int Z, const available_building_t build) : x(X), y(Y), z(Z), building(build) {}
};

struct cancel_build_request_message : public rltk::base_message_t {
    std::size_t building_entity = 0;
    cancel_build_request_message() {}
    cancel_build_request_message(const std::size_t &id) : building_entity(id) {}
};

struct door_changed_message : public rltk::base_message_t {  
};
