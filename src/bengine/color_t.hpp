#pragma once

namespace bengine {
    struct color_t {
        color_t() {}
        color_t(const float &R, const float &G, const float &B) : r(R), g(G), b(B) {}
        color_t(const uint8_t &R, const uint8_t &G, const uint8_t &B) : r(static_cast<float>(R) / 255.0f),
                                                                        g(static_cast<float>(G) / 255.0f),
                                                                        b(static_cast<float>(B) / 255.0f) {}

        float r=0.0;
        float g=0.0;
        float b=0.0;
    };
}