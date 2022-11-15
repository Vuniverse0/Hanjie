#ifndef HANJIE_TEXTFIELD_HPP
#define HANJIE_TEXTFIELD_HPP

#include "Entry.hpp"

#include <SFML/Graphics/Text.hpp>


struct TextField : Entry {
    TextField();
    explicit TextField(const std::string& string);
    ~TextField();

    ///also call clear
    TextField& setTitle(const std::string& string);
    ///return input string, don`t clear anything
    const std::string& getString();
    sf::Transformable& transform();
    void clear();
    TextField& lock();
private:
    void m_handle(const sf::Event &event, sf::RenderWindow &window) override;
    void m_draw(sf::RenderWindow &window) override;

    bool m_lock {false};
    std::string m_result{};
    sf::String m_playerInput{};
    sf::Text m_playerText;
};


#endif //HANJIE_TEXTFIELD_HPP
