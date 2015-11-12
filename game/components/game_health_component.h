#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct game_health_component {
     game_health_component() {}

     int entity_id;
     int handle;
     component_type type = gamehealth;
     bool deleted = false;
     
     int max_hit_points;
     int current_hit_points;
     
     void save ( fstream &lbfile ) {
          save_common_component_properties<game_health_component> ( lbfile, *this );
	  save_primitive<int>(lbfile, max_hit_points);
	  save_primitive<int>(lbfile, current_hit_points);
     }

     void load  ( fstream &lbfile ) {
          load_common_component_properties<game_health_component> ( lbfile, *this );
	  load_primitive<int>(lbfile, max_hit_points);
	  load_primitive<int>(lbfile, current_hit_points);
     }
};
