#include "raws.hpp"
#include "lua_bridge.hpp"
#include "../components/components.hpp"
#include "../utils/string_utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

std::unique_ptr<lua_lifecycle> lua_handle;

string_table_t first_names_male;
string_table_t first_names_female;
string_table_t last_names;

boost::container::flat_map<uint8_t, tile_type_t> tile_types;
boost::container::flat_map<std::string, uint8_t> tile_type_index;
boost::container::flat_map<uint16_t, tile_content_t> tile_contents;
boost::container::flat_map<std::string, uint16_t> tile_contents_index;

boost::container::flat_map<std::string, clothing_t> clothing_types;
std::vector<profession_t> starting_professions;

boost::container::flat_map<std::string, item_def_t> item_defs;
boost::container::flat_map<std::string, building_def_t> building_defs;
boost::container::flat_map<std::string, reaction_t> reaction_defs;
boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

std::vector<biome_type_t> biome_defs;
boost::container::flat_map<std::string, std::size_t> material_defs_idx;
std::vector<material_def_t> material_defs;
boost::container::flat_map<std::string, std::size_t> plant_defs_idx;
std::vector<plant_t> plant_defs;

boost::container::flat_map<std::string, raw_species_t> species_defs;
boost::container::flat_map<std::string, raw_creature_t> creature_defs;

boost::container::flat_map<std::string, std::vector<native_population_t>> native_pop_defs;

void load_string_table(const std::string filename, string_table_t &target) {
	std::ifstream f(filename);
	std::string line;
	while (getline(f, line))
	{
		target.strings.push_back(line);
	}
}

rltk::color_t read_lua_color(std::string field) {
	rltk::color_t col;
	lua_pushstring(lua_state, field.c_str());
	lua_gettable(lua_state, -2);
	while (lua_next(lua_state, -2) != 0) {
		std::string subfield = lua_tostring(lua_state, -2);
		if (subfield == "r") col.r = lua_tonumber(lua_state, -1);
		if (subfield == "g") col.g = lua_tonumber(lua_state, -1);
		if (subfield == "b") col.b = lua_tonumber(lua_state, -1);
		lua_pop(lua_state, 1);
	}
	return col;
}

void read_clothing(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "clothing");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        clothing_t c;

        std::string key = lua_tostring(lua_state, -2);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "slot") c.slot = lua_tostring(lua_state, -1);
            if (field == "description") c.description = lua_tostring(lua_state, -1);
            if (field == "ac") c.armor_class = lua_tonumber(lua_state, -1);
            if (field == "colors") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string color = lua_tostring(lua_state, -1);
                    c.colors.push_back(color);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }
        clothing_types[key] = c;

        lua_pop(lua_state, 1);
    }
}

void read_professions(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "starting_professions");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);
        profession_t p;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "name") p.name = lua_tostring(lua_state, -1);
            // Stat mods
            if (field == "modifiers") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string stat = lua_tostring(lua_state, -2);
                    int modifier = lua_tonumber(lua_state, -1);
                    if (stat == "str") p.strength = modifier;
                    if (stat == "dex") p.dexterity = modifier;
                    if (stat == "con") p.constitution = modifier;
                    if (stat == "int") p.intelligence = modifier;
                    if (stat == "wis") p.wisdom = modifier;
                    if (stat == "cha") p.charisma = modifier;
                    if (stat == "com") p.comeliness = modifier;
                    if (stat == "eth") p.ethics = modifier;
                    lua_pop(lua_state, 1);
                }
            }

            // Starting clothes
            if (field == "clothing") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string gender_specifier = lua_tostring(lua_state, -2);
                    lua_pushstring(lua_state, gender_specifier.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string slot = lua_tostring(lua_state, -2);
                        const std::string item = lua_tostring(lua_state, -1);
                        int gender_tag = 0;
                        if (gender_specifier == "male") gender_tag = 1;
                        if (gender_specifier == "female") gender_tag = 2;
                        p.starting_clothes.push_back( std::make_tuple(gender_tag, slot, item));
                        lua_pop(lua_state, 1);
                    }
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }
        starting_professions.push_back(p);
        lua_pop(lua_state, 1);
    }
}

