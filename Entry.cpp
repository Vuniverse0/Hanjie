#include "Entry.hpp"

#include <cassert>


void Entry::draw(sf::RenderWindow &window)
{
    if(!m_hide)
        m_draw(window);
}

void Entry::handle(const sf::Event &event, sf::RenderWindow &window)
{
    if(!m_hide)
        m_handle(event, window);
}

[[maybe_unused]] void Entry::handle(const sf::Time &time)
{
    if(!m_hide)
        m_handle(time);
}

void Entry::m_draw(sf::RenderWindow &window)
{ assert(false); }

void Entry::hide()
{ m_hide = true; }

void Entry::show()
{ m_hide = false; }

void Entry::m_handle(const sf::Event &event, sf::RenderWindow &window)
{ assert(false); }

void Entry::m_handle(const sf::Time &time)
{ assert(false); }
