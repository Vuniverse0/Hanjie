#ifndef HANJIE_BUTTON_HPP
#define HANJIE_BUTTON_HPP

#include <cmath>
#include <iostream>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Entry.hpp"


struct Button : Entry{
    Button(const char* text, sf::Vector2i cords, std::function<void()> function);

    void handle(const sf::Event &event, sf::RenderWindow &window) override;

private:
    void m_draw(sf::RenderWindow& window) override;

    sf::RectangleShape rectangle{sf::Vector2f(300, 50)};
    sf::Text m_text;
    std::function<void()> m_callback;
};

#endif //HANJIE_BUTTON_HPP
