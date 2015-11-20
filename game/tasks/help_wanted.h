#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using std::fstream;
using std::vector;
using std::string;
using std::unordered_map;

namespace ai {

enum job_type_e { BUILD_STRUCTURE };
enum job_step_e { MOVE_TO, PICK_UP_COMPONENT, DROP_OFF_COMPONENT, CONSTRUCT_WITH_SKILL, CONVERT_PLACEHOLDER_STRUCTURE, DESTROY_COMPONENT };

int get_next_job_id();

struct job_step_t {
     job_step_e type;
     int target_x = 0;
     int target_y = 0;
     int component_id = 0;
     bool requires_skill = false;
     string skill_name = "";
     int placeholder_structure_id;
};

struct job_t {
     job_type_e type;
     int job_id = 0;
     vector<job_step_t> steps;
     int current_step = 0;
     int assigned_to = 0;
};

extern unordered_map<int, job_t> jobs_board;

void save_help_wanted ( fstream& lbfile );
void load_help_wanted ( fstream& lbfile );
}