void read_items(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "items");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        item_def_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "description") c.description = lua_tostring(lua_state, -1);
            if (field == "background") c.bg = read_lua_color("background");
            if (field == "foreground") c.fg = read_lua_color("foreground");
            if (field == "glyph") c.glyph = lua_tonumber(lua_state, -1);
            if (field == "itemtype") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -1);
                    if (type == "component") c.categories.set(COMPONENT);
                    if (type == "tool-chopping") c.categories.set(TOOL_CHOPPING);
                    if (type == "tool-digging") c.categories.set(TOOL_DIGGING);
                    if (type == "weapon-melee") c.categories.set(WEAPON_MELEE);
                    if (type == "weapon-ranged") c.categories.set(WEAPON_RANGED);
                    if (type == "ammo") c.categories.set(WEAPON_AMMO);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "damage_n") c.damage_n = lua_tonumber(lua_state, -1);
            if (field == "damage_d") c.damage_d = lua_tonumber(lua_state, -1);
            if (field == "damage_mod") c.damage_mod = lua_tonumber(lua_state, -1);
            if (field == "ammo") c.ammo = lua_tostring(lua_state, -1);
            if (field == "range") c.range = lua_tonumber(lua_state, -1);
            if (field == "stack_size") c.stack_size = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        item_defs[key] = c;
        //tech_tree_file << "\"" << key << "\"\n";

        lua_pop(lua_state, 1);
    }
}

void read_buildings(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "buildings");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        building_def_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "components") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string comp_key = lua_tostring(lua_state, -1);
                    c.components.push_back(comp_key);
                    tech_tree_file << "item_" << comp_key << " -> " <<  key << "\n";
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "skill") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "name") c.skill.first = lua_tostring(lua_state, -1);
                    if (type == "difficulty") c.skill.second = lua_tonumber(lua_state, -1);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "provides") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);

                while (lua_next(lua_state, -2) != 0) {
                    building_provides_t provisions;
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "table") provisions.provides = provides_desk;
                    if (type == "wall") provisions.provides = provides_wall;
                    if (type == "door") provisions.provides = provides_door;
                    if (type == "food") provisions.provides = provides_food;
                    if (type == "sleep") provisions.provides = provides_sleep;
                    if (type == "floor") provisions.provides = provides_floor;
                    if (type == "stairs_up") provisions.provides = provides_stairs_up;
                    if (type == "stairs_down") provisions.provides = provides_stairs_down;
                    if (type == "stairs_updown") provisions.provides = provides_stairs_updown;
                    if (type == "ramp") provisions.provides = provides_ramp;
                    if (type == "light") provisions.provides = provides_light;

                    lua_pushstring(lua_state, type.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string inner_type = lua_tostring(lua_state, -2);
                        if (inner_type == "energy_cost") provisions.energy_cost = lua_tonumber(lua_state, -1);
                        if (inner_type == "radius") provisions.radius = lua_tonumber(lua_state, -1);
                        if (inner_type == "color") provisions.color = read_lua_color("color");
                        lua_pop(lua_state, 1);
                    }

                    c.provides.push_back(provisions);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string type = lua_tostring(lua_state, -2);
                    if (type == "width") c.width = lua_tonumber(lua_state, -1);
                    if (type == "height") c.height = lua_tonumber(lua_state, -1);
                    if (type == "tiles") {
                        lua_pushstring(lua_state, type.c_str());
                        lua_gettable(lua_state, -2);
                        int i = 0;
                        while (lua_next(lua_state, -2) != 0) {
                            rltk::vchar render;
                            lua_pushnumber(lua_state, i);
                            lua_gettable(lua_state, -2);
                            while (lua_next(lua_state, -2) != 0) {
                                std::string tiletag = lua_tostring(lua_state, -2);
                                if (tiletag == "glyph") render.glyph = lua_tonumber(lua_state, -1);
                                if (tiletag == "foreground") render.foreground = read_lua_color("foreground");
                                if (tiletag == "background") render.background = read_lua_color("background");
                                lua_pop(lua_state, 1);
                            }                       
                            lua_pop(lua_state, 1);
                            ++i;
                            c.glyphs.push_back(render);
                        }
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "render_rex") {
                std::string filename = "rex/" + std::string(lua_tostring(lua_state, -1));
                xp::rex_sprite sprite(filename);
                c.width = sprite.get_width();
                c.height = sprite.get_height();
                for (int y=0; y<c.height; ++y) {
                    for (int x=0; x<c.width; ++x) {
                        c.glyphs.push_back(*sprite.get_tile(0,x,y));
                    }
                }
            }

            lua_pop(lua_state, 1);
        }
        building_defs[key] = c;

        lua_pop(lua_state, 1);
    }
}

