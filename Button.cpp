#include "Button.hpp"

#include <cassert>

#include<SFML/Window/Event.hpp>
#include <utility>

#include "Resources.hpp"


Button::Button(const char *text, sf::Vector2i cords, std::function<void()> function)
    : m_text{text, Resources::resources.font}
    , m_callback{std::move(function)}
{
    mouseButtonReleased.push_back(this);
    drawable.push_back(this);

    m_text.setCharacterSize(40);
    m_text.setFillColor(sf::Color::Black);
    m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2);
    m_text.setPosition(static_cast<std::float_t>(cords.x), static_cast<std::float_t>(cords.y - 5));

    rectangle.setOutlineThickness(3);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);
    rectangle.setPosition(static_cast<std::float_t>(cords.x), static_cast<std::float_t>(cords.y));
}

void Button::m_draw(sf::RenderWindow& window)
{
    window.draw(rectangle);
    window.draw(m_text);
}

void Button::handle(const sf::Event &event, sf::RenderWindow &window)
{
    if(Entry::m_hide)
        return;

    assert(event.type == sf::Event::EventType::MouseButtonReleased);
    if(rectangle.getGlobalBounds().contains(static_cast<std::float_t>(event.mouseButton.x),
                                            static_cast<std::float_t>(event.mouseButton.y))){
        if (m_callback)
            m_callback();
        else
            std::cerr << __PRETTY_FUNCTION__ << "-> no callback: " << this << std::endl;
    }
}
