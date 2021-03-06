#include "strata.hpp"
#include "../../planet_builder.hpp"
#include "../../components/water_spawner.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/plants.hpp"
#include "../../region/region.hpp"

using namespace region;

std::vector<std::size_t> soils;
std::vector<std::size_t> sands;

strata_t build_strata(std::vector<uint8_t> &heightmap, random_number_generator &rng, std::pair<biome_t, biome_type_t> &biome, planet_t &planet) {
    strata_t result;
    result.strata_map.resize(REGION_TILES_COUNT);

    std::vector<std::size_t> sedimintaries;
    std::vector<std::size_t> igneouses;
    soils.clear();
    sands.clear();

    get_strata_materials(soils, sedimintaries, igneouses, sands);

    set_worldgen_status("Locating strata");
    const int n_strata = (REGION_WIDTH + REGION_HEIGHT)*4 + rng.roll_dice(1,64);
    result.strata_map.resize(REGION_TILES_COUNT);
    result.material_idx.resize(n_strata);
    std::fill(result.strata_map.begin(), result.strata_map.end(), 1);
    std::fill(result.material_idx.begin(), result.material_idx.end(), 1);
    std::fill(result.counts.begin(), result.counts.end(), std::make_tuple<int,int,int,int>(0,0,0,0));
    result.counts.resize(n_strata);

    FastNoise biome_noise(planet.perlin_seed + (region_y() * REGION_WIDTH ) + region_x());
    biome_noise.SetNoiseType(FastNoise::Cellular);

    for (int z=0; z<REGION_DEPTH; ++z) {
        const float Z = (float)z*8.0F;
        for (int y=0; y<REGION_WIDTH; ++y) {
            const float Y = (float)y*8.0F;
            for (int x=0; x<REGION_HEIGHT; ++x) {
                const float X = (float)x*8.0F;
                const float cell_noise = biome_noise.GetCellular(X,Y,Z);
                const float biome_ramp = (cell_noise + 2.0F)/4.0F;
                assert(biome_ramp > 0.0F);
                assert(biome_ramp < 1.0F);
                int biome_idx = static_cast<int>(biome_ramp * (float)n_strata);
                ++std::get<0>(result.counts[biome_idx]);
                std::get<1>(result.counts[biome_idx]) += x;
                std::get<2>(result.counts[biome_idx]) += y;
                std::get<3>(result.counts[biome_idx]) += z;
                const int map_idx = mapidx(x,y,z);
                result.strata_map[map_idx] = biome_idx;
                //std::cout << x << "/" << y << "/" << z << " : " << X << "/" << Y << "/" << Z << " = " << cell_noise << "/" << biome_ramp << " : " << biome_idx << "\n";
            }
        }
    }

    int count_used = 0;
    for (int i=0; i<n_strata; ++i) {
        if (std::get<0>(result.counts[i])>0) {
            ++count_used;
            std::get<1>(result.counts[i]) /= std::get<0>(result.counts[i]);
            std::get<2>(result.counts[i]) /= std::get<0>(result.counts[i]);
            std::get<3>(result.counts[i]) /= std::get<0>(result.counts[i]);

            int n,x,y,z;
            std::tie(n,x,y,z) = result.counts[i];
            const uint8_t altitude_at_center = heightmap[(y * REGION_WIDTH) + x] + REGION_DEPTH/2;

            if (z>altitude_at_center-(1+rng.roll_dice(1,4))) {
                // Soil
                int roll = rng.roll_dice(1,100);
                if (roll < biome.second.soil_pct) {
                    const std::size_t soil_idx = rng.roll_dice(1, soils.size())-1;
                    //std::cout << material_name(soils[soil_idx]) << "\n";
                    result.material_idx[i] = soils[soil_idx];
                } else {
                    const std::size_t sand_idx = rng.roll_dice(1, sands.size())-1;
                    //std::cout << material_name(sands[sand_idx]) << "\n";
                    result.material_idx[i] = sands[sand_idx];
                }
            } else if (z>(altitude_at_center-10)/2) {
                // Sedimentary
                const std::size_t sed_idx = rng.roll_dice(1, sedimintaries.size())-1;
                //std::cout << material_name(sedimintaries[sed_idx]) << "\n";
                result.material_idx[i] = sedimintaries[sed_idx];
            } else {
                // Igneous
                const std::size_t ig_idx = rng.roll_dice(1, igneouses.size())-1;
                //std::cout << material_name(igneouses[ig_idx]) << "\n";
                result.material_idx[i] = igneouses[ig_idx];
            }
        } else {
            //std::cout << "Warning - we didn't think this strata was in use!\n";
            result.material_idx[i] = 1;
        }
    }
    std::cout << count_used << " strata detected, " << n_strata - count_used << " unused.\n";

    return result;
}