void read_reactions(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "reactions");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        reaction_t c;

        std::string key = lua_tostring(lua_state, -2);
        c.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "emits_smoke") c.emits_smoke = lua_toboolean(lua_state, -1);
            if (field == "workshop") c.workshop = lua_tostring(lua_state, -1);
            if (field == "skill") c.skill = lua_tostring(lua_state, -1);
            if (field == "difficulty") c.difficulty = lua_tonumber(lua_state, -1);
            if (field == "inputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);
                    std::pair<std::string, int> comp;
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") comp.first = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.second = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    c.inputs.push_back(comp);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "outputs") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    lua_pushnil(lua_state);
                    lua_gettable(lua_state, -2);
                    std::pair<std::string, int> comp;
                    while (lua_next(lua_state, -2) != 0) {
                        std::string f = lua_tostring(lua_state, -2);
                        if (f == "item") comp.first = lua_tostring(lua_state, -1);
                        if (f == "qty") comp.second = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    c.outputs.push_back(comp);
                    //tech_tree_file << "\"" << key << "\"" << comp.first << "\"\n";

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "automatic") c.automatic = lua_toboolean(lua_state, -1);
            if (field == "power_drain") c.power_drain = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        reaction_defs[key] = c;
        reaction_building_defs[c.workshop].push_back(key);
        for (const auto &input : c.inputs) {
            tech_tree_file << "item_" << input.first << " -> " << c.workshop << "\n";
        }
        for (const auto &output : c.outputs) {
            tech_tree_file << c.workshop << " -> item_" << output.first << "\n";
        }

        lua_pop(lua_state, 1);
    }
}

void read_material_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "materials");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        material_def_t m;
        m.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") m.name = lua_tostring(lua_state, -1);
            if (field == "type") {
                std::string type_s = lua_tostring(lua_state, -1);
                if (type_s == "cluster_rock") m.spawn_type = cluster_rock;
                if (type_s == "rock") m.spawn_type = rock;
                if (type_s == "soil") m.spawn_type = soil;
                if (type_s == "sand") m.spawn_type = sand;
            }
            if (field == "parent") m.parent_material_tag = lua_tostring(lua_state, -1);
            if (field == "glyph") m.glyph = lua_tonumber(lua_state, -1);
            if (field == "fg") m.fg = read_lua_color("fg");
            if (field == "bg") m.bg = read_lua_color("bg");
            if (field == "hit_points") m.hit_points = lua_tonumber(lua_state, -1);
            if (field == "mines_to") m.mines_to_tag = lua_tostring(lua_state, -1);
            if (field == "mines_to_also") m.mines_to_tag_second = lua_tostring(lua_state, -1);
            if (field == "layer") m.layer = lua_tostring(lua_state, -1);           

            lua_pop(lua_state, 1);
        }
        material_defs.push_back(m);
        if (m.mines_to_tag.size() > 1)
            tech_tree_file << key << " -> mining -> item_" << m.mines_to_tag << "\n"; 
        if (m.mines_to_tag_second.size() > 1)
            tech_tree_file << key << " -> mining -> item_" << m.mines_to_tag_second << "\n"; 

        lua_pop(lua_state, 1);
    }

    std::sort(material_defs.begin(), material_defs.end(), [] (material_def_t a, material_def_t b) {
        return a.tag < b.tag;
    });
    for (std::size_t material_index = 0; material_index < material_defs.size(); ++material_index) {
        material_defs_idx[material_defs[material_index].tag] = material_index;
    }
}

