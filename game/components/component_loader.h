#pragma once

#include "components.h"
#include "../game.h"

template<typename T>
void component_loader(fstream &lbfile) {
  T component;
  component.load(lbfile);
  const int entity_handle = component.entity_id;
  entity * parent = game_engine->ecs->get_entity_by_handle ( entity_handle );
  if (parent == nullptr) {
    std::cout << "Error! Entity " << component.entity_id << " is null.\n";
  } else {
    game_engine->ecs->add_component<T>(*parent, component);
  }
}

inline void component_factory(fstream &lbfile, const int ct) {
  switch ( ct ) {
      case position : component_loader<position_component>(lbfile); break;
      case name : component_loader<debug_name_component>(lbfile); break;
      case renderable : component_loader<renderable_component>(lbfile); break;
      case viewshed :  component_loader<viewshed_component>(lbfile); break;
      case calendar : component_loader<calendar_component>(lbfile); break;
      case settler_ai : component_loader<settler_ai_component>(lbfile); break;
      case obstruction : component_loader<obstruction_component>(lbfile); break;
      case power_generator : component_loader<power_generator_component>(lbfile); break;
      case power_battery : component_loader<power_battery_component>(lbfile); break;
      case gamestats : component_loader<game_stats_component>(lbfile); break;
      case gamespecies : component_loader<game_species_component>(lbfile); break;
      case gamehealth : component_loader<game_health_component>(lbfile); break;
      case provision : component_loader<provisions_component>(lbfile); break;
      case item_storage : component_loader<item_storage_component>(lbfile); break;
      case item_carried : component_loader<item_carried_component>(lbfile); break;
      default :
	    throw 102;
  }
}