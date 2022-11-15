#ifndef HANJIE_GAMEBOARD_HPP
#define HANJIE_GAMEBOARD_HPP

#include <cstdint>
#include <string>
#include <memory>
#include <cmath>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entry.hpp"


struct GameBoard : Entry {
    using size = std::uint32_t;

    GameBoard();
    GameBoard(sf::RenderWindow &window, size x, size y);
    explicit GameBoard(sf::RenderWindow &window, const std::string &file_name);
    GameBoard& operator=(GameBoard&& board) = default;
    ~GameBoard();

    enum class Mode{Error, Create, Play, Tutorial, Size};
    static Mode mode;

    void save(const std::string &file_name);
    bool isWin();
    sf::Vector2u getSize() const;
    void highlightAt(size a_x, size a_y, sf::RenderWindow &window, bool fill = true);

private:
    struct Cell{
        struct Draw{
            enum class Status {
                Error, Empty, Fill, Cross, Number, Size
            } paint, status;
        };
        using Status = GameBoard::Cell::Draw::Status;

        /// number: paint - fill, status - number
        /// empty number: paint - empty, status - number
        /// paint - cross | fill | empty, status - cross | fill;
        explicit Cell(size n);
        Cell();

        ///click
        bool click(bool right_click);

        size number{0};
        Draw draw{};
    };

    void m_draw(sf::RenderWindow &window) override;
    void m_handle(const sf::Event &event, sf::RenderWindow &window) override;
    void updateNumbers(size x, size y);
    void init();
    void addHandles();
    Cell& get(size x, size y);

    bool m_updated{ true };
    sf::Vector2u m_board_size, m_numbers_size, m_size;
    std::float_t m_a{0.f};
    sf::RectangleShape line_x{}, line_y{}, line_x_s{}, line_y_s{}, square{}, highlight_square{};
    sf::Texture texture{};
    sf::Sprite cross{};
    sf::Text m_text;
    std::unique_ptr<Cell[]> m_board;
};

#endif //HANJIE_GAMEBOARD_HPP