void read_plant_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "vegetation");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        plant_t p;
        p.tag = key;
        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while(lua_next(lua_state, -2) != 0)
        {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "name") p.name = lua_tostring(lua_state, -1);
            if (field == "glyph") p.glyph = lua_tonumber(lua_state, -1);
            if (field == "fg") p.fg = read_lua_color("fg");
            if (field == "bg") p.bg = read_lua_color("bg");
            if (field == "provides") p.provides = lua_tostring(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        plant_defs.push_back(p);
        if (p.provides.size() > 1) {
            tech_tree_file << key << " -> farming -> item_" << p.provides << "\n";
        }
        lua_pop(lua_state, 1);
    }

    std::sort(plant_defs.begin(), plant_defs.end(), [] (plant_t a, plant_t b) { return a.tag < b.tag; });
    for (std::size_t i=0; i<plant_defs.size(); ++i) {
        plant_defs_idx[plant_defs[i].tag] = i;
    }
}

void read_biome_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "biomes");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        biome_type_t b;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") b.name = lua_tostring(lua_state, -1);
            if (field == "min_temp") b.min_temp = lua_tonumber(lua_state, -1);
            if (field == "max_temp") b.max_temp = lua_tonumber(lua_state, -1);
            if (field == "min_rain") b.min_rain = lua_tonumber(lua_state, -1);
            if (field == "max_rain") b.max_rain = lua_tonumber(lua_state, -1);
            if (field == "min_mutation") b.min_mutation = lua_tonumber(lua_state, -1);
            if (field == "max_mutation") b.max_mutation = lua_tonumber(lua_state, -1);
            if (field == "soils") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string soil_type = lua_tostring(lua_state, -2);
                    if (soil_type == "soil" ) b.soil_pct = lua_tonumber(lua_state, -1);
                    if (soil_type == "sand" ) b.soil_pct = lua_tonumber(lua_state, -1);
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "occurs") {
                // List of biome type indices
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    b.occurs.push_back(lua_tonumber(lua_state, -1));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "worldgen_render") {
                // Load glyph and color
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string sub_field = lua_tostring(lua_state, -2);
                    if (sub_field == "glyph") b.worldgen_glyph = lua_tonumber(lua_state, -1);
                    if (sub_field == "color") b.worldgen_color = read_lua_color("color");
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "plants") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string plant_name = lua_tostring(lua_state, -2);
                    int frequency = lua_tonumber(lua_state, -1);
                    b.plants.push_back(std::make_pair(plant_name, frequency));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "trees") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string tree_type = lua_tostring(lua_state, -2);
                    int frequency = lua_tonumber(lua_state, -1);
                    if (tree_type == "deciduous") b.deciduous_tree_chance = frequency;
                    if (tree_type == "evergreen") b.evergreen_tree_chance = frequency;
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "wildlife") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string critter = lua_tostring(lua_state, -1);
                    b.wildlife.push_back(critter);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }

        biome_defs.push_back(b);

        lua_pop(lua_state, 1);
    }
}

