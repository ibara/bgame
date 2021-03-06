#include "systems.hpp"

#include "render/map_render_system.hpp"
#include "scheduler/calendar_system.hpp"
#include "ai/settler_ai_system.hpp"
#include "gui/panel_render_system.hpp"
#include "ai/mining_system.hpp"
#include "ai/inventory_system.hpp"
#include "ai/visibility_system.hpp"
#include "ai/workflow_system.hpp"
#include "physics/topology_system.hpp"
#include "render/camera_system.hpp"
#include "render/renderables_system.hpp"
#include "render/lighting_system.hpp"
#include "power_system.hpp"
#include "damage/damage_system.hpp"
#include "ai/wildlife_population_system.hpp"
#include "ai/movement_system.hpp"
#include "gui/mode_units_system.hpp"
#include "ai/mode_rogue_system.hpp"
#include "gui/mode_rogue_render_system.hpp"
#include "scheduler/corpse_system.hpp"
#include "gui/particle_system.hpp"
#include "physics/vegetation_system.hpp"
#include "input/keyboard_system.hpp"
#include "ai/sentient_ai_system.hpp"
#include "world_system.hpp"
#include "gui/game_over_system.hpp"
#include "gui/mode_civs_system.hpp"
#include "gui/log_system.hpp"
#include "input/mouse_input_system.hpp"
#include "gui/hud_system.hpp"
#include "gui/mode_settler_info_system.hpp"
#include "gui/mode_play_system.hpp"
#include "gui/mode_design_system.hpp"
#include "gui/mode_standing_orders.hpp"
#include "gui/mode_sentient_info_system.hpp"
#include "physics/fluid_system.hpp"
#include "physics/gravity_system.hpp"
#include "gui/wish_system.hpp"
#include "physics/door_system.hpp"
#include "scheduler/pause_system.hpp"
#include "damage/settler_ranged_attack.hpp"
#include "damage/settler_melee_attacks.hpp"
#include "damage/creature_attacks.hpp"
#include "damage/kill_system.hpp"
#include "damage/healing_system.hpp"
#include "scheduler/initiative_system.hpp"
#include "stockpile_system.hpp"
#include "ai/distance_map_system.hpp"
#include "ai/settler_spawner_system.hpp"
#include "physics/trigger_system.hpp"
#include "ai/settler/ai_status_effects.hpp"
#include "ai/settler/ai_new_arrival.hpp"
#include "ai/settler/ai_action_shim.hpp"
#include "ai/settler/ai_visibility_scan.hpp"
#include "ai/settler/ai_idle.hpp"
#include "damage/sentient_attacks.hpp"
#include "ai/settler/ai_stuck.hpp"
#include "ai/settler/ai_scheduler.hpp"
#include "ai/settler/ai_leisure_time.hpp"
#include "ai/settler/ai_sleep_time_system.hpp"
#include "ai/settler/ai_work_time.hpp"
#include "ai/settler/ai_work_lumberjack.hpp"
#include "ai/settler/ai_work_mining.hpp"
#include "ai/settler/ai_work_guard.hpp"
#include "ai/settler/ai_work_harvest.hpp"
#include "ai/settler/ai_work_lever_pull.hpp"
#include "ai/settler/ai_work_building.hpp"
#include "ai/settler/ai_work_order.hpp"
#include "ai/settler/ai_work_architect.hpp"
#include "ai/settler/ai_work_hunt.hpp"
#include "ai/settler/ai_work_butcher.hpp"
#include "physics/explosive_system.hpp"
#include "damage/turret_ranged_attack.hpp"

void add_systems_to_ecs() {
    add_system<fluid_system>();
    add_system<explosive_system>();
    add_system<keyboard_system>();
    add_system<mouse_input_system>();
    add_system<door_system>();
    add_system<pause_system>();
    add_system<calendar_system>();
    add_system<initiative_system>();
    add_system<ai_status_effects>();
    add_system<ai_stuck_system>();
    add_system<world_system>();
    add_system<settler_spawner_system>();
    add_system<gravity_system>();
    add_system<distance_map_system>();
    add_system<wildlife_population_system>();
    add_system<sentient_ai_system>();
    add_system<corpse_system>();
    add_system<particle_system>();
    add_system<camera_system>();
    add_system<lighting_system>();
    add_system<mining_system>();
    add_system<inventory_system>();
    add_system<stockpile_system>();
    add_system<power_system>();
    add_system<workflow_system>();
    add_system<ai_visibility_scan>();
    add_system<ai_new_arrival>();
    add_system<ai_scheduler>();
    add_system<ai_leisure_time_t>();
    add_system<ai_sleep_time_system>();
    add_system<ai_work_time>();
    add_system<ai_work_lumberjack>();
    add_system<ai_work_mining>();
    add_system<ai_work_guard>();
    add_system<ai_work_harvest>();
    add_system<ai_work_lever_pull>();
    add_system<ai_work_building>();
    add_system<ai_work_order>();
    add_system<ai_work_architect>();
    add_system<ai_work_hunt>();
    add_system<ai_work_butcher>();
    add_system<ai_action_shim>();
    add_system<ai_idle>();
    add_system<settler_ai_system>();
    add_system<mode_rogue_system>();
    add_system<movement_system>();
    add_system<trigger_system>();
    add_system<settler_ranged_attack_system>();
    add_system<settler_melee_attacks_system>();
    add_system<creature_attacks_system>();
    add_system<sentient_attacks_system>();
    add_system<turret_ranged_attack_system>();
    add_system<damage_system>();
    add_system<kill_system>();
    add_system<healing_system>();
    add_system<topology_system>();
    add_system<visibility_system>();
    add_system<vegetation_system>();
    add_system<renderables_system>();
    add_system<map_render_system>();
    add_system<hud_system>();
    add_system<panel_render_system>();
    add_system<mode_play_system>();
    add_system<mode_design_system>();
    add_system<mode_units_system>();
    add_system<mode_rogue_render_system>();
    add_system<mode_settler_info_system>();
    add_system<game_over_system>();
    add_system<mode_civs_system>();
    add_system<mode_standing_orders>();
    add_system<mode_sentient_info_system>();
    add_system<wish_system>();
    add_system<log_system>();
}
