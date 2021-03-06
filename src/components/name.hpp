#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct name_t {
	std::string first_name;
	std::string last_name;

	name_t() {}
	name_t(const std::string fn, const std::string ln) : first_name(fn), last_name(ln) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( first_name, last_name ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<name_t>>)