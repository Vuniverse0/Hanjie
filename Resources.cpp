#include "Resources.hpp"

extern const std::uint8_t OpenSans_Regular_ttf[];
extern const std::uint8_t cross_png[];
extern const std::uint8_t Rules_png[];

Resources Resources::resources{};

Resources::Resources()
{
    font.loadFromMemory(OpenSans_Regular_ttf, 217360);
    cross_texture.loadFromMemory(cross_png, 857);
    rules_texture.loadFromMemory(Rules_png, 96206);
    cross_texture.setSmooth(true);
    rules_texture.setSmooth(true);
}