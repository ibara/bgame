#include "game_globals.hpp"

config_t game_config;
planet_t planet;
std::unique_ptr<region_t> current_region = std::make_unique<region_t>();
std::size_t camera_entity;
world_position_t * camera_position;
calendar_t * calendar;
designations_t * designations;
int clip_left = -1;
int clip_right = -1;
int clip_top = -1;
int clip_bottom = -1;
rltk::random_number_generator rng;
pause_mode_t pause_mode = PAUSED;
game_master_mode_t game_master_mode = PLAY;
game_design_mode_t game_design_mode = DIGGING;
game_mining_mode_t game_mining_mode = DIG;
boost::optional<available_building_t> build_mode_building;
std::size_t selected_settler;
bool quitting = false;