#include "Sprite.hpp"


Sprite::Sprite(const sf::Texture &texture)
    : m_sprite{texture}
{ ADD_HANDLE(drawable); }

Sprite::~Sprite()
{ REMOVE_HANDLE(drawable); }

void Sprite::m_draw(sf::RenderWindow &window)
{ window.draw(m_sprite); }