void lay_strata(std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, strata_t &strata, random_number_generator &rng, std::vector<uint8_t> &pools, std::vector<std::pair<int, uint8_t>> &water_spawners) {
    // For vegetation
    int max_veg_probability = 0;
    for (const auto &vegprob : biome.second.plants) max_veg_probability += vegprob.second;

    // Lay down layers
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int cell_idx = (y * REGION_WIDTH) + x;
            uint8_t altitude = heightmap[cell_idx];
            if (altitude > REGION_DEPTH-10) altitude = REGION_DEPTH-1;
            bool wet = false;
            if (altitude < 5) wet = true;

            // The bottom layer is always SMR to avoid spill-through
            set_tile_type(mapidx(x,y,0), tile_type::SEMI_MOLTEN_ROCK);

            int z = 1;
            while (z < altitude) {
                // Place Lava area - SMR for now
                if (x==0 || x==REGION_WIDTH-1 || y==0 || y==REGION_HEIGHT-1) {
                    set_tile_type(mapidx(x,y,z), tile_type::SEMI_MOLTEN_ROCK);
                } else {
                    set_tile_type(mapidx(x,y,z), tile_type::OPEN_SPACE);
                    set_tile_material(mapidx(x,y,z), 0);
                    // TODO: Just add magma
                }
                ++z;
            }

            // Next up is rock, until the soil layer
            while(z < std::min(altitude + 64, REGION_DEPTH-20)) {
                // Place rock and soil
                set_tile_type(mapidx(x,y,z), tile_type::SOLID);
                std::size_t material_idx;
                const int idx = mapidx(x,y,z);
                if (idx < strata.strata_map.size()) { 
                    const int strata_idx = strata.strata_map[mapidx(x,y,z)];
                    if (strata_idx < strata.material_idx.size()) {
                        material_idx = strata.material_idx[strata_idx];
                    } else {
                        material_idx = 1;
                        std::cout << "Warning - exceeded strata_material size\n";
                    }
                } else {
                    std::cout << "Warning - exceeded strata_map size (" << strata.strata_map.size() << ")\n";
                    material_idx = 1;
                }
                set_tile_material(mapidx(x,y,z), material_idx);
                ++z;
            }
            
            // Populate the surface tile at z-1
            reveal(mapidx(x,y,z-1));
            set_tile_type(mapidx(x,y,z-1), tile_type::FLOOR);
            if (wet) {
                set_water_level(mapidx(x,y,z-1), 10); // Below the water line; flood it!
            } else {
                if (pools[cell_idx]>0) {
                    int w = pools[cell_idx];
                    int Z = z-1;

                    for (const auto &ws : water_spawners) {
                        if (ws.first == cell_idx) {
                            create_entity()->assign(position_t{x,y,Z})->assign(water_spawner_t{ws.second});
                        }
                    }

                    while (w > 0) {
                        set_water_level(mapidx(x,y,Z), 10);
                        w -= 10;
                        ++Z;
                    }
                } else {

                    set_water_level(mapidx(x,y,z-1), 0);

                    /*
                    // Soil/sand
                    int roll = rng.roll_dice(1,100);
                    if (roll < biome.second.soil_pct) {
                        const std::size_t soil_idx = rng.roll_dice(1, soils.size())-1;
                        //std::cout << material_name(soils[soil_idx]) << "\n";
                        region.tile_material[mapidx(x,y,z-1)] = soils[soil_idx];
                    } else {
                        const std::size_t sand_idx = rng.roll_dice(1, sands.size())-1;
                        //std::cout << material_name(sands[sand_idx]) << "\n";
                        region.tile_material[mapidx(x,y,z-1)] = sands[sand_idx];
                    }*/

                    // Surface coverage
                    std::string veg_type = "";
                    int die_roll = rng.roll_dice(1, max_veg_probability);
                    for (const auto &veg : biome.second.plants) {
                        die_roll -= veg.second;
                        if (die_roll < 1) {
                            veg_type = veg.first;
                            break;
                        }
                    }
                    if (veg_type == "") veg_type = "none";

                    if (veg_type != "none") {
                        auto finder = get_plant_idx(veg_type);
                        set_veg_type(mapidx(x,y,z-1), finder);
                        set_veg_hp(mapidx(x,y,z-1), 10);
                        set_veg_ticker(mapidx(x,y,z-1), 1);
                        set_veg_lifecycle(mapidx(x,y,z-1), rng.roll_dice(1,4)-1);
                    }
                }
            }

            while (z<REGION_DEPTH) {
                // Place sky
                reveal(mapidx(x,y,z));
                set_tile_type(mapidx(x,y,z), tile_type::OPEN_SPACE);
                if (z < 69) set_water_level(mapidx(x,y,z), 10); // Below the water line; flood it!
                ++z;
            }
        }
    }
}
