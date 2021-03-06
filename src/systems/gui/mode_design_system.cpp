#include "mode_design_system.hpp"
#include "../input/mouse_input_system.hpp"
#include "../../messages/messages.hpp"
#include "../ai/inventory_system.hpp"
#include "../../components/stockpile.hpp"
#include "../../raws/plants.hpp"
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "imgui_helper.hpp"
#include "../../components/bridge.hpp"
#include "../../main/game_designations.hpp"
#include "../../main/game_selections.hpp"
#include "../../planet/region/region.hpp"
#include "../../main/game_mode.hpp"
#include "../../main/game_camera.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/defs/plant_t.hpp"
#include <vector>

using namespace rltk;
using namespace rltk::colors;
using namespace region;

std::vector<available_building_t> available_buildings;
const color_t GREEN_BG{0,32,0};

int architecture_mode = 0;
int arch_width = 1;
int arch_height = 1;
bool arch_filled = true;
bool arch_available = false;
bool arch_possible = true;
int arch_x = 0;
int arch_y = 0;
bool flags_debug = false;

void mode_design_system::configure() {
    system_name = "Design Mode";
    subscribe<refresh_available_buildings_message>([this] (refresh_available_buildings_message &msg) {
		available_buildings = get_available_buildings();
	});
}

inline bool is_mining_designation_valid(const int &x, const int &y, const int &z, const game_mining_mode_t &mode) {
	return true;
}

void mode_design_system::digging() {
    // Keyboard handler
    if (is_window_focused()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) game_mining_mode = DIG;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) game_mining_mode = CHANNEL;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) game_mining_mode = RAMP;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) game_mining_mode = UP;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) game_mining_mode = DOWN;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) game_mining_mode = UPDOWN;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) game_mining_mode = MINING_DELETE;
    }

    if (get_mouse_button_state(rltk::button::LEFT)) {
        const auto idx = mapidx(mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z);
        if (is_mining_designation_valid(mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z, game_mining_mode)) {
            switch (game_mining_mode) {
                case DIG : designations->mining[idx] = 1; break;
                case CHANNEL : designations->mining[idx] = 2; break;
                case RAMP : designations->mining[idx] = 3; break;
                case UP : designations->mining[idx] = 4; break;
                case DOWN : designations->mining[idx] = 5; break;
                case UPDOWN : designations->mining[idx] = 6; break;
                case MINING_DELETE : designations->mining.erase(idx); break;
            }
            emit(map_dirty_message{});
        }
    }

    ImGui::Begin(win_mining.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    switch (game_mining_mode) {
        case DIG : ImGui::Text("Currently: DIGGING"); break;
        case CHANNEL : ImGui::Text("Currently: CHANNELING"); break;
        case RAMP : ImGui::Text("Currently: RAMPING"); break;
        case UP : ImGui::Text("Currently: Carving UP stairs"); break;
        case DOWN : ImGui::Text("Currently: Carving DOWN stairs"); break;
        case UPDOWN : ImGui::Text("Currently: Carving UP/DOWN stairs"); break;
        case MINING_DELETE : ImGui::Text("Currently: Removing designations"); break;
    }
    if (ImGui::Button("Dig (d)")) game_mining_mode = DIG;
    if (ImGui::Button("Channel (c)")) game_mining_mode = CHANNEL;
    if (ImGui::Button("Ramp (r)")) game_mining_mode = RAMP;
    if (ImGui::Button("Up Stair (u)")) game_mining_mode = UP;
    if (ImGui::Button("Down Stair (j)")) game_mining_mode = DOWN;
    if (ImGui::Button("Up/Down Stair (i)")) game_mining_mode = UPDOWN;
    if (ImGui::Button("Clear Designation (x)")) game_mining_mode = MINING_DELETE;
    ImGui::End();
}

