#pragma once

#include <rltk.hpp>
#include <bitset>
#include <cereal/cereal.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct stockpile_t {

    stockpile_t() {}

    std::bitset<128> category;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( category ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<stockpile_t>>)