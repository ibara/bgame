#pragma once

#include <rltk.hpp>

using namespace rltk;

struct calendar_t {
	std::size_t serialization_identity = 3;

	void save(std::ostream &lbfile) {
	}
};