void mode_design_system::building()
{
    std::vector<std::pair<std::string, std::string>> buildings;

    bool rendered_selected = false;
    for (const available_building_t &building : available_buildings) {
        if (has_build_mode_building && build_mode_building.tag == building.tag) rendered_selected = true;
            buildings.emplace_back(std::make_pair(building.tag, building.get_name()));
    }
    const char* building_listbox_items[buildings.size()];
    for (int i=0; i<buildings.size(); ++i) {
        building_listbox_items[i] = buildings[i].second.c_str();
    }

    ImGui::Begin(win_building.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::ListBox("", &selected_building, building_listbox_items, buildings.size(), 10);
    ImGui::End();

    if (!rendered_selected) {
        has_build_mode_building = false;
    }
    if (buildings.size() > 0 && selected_building < available_buildings.size()) {
        build_mode_building = available_buildings[selected_building];
        has_build_mode_building = true;
    }
}

int calc_required_blocks(const int &w, const int &h, const bool &filled) {
    if (!filled) {
        return w*h;
    } else {
        return (w * 2) + (h * 2) - 4;
    }
}

void mode_design_system::architecture() {
    const int available_blocks = blocks_available() - designations->architecture.size();
    const int required_blocks = calc_required_blocks(arch_width, arch_height, arch_filled);
    const bool materials_available = (required_blocks <= available_blocks);
    const std::string block_availability = std::string("Available building blocks: ") + std::to_string(available_blocks) +
                                           std::string(" (Required: ") + std::to_string(required_blocks) + std::string(")");

    ImGui::Begin(win_architect.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    if (materials_available) {
        ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "%s", block_availability.c_str());
    } else {
        ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, "%s", block_availability.c_str());
    }
    ImGui::Text("Left click to build, right click to clear tile");

    // Options for wall/floor/up/down/updown/ramp/bridge
    ImGui::RadioButton("Wall", &architecture_mode, 0); ImGui::SameLine();
    ImGui::RadioButton("Floor", &architecture_mode, 1); ImGui::SameLine();
    //ImGui::RadioButton("Up", &architecture_mode, 2); ImGui::SameLine();
    //ImGui::RadioButton("Down", &architecture_mode, 3);
    //ImGui::RadioButton("Up/Down", &architecture_mode, 4); ImGui::SameLine();
    ImGui::RadioButton("Ramp", &architecture_mode, 5); ImGui::SameLine();
    ImGui::RadioButton("Bridge", &architecture_mode, 6);

    // Size options
    if (architecture_mode==0 || architecture_mode==1 || architecture_mode==6) {
        ImGui::InputInt("Width", &arch_width);
        ImGui::InputInt("Height", &arch_height);
        if (architecture_mode != 6) {
            ImGui::Checkbox("Filled?", &arch_filled);
        }
    } else {
        arch_width = 1;
        arch_height = 1;
        arch_filled = true;
    }
    ImGui::End();

    // Pass through to render system
    arch_available = true; // We're always allowing this to enable future planning mode

    arch_x = mouse::mouse_world_x;
    arch_y = mouse::mouse_world_y;
    const int world_x = arch_x;
    const int world_y = arch_y;

    if (mouse::clicked && arch_possible) {
        // Build!
        std::size_t bridge_id = 0;
        if (architecture_mode == 6) {
            auto new_bridge = create_entity()->assign(bridge_t{});
            bridge_id = new_bridge->id;
        }

        for (int y=world_y; y<world_y+arch_height; ++y) {
            for (int x = world_x; x < world_x + arch_width; ++x) {
                if (arch_filled) {
                    const int idx = mapidx(x,y,camera_position->region_z);
                    designations->architecture[idx] = architecture_mode;
                    if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
                    emit(map_dirty_message{});
                    emit(architecture_changed_message{});
                } else {
                    bool interior = false;
                    if (x>world_x && x<world_x+arch_width && y>world_y && y<world_y+arch_height) interior = true;
                    if (x==world_x) interior=false;
                    if (x==world_x+arch_width-1) interior = false;
                    if (y==world_y) interior = false;
                    if (y==world_y+arch_height-1) interior = false;
                    if (!interior) {
                        const int idx = mapidx(x,y,camera_position->region_z);
                        designations->architecture[idx] = architecture_mode;
                        if (architecture_mode == 6) set_bridge_id(idx, bridge_id);
                        emit(map_dirty_message{});
                        emit(architecture_changed_message{});
                    }
                }
            }
        }
    }
    if (get_mouse_button_state(rltk::button::RIGHT)) {
        // Erase
        const int idx = mapidx(world_x, world_y, camera_position->region_z);
        auto finder = designations->architecture.find(idx);
        if (finder != designations->architecture.end()) {
            if (finder->second == 6) {
                // Bridge - remove all of it
                const std::size_t bid = bridge_id(idx);
                if (bid > 0) {
                    delete_bridge(bid);
                }
                for (auto it=designations->architecture.begin(); it!=designations->architecture.end(); ++it) {
                    if (it->second == 6 && bridge_id(it->first) == bid) {
                        designations->architecture.erase(it->first);
                    }
                }
            }
            designations->architecture.erase(idx);
            emit(map_dirty_message{});
            emit(architecture_changed_message{});
        }
    }

    arch_possible = true;
}

