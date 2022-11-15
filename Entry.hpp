#ifndef HANJIE_ENTRY_HPP
#define HANJIE_ENTRY_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#define ADD_HANDLE(handle) handle.push_back(this)
#define REMOVE_HANDLE(handle) handle.erase(std::find(handle.begin(), handle.end(), this))


struct Entry{
    virtual void draw(sf::RenderWindow& window) final;
    virtual void handle(const sf::Event &event, sf::RenderWindow &window);

    [[maybe_unused]] virtual void handle(const sf::Time &time);

    virtual void hide() final;
    virtual void show() final;

    inline static std::vector<Entry*> mouseButtonReleased; //TODO try make add of compile time
    inline static std::vector<Entry*> mouseMoved;
    inline static std::vector<Entry*> textEntered;
    inline static std::vector<Entry*> drawable;
    inline static std::vector<Entry*> deltaTime;

private:
    virtual void m_draw(sf::RenderWindow& window);
    virtual void m_handle(const sf::Event &event, sf::RenderWindow &window);
    virtual void m_handle(const sf::Time &time);

protected:
    bool m_hide{false};
};

#endif //HANJIE_ENTRY_HPP
