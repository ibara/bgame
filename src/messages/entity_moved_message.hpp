#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

struct entity_wants_to_move_randomly_message : public rltk::base_message_t {
	entity_wants_to_move_randomly_message() {}
	entity_wants_to_move_randomly_message(std::size_t id) : entity_id(id) {}
	std::size_t entity_id;
};

struct entity_wants_to_flee_message : public rltk::base_message_t {
	entity_wants_to_flee_message() {}
	entity_wants_to_flee_message(std::size_t id, std::size_t flee_from) : entity_id(id), flee_from_id(flee_from) {}
	std::size_t entity_id;
	std::size_t flee_from_id;
};

struct entity_wants_to_charge_message : public rltk::base_message_t {
	entity_wants_to_charge_message() {}
	entity_wants_to_charge_message(std::size_t id, std::size_t charge_to) : entity_id(id), charge_to_id(charge_to) {}
	std::size_t entity_id;
	std::size_t charge_to_id;
};

struct entity_wants_to_move_message : public rltk::base_message_t {
	entity_wants_to_move_message() {}
	entity_wants_to_move_message(std::size_t id, const position_t dest) : entity_id(id), destination(dest) { }
	std::size_t entity_id;
	position_t destination;
};

struct entity_moved_message : public rltk::base_message_t {
	entity_moved_message() {}
	entity_moved_message(std::size_t id, const position_t &orig, const position_t &dest) : entity_id(id), origin(orig), destination(dest) {}
	std::size_t entity_id;
	position_t origin;
	position_t destination;
};

struct huntable_moved_message : public rltk::base_message_t {};
struct butcherable_moved_message : public rltk::base_message_t {};
struct bed_changed_message : public rltk::base_message_t {};
struct settler_moved_message : public rltk::base_message_t {};