#pragma once

#include <rltk.hpp>
#include "name.hpp"

struct log_line_t {
    log_line_t() {}
    log_line_t(std::vector<rltk::vchar> new_chars) : chars(new_chars) {}

    int age = 0;
    std::vector<rltk::vchar> chars;

    void save(std::ostream &f);
    void load(std::istream &f);
};

struct logger_t {
	logger_t() {}
    std::vector<log_line_t> lines;

	std::size_t serialization_identity = 26;

	void save(std::ostream &lbfile);
	static logger_t load(std::istream &lbfile);
};

struct LOG {
    std::vector<rltk::vchar> chars;
    rltk::color_t fg = rltk::colors::WHITE;

    LOG * text(const std::string &txt);
    inline LOG * col(const rltk::color_t &col) { fg = col; return this; }
    LOG * settler_name(const std::size_t &entity_id);
    LOG * settler_name_t(const name_t &name);
    LOG * sentient_name(const std::size_t &entity_id);
    LOG * other_name(const std::size_t &entity_id);
    LOG * civ_name(const std::size_t &civ_id);
};