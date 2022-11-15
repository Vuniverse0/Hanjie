#ifndef HANJIE_SPRITE_HPP
#define HANJIE_SPRITE_HPP

#include "Entry.hpp"

#include <SFML/Graphics/Sprite.hpp>


struct Sprite : Entry {
    explicit Sprite(const sf::Texture &texture);
    ~Sprite();
private:
    void m_draw(sf::RenderWindow &window) override;

    sf::Sprite m_sprite;
};


#endif //HANJIE_SPRITE_HPP
