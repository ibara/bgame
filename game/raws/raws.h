#pragma once

#include <string>

using std::string;

namespace engine {
namespace raws {

/*
 * Load the database defining the game, from the "../raws/" folder. Raw files to
 * read are listed in "index".
 */
void read_raws();

/*
 * Create an entity and related components using definitions in the raw files.
 * Returns the entity handle.
 */
int create_structure_from_raws( const string& name, const int& x, const int& y );

string get_random_starting_profession();
string get_random_first_name(const short &gender);
string get_random_last_name();

}
}