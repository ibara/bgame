#include "renderable.hpp"
#include "../utils/serialization_wrapper.hpp"

void renderable_t::save(std::ostream &lbfile) {
    Serialize(lbfile, glyph, foreground, background);
}

renderable_t renderable_t::load(std::istream &lbfile) {
    renderable_t c;
    Deserialize(lbfile, c.glyph, c.foreground, c.background);
    return c;
}