void read_species_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "species_sentient");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        raw_species_t s;
        std::string key = lua_tostring(lua_state, -2);
        s.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") s.name = lua_tostring(lua_state, -1);
            if (field == "male_name") s.male_name = lua_tostring(lua_state, -1);
            if (field == "female_name") s.female_name = lua_tostring(lua_state, -1);
            if (field == "group_name") s.collective_name = lua_tostring(lua_state, -1);
            if (field == "description") s.description = lua_tostring(lua_state, -1);
            if (field == "stat_mods") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    int value = lua_tonumber(lua_state, -1);
                    s.stat_mods.insert(std::make_pair(subfield, value));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "ethics") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    if (subfield == "diet") {
                        std::string diet_type = lua_tostring(lua_state, -1);
                        if (diet_type == "omnivore") s.diet = diet_omnivore;
                        if (diet_type == "herbivore") s.diet = diet_herbivore;
                        if (diet_type == "carnivore") s.diet = diet_carnivore;
                    }
                    if (subfield == "alignment") {
                        std::string alignment_type = lua_tostring(lua_state, -1);
                        if (alignment_type == "good") s.alignment = align_good;
                        if (alignment_type == "neutral") s.alignment = align_neutral;
                        if (alignment_type == "evil") s.alignment = align_evil;
                    }
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "parts") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string part_name = lua_tostring(lua_state, -2);
                    std::tuple<std::string, int, int> part;
                    std::get<0>(part) = part_name;
                    lua_pushstring(lua_state, part_name.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string part_field = lua_tostring(lua_state, -2);
                        if (part_field == "qty") std::get<1>(part) = lua_tonumber(lua_state, -1);
                        if (part_field == "size") std::get<2>(part) = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    s.body_parts.push_back(part);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "max_age") s.max_age = lua_tonumber(lua_state, -1);
            if (field == "infant_age") s.infant_age = lua_tonumber(lua_state, -1);
            if (field == "child_age") s.child_age = lua_tonumber(lua_state, -1);
            if (field == "glyph") s.glyph = lua_tonumber(lua_state, -1);

            lua_pop(lua_state, 1);
        }
        species_defs[key] = s;

        lua_pop(lua_state, 1);
    }
}

