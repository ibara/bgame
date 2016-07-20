#include "region.hpp"
#include <rltk.hpp>
#include <string>
#include "../raws/raws.hpp"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

using namespace rltk;

void save_region(const region_t &region) {
	std::string region_filename = "world/region_" + std::to_string(region.region_x) + "_" + std::to_string(region.region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::out | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::output> deflate;
	deflate.push(boost::iostreams::zlib_compressor());
    deflate.push(lbfile);

	serialize(deflate, region.region_x);
	serialize(deflate, region.region_y);
	serialize(deflate, region.biome_idx);
	serialize(deflate, region.next_tree_id);

	serialize(deflate, region.revealed);
	serialize(deflate, region.visible);
	serialize(deflate, region.solid);
	serialize(deflate, region.opaque);
	serialize(deflate, region.tile_type);
	serialize(deflate, region.tile_material);
	serialize(deflate, region.tile_hit_points);
	serialize(deflate, region.building_id);
	serialize(deflate, region.tree_id);
	serialize(deflate, region.tile_vegetation_type);
	serialize(deflate, region.tile_flags);
	serialize(deflate, region.water_level);
}

region_t load_region(const int region_x, const int region_y) {
	std::string region_filename = "world/region_" + std::to_string(region_x) + "_" + std::to_string(region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::in | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::input> inflate;
	inflate.push(boost::iostreams::zlib_decompressor());
    inflate.push(lbfile);
	region_t region;

	deserialize(inflate, region.region_x);
	deserialize(inflate, region.region_y);
	deserialize(inflate, region.biome_idx);
	deserialize(inflate, region.next_tree_id);

	deserialize(inflate, region.revealed);
	deserialize(inflate, region.visible);
	deserialize(inflate, region.solid);
	deserialize(inflate, region.opaque);
	deserialize(inflate, region.tile_type);
	deserialize(inflate, region.tile_material);
	deserialize(inflate, region.tile_hit_points);
	deserialize(inflate, region.building_id);
	deserialize(inflate, region.tree_id);
	deserialize(inflate, region.tile_vegetation_type);
	deserialize(inflate, region.tile_flags);
	deserialize(inflate, region.water_level);

	region.tile_recalc_all();
	return region;
}

void region_t::tile_recalc_all() {
	for (int z=0; z<REGION_DEPTH; ++z) {
        for (int y=0; y<REGION_HEIGHT; ++y) {
            for (int x=0; x<REGION_WIDTH; ++x) {
                tile_calculate(x,y,z);
            }
        }
    }
	for (int z=0; z<REGION_DEPTH; ++z) {
        for (int y=0; y<REGION_HEIGHT; ++y) {
            for (int x=0; x<REGION_WIDTH; ++x) {
                tile_pathing(x,y,z);
            }
        }
    }
}

void region_t::tile_calculate(const int &x, const int &y, const int &z) {
	const int idx = mapidx(x,y,z);

	// Calculate render characteristics
	calc_render(idx);
	
	// Solidity and first-pass standability
	if (tile_type[idx] == tile_type::SEMI_MOLTEN_ROCK || tile_type[idx] == tile_type::SOLID || tile_type[idx] == tile_type::WALL) {
		solid[idx] = true;
		tile_flags[idx].reset(CAN_STAND_HERE);
	} else {
		solid[idx] = false;

		// Locations on which one can stand
		if (tile_type[idx] == tile_type::RAMP || tile_type[idx] == tile_type::FLOOR ||
			tile_type[idx] == tile_type::STAIRS_UP || tile_type[idx] == tile_type::STAIRS_DOWN ||
			tile_type[idx] == tile_type::STAIRS_UPDOWN) 
		{
			tile_flags[idx].set(CAN_STAND_HERE);
		}

		if (z>0) {
			const int idx_below = mapidx(x,y,z-1);

			// Can stand on the tile above walls, ramps and up stairs
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::WALL) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::RAMP) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::STAIRS_UP) tile_flags[idx].set(CAN_STAND_HERE);
		}
	}
}

