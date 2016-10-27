#pragma once

#include <rltk.hpp>
#include <FastNoise/FastNoise.h>
#include "../../planet.hpp"
#include "../../region.hpp"
#include "../../../raws/raws.hpp"

struct strata_t {
    std::vector<int> strata_map;
    std::vector<std::size_t> material_idx;
    std::vector<std::tuple<int,int,int,int>> counts;
};

std::unique_ptr<strata_t> build_strata(region_t &region, std::vector<uint8_t> &heightmap, random_number_generator &rng, std::pair<biome_t, biome_type_t> &biome, planet_t &planet);
void lay_strata(region_t &region, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, strata_t &strata, random_number_generator &rng, std::vector<uint8_t> &pools, std::vector<std::pair<int, uint8_t>> &water_spawners);