#pragma once

#include <string>
#include <vector>
#include <rltk.hpp>

namespace string_tables {

    constexpr int FIRST_NAMES_MALE = 1;
    constexpr int FIRST_NAMES_FEMALE = 2;
    constexpr int LAST_NAMES = 3;
    constexpr int NEW_ARRIVAL_QUIPS = 4;
    constexpr int MENU_SUBTITLES = 5;

    struct string_table_t {
        std::vector<std::string> strings;

        inline std::string random_entry(rltk::random_number_generator rng = rltk::random_number_generator()) {
            const int position = rng.roll_dice(1, static_cast<int>(strings.size())) - 1;
            return strings[position];
        }
    };

    void load_string_table(const int index, const std::string filename);

    string_table_t *string_table(const int index);

}