void region_t::tile_pathing(const int &x, const int &y, const int &z) {
	const int idx = mapidx(x,y,z);

	if (solid[idx] || !tile_flags[idx].test(CAN_STAND_HERE)) {
		// If you can't go there, it doesn't have any exits.
		tile_flags[idx].reset(CAN_GO_NORTH);
		tile_flags[idx].reset(CAN_GO_SOUTH);
		tile_flags[idx].reset(CAN_GO_EAST);
		tile_flags[idx].reset(CAN_GO_WEST);
		tile_flags[idx].reset(CAN_GO_UP);
		tile_flags[idx].reset(CAN_GO_DOWN);
	} else {
		if (x>0 && tile_flags[mapidx(x-1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_WEST);
		if (x<REGION_WIDTH-1 && tile_flags[mapidx(x+1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_EAST);
		if (y>0 && tile_flags[mapidx(x, y-1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH);
		if (y<REGION_HEIGHT-1 && tile_flags[mapidx(x, y+1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_SOUTH);

		if (z<REGION_DEPTH-1 && 
			(tile_type[idx] == tile_type::RAMP || tile_type[idx] == tile_type::STAIRS_UP || tile_type[idx] == tile_type::STAIRS_UPDOWN) && 
			tile_flags[mapidx(x,y,z+1)].test(CAN_STAND_HERE)) 
		{
			tile_flags[idx].set(CAN_GO_UP);
		}

		if (z>0 && (tile_type[idx] == tile_type::STAIRS_DOWN || tile_type[idx] == tile_type::STAIRS_UPDOWN ) && tile_flags[mapidx(x,y,z-1)].test(CAN_STAND_HERE)) {
			tile_flags[idx].set(CAN_GO_DOWN);
		}

		if (z>0 && tile_type[idx] == tile_type::OPEN_SPACE && tile_type[mapidx(x,y,z-1)]==tile_type::RAMP) {
			tile_flags[idx].set(CAN_GO_DOWN);
		}
	}
}

void region_t::calc_render(const int &idx) {
	uint8_t glyph;
	color_t fg;
	color_t bg = rltk::colors::BLACK;

	// Start with the basic tile_type; this hard-sets some glyphs.
	switch (tile_type[idx]) {
		case tile_type::SEMI_MOLTEN_ROCK : {
			glyph = 178;
			fg = rltk::colors::RED;
			bg = rltk::colors::YELLOW;
		} break;
		case tile_type::SOLID : {
			glyph = material_defs[tile_material[idx]].glyph;
			fg = material_defs[tile_material[idx]].fg;
			bg = material_defs[tile_material[idx]].bg;
		} break;
		case tile_type::OPEN_SPACE : {
			glyph = ' ';
			fg = rltk::colors::BLACK;
		} break;
		case tile_type::WALL : {
			// TODO: Color determined by material, glyph by other walls adjacent
			glyph = 219;
			fg = material_defs[tile_material[idx]].fg;
			//bg = material_defs[tile_material[idx]].bg;
		} break;
		case tile_type::RAMP : {
			// TODO: Color determined by material
			glyph = 30;
			fg = material_defs[tile_material[idx]].fg;

			if (tile_vegetation_type[idx]>0) {
				glyph = plant_defs[tile_vegetation_type[idx]].glyph;
				fg = plant_defs[tile_vegetation_type[idx]].fg;
				bg = plant_defs[tile_vegetation_type[idx]].bg;
			}
		} break;
		case tile_type::STAIRS_UP : {
			// TODO: Color determined by material
			glyph = '<';
			fg = material_defs[tile_material[idx]].fg;
		} break;
		case tile_type::STAIRS_DOWN : {
			// TODO: Color determined by material
			glyph = '>';
			fg = material_defs[tile_material[idx]].fg;
		} break;
		case tile_type::STAIRS_UPDOWN : {
			// TODO: Color determined by material
			glyph = 'X';
			fg = material_defs[tile_material[idx]].fg;
		} break;
		case tile_type::FLOOR : {	
			if (material_defs[tile_material[idx]].spawn_type == sand) {
				glyph = 247;
			} else {
				glyph = ',';
			}
			fg = material_defs[tile_material[idx]].fg;

			if (tile_vegetation_type[idx]>0) {
				//std::cout << plant_defs[tile_vegetation_type[idx]].name << "\n";
				glyph = plant_defs[tile_vegetation_type[idx]].glyph;
				fg = plant_defs[tile_vegetation_type[idx]].fg;
				bg = plant_defs[tile_vegetation_type[idx]].bg;
			}
		} break;
		case tile_type::TREE_TRUNK : {
			glyph = 10;
			fg = rltk::colors::Brown;
			bg = rltk::colors::Black;
		} break;
		case tile_type::TREE_LEAF : {
			glyph = 177;
			fg = rltk::colors::Green;
			bg = rltk::colors::Black;
		} break;
	}

	if (water_level[idx]>0) {
		switch (water_level[idx]) {
			case 1 : glyph = '1'; break;
			case 2 : glyph = '2'; break;
			case 3 : glyph = '3'; break;
			case 4 : glyph = '4'; break;
			case 5 : glyph = '5'; break;
			case 6 : glyph = '6'; break;
			case 7 : glyph = '7'; break;
			case 8 : glyph = '8'; break;
			case 9 : glyph = '9'; break;
			case 10 : glyph = '0'; break;
			default : glyph = '~';
		}
		fg = rltk::colors::Blue;
		bg = rltk::colors::DarkBlue;
	}

	// Apply it
	render_cache[idx] = vchar{glyph, fg, bg}; 
}