void read_creature_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "creatures");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        raw_creature_t s;
        std::string key = lua_tostring(lua_state, -2);
        s.tag = key;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") s.name = lua_tostring(lua_state, -1);
            if (field == "male_name") s.male_name = lua_tostring(lua_state, -1);
            if (field == "female_name") s.female_name = lua_tostring(lua_state, -1);
            if (field == "group_name") s.collective_name = lua_tostring(lua_state, -1);
            if (field == "description") s.description = lua_tostring(lua_state, -1);
            if (field == "stats") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string subfield = lua_tostring(lua_state, -2);
                    int value = lua_tonumber(lua_state, -1);
                    s.stats.insert(std::make_pair(subfield, value));
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "parts") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string part_name = lua_tostring(lua_state, -2);
                    std::tuple<std::string, int, int> part;
                    std::get<0>(part) = part_name;
                    lua_pushstring(lua_state, part_name.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string part_field = lua_tostring(lua_state, -2);
                        if (part_field == "qty") std::get<1>(part) = lua_tonumber(lua_state, -1);
                        if (part_field == "size") std::get<2>(part) = lua_tonumber(lua_state, -1);
                        lua_pop(lua_state, 1);
                    }
                    s.body_parts.push_back(part);

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "combat") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string cname = lua_tostring(lua_state, -2);
                    if (cname == "armor_class") s.armor_class = lua_tonumber(lua_state, -1);
                    if (cname == "attacks") {
                        lua_pushstring(lua_state, cname.c_str());
                        lua_gettable(lua_state, -2);
                        while (lua_next(lua_state, -2) != 0) {
                            std::string attack_name = lua_tostring(lua_state, -2);
                            lua_pushstring(lua_state, attack_name.c_str());
                            lua_gettable(lua_state, -2);
                            creature_attack_t attack;
                            while (lua_next(lua_state, -2) != 0) {
                                std::string attack_field = lua_tostring(lua_state, -2);
                                if (attack_field == "type") attack.type = lua_tostring(lua_state, -1);
                                if (attack_field == "hit_bonus") attack.hit_bonus = lua_tonumber(lua_state, -1);
                                if (attack_field == "n_dice") attack.damage_n_dice = lua_tonumber(lua_state, -1);
                                if (attack_field == "die_type") attack.damage_dice = lua_tonumber(lua_state, -1);
                                if (attack_field == "die_mod") attack.damage_mod = lua_tonumber(lua_state, -1);
                                lua_pop(lua_state, 1);
                            }
                            //std::cout << attack.type << attack_name << "\n";
                            s.attacks.push_back(attack);
                            lua_pop(lua_state, 1);
                        }
                    }

                    lua_pop(lua_state, 1);
                }
            }
            if (field == "hunting_yield") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string yield_type = lua_tostring(lua_state, -2);
                    int value = lua_tonumber(lua_state, -1);
                    if (yield_type == "meat") s.yield_meat = value;
                    if (yield_type == "hide") s.yield_hide = value;
                    if (yield_type == "bone") s.yield_bone = value;
                    if (yield_type == "skull") s.yield_skull = value;
                    lua_pop(lua_state, 1);
                }
            }
            if (field == "ai") {
                std::string ai_type = lua_tostring(lua_state, -1);
                if (ai_type == "grazer") s.ai = creature_grazer;
            }
            if (field == "glyph") s.glyph = lua_tonumber(lua_state, -1);
            if (field == "hp_n") s.hp_n = lua_tonumber(lua_state, -1);
            if (field == "hp_dice") s.hp_dice = lua_tonumber(lua_state, -1);
            if (field == "hp_mod") s.hp_mod = lua_tonumber(lua_state, -1);
            if (field == "group_size_n_dice") s.group_size_n_dice = lua_tonumber(lua_state, -1);
            if (field == "group_size_dice") s.group_size_dice = lua_tonumber(lua_state, -1);
            if (field == "group_size_mod") s.group_size_mod = lua_tonumber(lua_state, -1);
            if (field == "color") s.fg = read_lua_color("color");

            lua_pop(lua_state, 1);
        }
        creature_defs[key] = s;

        lua_pop(lua_state, 1);
    }
}

void read_native_population_types(std::ofstream &tech_tree_file) {
    lua_getglobal(lua_state, "native_populations");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            native_population_t p;

            // We need to iterate the array
            lua_pushnil(lua_state);
            lua_gettable(lua_state, -2);
            while (lua_next(lua_state, -2) != 0) {

                std::string field = lua_tostring(lua_state, -2);
                if (field == "title") {
                    p.name = lua_tostring(lua_state, -1);
                }
                if (field == "aggression") p.aggression = lua_tonumber(lua_state, -1);
                if (field == "melee") p.melee = lua_tostring(lua_state, -1);
                if (field == "ranged") p.ranged = lua_tostring(lua_state, -1);
                if (field == "ammo") p.ammo = lua_tostring(lua_state, -1);
                // Stat mods
                if (field == "modifiers") {
                    lua_pushstring(lua_state, field.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        std::string stat = lua_tostring(lua_state, -2);
                        int modifier = lua_tonumber(lua_state, -1);
                        if (stat == "str") p.strength = modifier;
                        if (stat == "dex") p.dexterity = modifier;
                        if (stat == "con") p.constitution = modifier;
                        if (stat == "int") p.intelligence = modifier;
                        if (stat == "wis") p.wisdom = modifier;
                        if (stat == "cha") p.charisma = modifier;
                        if (stat == "com") p.comeliness = modifier;
                        if (stat == "eth") p.ethics = modifier;
                        lua_pop(lua_state, 1);
                    }
                }

                // Starting clothes
                if (field == "clothing") {
                    lua_pushstring(lua_state, field.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string gender_specifier = lua_tostring(lua_state, -2);
                        lua_pushstring(lua_state, gender_specifier.c_str());
                        lua_gettable(lua_state, -2);
                        while (lua_next(lua_state, -2) != 0) {
                            const std::string slot = lua_tostring(lua_state, -2);
                            const std::string item = lua_tostring(lua_state, -1);
                            int gender_tag = 0;
                            if (gender_specifier == "male") gender_tag = 1;
                            if (gender_specifier == "female") gender_tag = 2;
                            p.starting_clothes.push_back( std::make_tuple(gender_tag, slot, item));
                            lua_pop(lua_state, 1);
                        }
                        lua_pop(lua_state, 1);
                    }
                }

                lua_pop(lua_state, 1);
            }
            //std::cout << key << ":" << p.name << "\n"; 
            auto finder = native_pop_defs.find(key);
            if (finder == native_pop_defs.end()) {
                native_pop_defs[key] = { p };
            } else {
                finder->second.push_back(p);
            }
            lua_pop(lua_state, 1);
        }
        lua_pop(lua_state, 1);
    }
}