void mode_design_system::chopping() {
    //add_gui_element(std::make_unique<map_static_text>(5,4, "Tree cutting mode - click a tree to cut, right-click to clear designation."));
    ImGui::Begin(win_chopping.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Click a tree to cut it down, right-click to clear designation.");
    ImGui::End();

    const auto idx = mapidx(mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z);
    const auto tree_id = region::tree_id(idx);

    if (get_mouse_button_state(rltk::button::LEFT) && tree_id > 0) {
        // Naieve search for the base of the tree; this could be optimized a LOT
        int lowest_z = camera_position->region_z;
        const int stop_z = lowest_z-10;

        position_t tree_pos{mouse::mouse_world_x, mouse::mouse_world_y, lowest_z};
        while (lowest_z > stop_z) {
            for (int y=-10; y<10; ++y) {
                for (int x=-10; x<10; ++x) {
                    const int tree_idx = mapidx(mouse::mouse_world_x + x, mouse::mouse_world_y + y, lowest_z);
                    if (region::tree_id(tree_idx) == tree_id) {
                        tree_pos.x = mouse::mouse_world_x+x;
                        tree_pos.y = mouse::mouse_world_y+y;
                        tree_pos.z = lowest_z;
                    }
                }
            }
            --lowest_z;
        }

        designations->chopping[(int)tree_id] = tree_pos;
        emit(map_dirty_message{});
    } else if (get_mouse_button_state(rltk::button::RIGHT) && tree_id > 0) {
        designations->chopping.erase((int)tree_id);
        emit(map_dirty_message{});
    }
}

void mode_design_system::guardposts() {
    //add_gui_element(std::make_unique<map_static_text>(5,4, "Guard mode - click a square to guard it, right-click to clear."));
    ImGui::Begin(win_guard.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Click a tile to guard it, right click to remove guard status.");
    ImGui::End();

    const int world_x = mouse::mouse_world_x;
    const int world_y = mouse::mouse_world_y;
    const int world_z = mouse::mouse_world_z;

    const auto idx = mapidx(world_x, world_y, camera_position->region_z);
    if (flag(idx, CAN_STAND_HERE)) {
        if (get_mouse_button_state(rltk::button::LEFT)) {
            bool found = false;
            for (const auto &g : designations->guard_points) {
                if (mapidx(g.second) == idx) found = true;
            }
            if (!found) designations->guard_points.push_back(std::make_pair(false, position_t{world_x, world_y, world_z}));
        } else if (get_mouse_button_state(rltk::button::RIGHT)) {
            designations->guard_points.erase(std::remove_if(
                    designations->guard_points.begin(),
                    designations->guard_points.end(),
                    [&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
                ),
                designations->guard_points.end());
        }
    }
}

void mode_design_system::harvest() {

    std::string harvest_name = "";
    const int world_x = mouse::mouse_world_x;
    const int world_y = mouse::mouse_world_y;
    const int world_z = mouse::mouse_world_z;

    const auto idx = mapidx(world_x, world_y, camera_position->region_z);
    bool ok = true;
    if (veg_type(idx)==0) ok = false;
    if (ok) {
        auto p = get_plant_def(veg_type(idx));
        const std::string harvests_to = p->provides[veg_lifecycle(idx)];
        if (harvests_to == "none") {
            ok=false;
        } else {
            auto finder = get_item_def(harvests_to);
            if (finder != nullptr) {
                harvest_name = finder->name;
            }
        }
    }

    if (ok && flag(idx, CAN_STAND_HERE)) {
        if (get_mouse_button_state(rltk::button::LEFT)) {
            bool found = false;
            for (const auto &g : designations->harvest) {
                if (mapidx(g.second) == idx) found = true;
            }
            if (!found) {
                designations->harvest.push_back(std::make_pair(false, position_t{world_x, world_y, world_z}));
                emit_deferred(harvestmap_changed_message{});
            }
        } else if (get_mouse_button_state(rltk::button::RIGHT)) {
            designations->harvest.erase(std::remove_if(
                    designations->harvest.begin(),
                    designations->harvest.end(),
                    [&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
                                             ),
                                             designations->harvest.end());
            emit_deferred(harvestmap_changed_message{});
        }
    }

    ImGui::Begin(win_harvest.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::TextColored(ImVec4{1.0f, 1.0f, 0.0f, 1.0f}, "%s", "Click a tile to harvest it, right click to remove harvest status.");
    if (!harvest_name.empty()) {
        ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "%s", "Current tile will provide: ");
        ImGui::SameLine();
        ImGui::Text("%s", harvest_name.c_str());
    }
    ImGui::End();

}

void mode_design_system::stockpiles() {
    std::vector<std::pair<std::size_t, std::string>> stockpiles;
    each<stockpile_t>([&stockpiles] (entity_t &e, stockpile_t &sp) {
        stockpiles.emplace_back(std::make_pair( e.id, std::string("Stockpile #" + std::to_string(e.id)) ));
    });
    const char* stockpile_listbox_items[stockpiles.size()];
    for (int i=0; i<stockpiles.size(); ++i) {
        stockpile_listbox_items[i] = stockpiles[i].second.c_str();
        if (i == selected_stockpile) current_stockpile = stockpiles[i].first;
    }

    // Stockpiles list
    ImGui::Begin(win_stockpiles.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::TextWrapped("Stockpiles - select a stockpile from the right panel, click to add, right click to remove.");
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("## Stockpiles", &selected_stockpile, stockpile_listbox_items, stockpiles.size(), 10);
    if (ImGui::Button("+ Add Stockpile")) {
        auto sp = create_entity()->assign(stockpile_t{});
        current_stockpile = sp->id;
    }
    ImGui::SameLine();
    if (ImGui::Button("- Remove Stockpile")) {
        delete_entity(current_stockpile);
        delete_stockpile(current_stockpile);
        current_stockpile = 0;
        selected_stockpile = 0;
    }
    if (current_stockpile != 0) {
        auto sp_entity = entity(current_stockpile);
        if (sp_entity) {
            auto sp = sp_entity->component<stockpile_t>();
            if (sp) {
                each_stockpile([&sp] (stockpile_def_t * it) {
                    if (sp->category.test(it->index)) {
                        const std::string sp_label = std::string(ICON_FA_CHECK) + std::string(" ") + it->name;
                        if (ImGui::Button(sp_label.c_str())) {
                            sp->category.reset(it->index);
                        }
                    } else {
                        const std::string sp_label = std::string(ICON_FA_TIMES) + std::string(" ") + it->name;
                        if (ImGui::Button(sp_label.c_str())) {
                            sp->category.set(it->index);
                        }
                    }
                });
            }
        }
    }
    ImGui::End();

    if (current_stockpile>0) {

        const auto idx = mapidx(mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z);
        if (flag(idx, CAN_STAND_HERE)) {
            if (get_mouse_button_state(rltk::button::LEFT)) {
                if (stockpile_id(idx)==0) {
                    set_stockpile_id(idx, current_stockpile);
                    calc_render(idx);
                    emit(map_dirty_message{});
                } else {
                    current_stockpile = stockpile_id(idx);
                }
            } else if (get_mouse_button_state(rltk::button::RIGHT)) {
                set_stockpile_id(idx, 0);
                calc_render(idx);
                emit(map_dirty_message{});
            }
        }
    }
}

void mode_design_system::update(const double duration_ms) {
    if (game_master_mode != DESIGN) return;

    switch (game_design_mode) {
        case DIGGING    : digging(); break;
        case BUILDING   : building(); break;
        case CHOPPING   : chopping(); break;
        case GUARDPOINTS: guardposts(); break;
        case STOCKPILES : stockpiles(); break;
        case HARVEST : harvest(); break;
        case ARCHITECTURE : architecture(); break;
    }
}
