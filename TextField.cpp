#include "TextField.hpp"

#include <cassert>

#include "Resources.hpp"

#include "SFML/Window/Event.hpp"


TextField::TextField(const std::string& string)
    : m_playerInput(string)
    , m_playerText{" ", Resources::resources.font}
{
    m_lock = false;
    m_playerText.setString(m_playerInput);
    m_playerText.setCharacterSize(30);
    m_playerText.setFillColor(sf::Color::Black);
    //m_playerText.setOrigin(
      //      m_playerText.getGlobalBounds().width / 2,
        //    m_playerText.getGlobalBounds().height / 2
          //  );
    ADD_HANDLE(drawable);
    ADD_HANDLE(textEntered);
}

TextField::TextField()
    : TextField("null")
{}

TextField::~TextField()
{
    REMOVE_HANDLE(drawable);
    REMOVE_HANDLE(textEntered);
}

void TextField::m_draw(sf::RenderWindow &window)
{ window.draw(m_playerText); }

void TextField::m_handle(const sf::Event &event, sf::RenderWindow &window)
{
    assert(event.type == sf::Event::TextEntered);
    if(m_lock)
        return;

    if(event.text.unicode == 8 && !m_result.empty()){
        m_result.pop_back();
        m_playerInput.erase(m_playerInput.getSize() - 1);
    }else if(event.text.unicode < 128 && event.text.unicode != 13) {
        m_playerInput += event.text.unicode;
        m_result.push_back(static_cast<char>(event.text.unicode));
    }else{
        return;
    }
    m_playerText.setString(m_playerInput);
}

TextField& TextField::setTitle(const std::string &string)
{
    clear();
    m_playerInput = string;
    m_playerText.setString(m_playerInput);
    return *this;
}

const std::string& TextField::getString()
{ return m_result; }

void TextField::clear()
{
    m_playerInput.erase(m_playerInput.getSize() - m_result.size(), m_result.size());
    m_result.clear();
    m_playerText.setString(m_playerInput);
}

sf::Transformable &TextField::transform()
{ return m_playerText; }

TextField& TextField::lock()
{
    m_lock = true;
    return *this;
}
