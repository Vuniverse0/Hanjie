#ifndef HANJIE_RESOURCES_HPP
#define HANJIE_RESOURCES_HPP

#include <SFML/Graphics/Font.hpp>


struct Resources {
    static Resources resources;

    sf::Font font;
    sf::Texture cross_texture;
    sf::Texture rules_texture;

private:
    Resources();
};


#endif //HANJIE_RESOURCES_HPP
