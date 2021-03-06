#pragma once

#include <rltk.hpp>
#include <sstream>
#include <iomanip>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/utility.hpp>

using namespace rltk;

enum gender_t { MALE, FEMALE };
enum sexuality_t { HETEROSEXUAL, HOMOSEXUAL, BISEXUAL };
enum hair_color_t { WHITE_HAIR, BROWN_HAIR, BLACK_HAIR, BLONDE_HAIR, RED_HAIR };
enum hair_style_t { BALD, SHORT_HAIR, LONG_HAIR, PIGTAILS, MOHAWK, BALDING, TRIANGLE };

struct species_t {
	std::string tag = "";
	std::size_t index = 0;
	gender_t gender;
	sexuality_t sexuality;
	hair_style_t hair_style;
	std::pair<std::string, color_t> skin_color;
	std::pair<std::string, color_t> hair_color;
	float height_cm;
	float weight_kg;
	bool bearded;
    uint16_t base_male_glyph;
    uint16_t base_female_glyph;

	species_t() {}

	std::string gender_str();
	std::string gender_pronoun();
	std::string sexuality_str();
	std::string height_feet();
	std::string weight_lbs();
	std::string ethnicity();
	std::string hair_color_str();
	std::string hair_style_str();

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( tag, gender, sexuality, hair_color, hair_style, skin_color, height_cm, weight_kg, bearded, index, base_male_glyph, base_female_glyph ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<species_t>>)
