#pragma once

#include <rltk.hpp>
#include <vector>
#include "boost/container/flat_set.hpp"

using namespace rltk;

struct viewshed_t {
	viewshed_t() {}
	viewshed_t(const int radius, const bool pen, const bool good_guy=true) : viewshed_radius(radius), penetrating(pen), good_guy_visibility(good_guy) {}

	int viewshed_radius = 0;
	bool penetrating = false;
	bool good_guy_visibility = true;
	boost::container::flat_set<std::size_t> visible_entities;

	// Non-persistent
	std::vector<int> visible_cache;

	std::size_t serialization_identity = 17;

	void save(std::ostream &lbfile);
	static viewshed_t load(std::istream &lbfile);
};