void load_game_tables() {
    std::ofstream tech_tree_file("tech_tree.gv");
    tech_tree_file << "digraph G {\n";
    tech_tree_file << "\"cut trees\" -> wood_logs\n";

    read_clothing(tech_tree_file);
    read_professions(tech_tree_file);
    read_items(tech_tree_file);
    read_buildings(tech_tree_file);
    read_reactions(tech_tree_file);
    read_material_types(tech_tree_file);
    read_plant_types(tech_tree_file);
    read_biome_types(tech_tree_file);
    read_species_types(tech_tree_file);
    read_creature_types(tech_tree_file);
    read_native_population_types(tech_tree_file);

    tech_tree_file << "}\n";
    tech_tree_file.close();
}

void load_raws() {
	// Load string tables for first names and last names
	load_string_table("world_defs/first_names_male.txt", first_names_male);
	load_string_table("world_defs/first_names_female.txt", first_names_female);
	load_string_table("world_defs/last_names.txt", last_names);

	// Setup LUA
	lua_handle = std::make_unique<lua_lifecycle>();

	// Load game data via LUA
	string_table_t raw_index;
	load_string_table("world_defs/index.txt", raw_index);
	for (const std::string &filename : raw_index.strings) {
		load_lua_script("world_defs/" + filename);
	}

	// Extract game tables
	load_game_tables();

    // Quit LUA
    lua_handle.reset();
}

uint8_t get_tile_type_index(const std::string name) {
	auto finder = tile_type_index.find(name);
	if (finder != tile_type_index.end()) {
		return finder->second;
	} else {
		throw std::runtime_error("Unknown tile type: " + name);
	}
}

uint16_t get_tile_contents_index(const std::string name) {
    //std::cout << "Tile type: " << name;
	auto finder = tile_contents_index.find(name);
	if (finder != tile_contents_index.end()) {
        //std::cout << " = " << finder->second << "\n";
		return finder->second;
	} else {
		throw std::runtime_error("Unknown tile contents: " + name);
	}
}

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto item = create_entity()
        ->assign(position_t{ x,y,z })
        ->assign(renderable_t{ finder->second.glyph, finder->second.fg, finder->second.bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}

void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto item = create_entity()
        ->assign(item_stored_t{ container_id })
        ->assign(renderable_t{ finder->second.glyph, finder->second.fg, finder->second.bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}

void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc) {
    auto finder = item_defs.find(tag);
    if (finder == item_defs.end()) throw std::runtime_error(std::string("Unknown item tag: ") + tag);

    auto item = create_entity()
        ->assign(item_carried_t{ loc, holder_id })
        ->assign(renderable_t{ finder->second.glyph, finder->second.fg, finder->second.bg })
        ->assign(item_t{tag, finder->second.name, finder->second.categories, material, finder->second.stack_size});
}