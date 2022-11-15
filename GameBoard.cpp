#include "GameBoard.hpp"

#include <cassert>
#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Resources.hpp"
#include "Settings.hpp"


GameBoard::Mode GameBoard::mode{Mode::Error};

GameBoard::GameBoard(sf::RenderWindow &window, GameBoard::size x, GameBoard::size y)
    : m_board_size{x, y}
    , m_numbers_size{x / 10 * 5 == 0 ? 5 : x / 10 * 5, y / 10 * 5 == 0 ? 5 : y / 10 * 5}
    , m_size{x + m_numbers_size.x, y + m_numbers_size.y}
    , m_a{(window.setSize({x * WINDOW_SIZE, y * WINDOW_SIZE}),
           window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / static_cast<std::float_t>(x))}
    , m_board{ new Cell[m_size.x * m_size.y] }
    , line_x{sf::Vector2f(m_a * static_cast<float>(m_size.x), LINE_THICKNESS)}
    , line_y{sf::Vector2f(m_a * static_cast<float>(m_size.y), LINE_THICKNESS)}
    , line_x_s{sf::Vector2f(m_a * static_cast<float>(m_size.x), MARKUP_LINE_THICKNESS)}
    , line_y_s{sf::Vector2f(m_a * static_cast<float>(m_size.y), MARKUP_LINE_THICKNESS)}
    , square(sf::Vector2f{m_a, m_a})
    , m_text{" ", Resources::resources.font}
{
    addHandles();
    assert(x % 5 == 0), assert(x >= 5);
    assert(y % 5 == 0), assert(y >= 5);
    assert(m_numbers_size.x % 5 == 0), assert(m_numbers_size.x >= 5);
    assert(m_numbers_size.y % 5 == 0), assert(m_numbers_size.y >= 5);

    init();

    for(std::uint32_t i{0}; i < m_numbers_size.x; ++i)
        for(std::uint32_t j{0}; j < m_size.y; ++j)
            get(i, j) = Cell(0);

    for(std::uint32_t j{0}; j < m_numbers_size.y; ++j)
        for(std::uint32_t i{0}; i < m_size.x; ++i)
            get(i, j) = Cell(0);
}

GameBoard::GameBoard(sf::RenderWindow &window, const std::string &file_name)
{
    addHandles();
    FILE *file = fopen(file_name.c_str(), "rb");
    size buffer[2];
    fread(&buffer, 1, sizeof(buffer), file);
    m_board_size = {buffer[0], buffer[1]};

    *this = GameBoard(window, m_board_size.x, m_board_size.y);

    fread(m_board.get(), sizeof(Cell), m_size.x * m_size.y, file);
    fclose(file);

    if(mode == Mode::Create){
        for(std::uint32_t i{m_numbers_size.x}; i < m_size.x; ++i){
            for(std::uint32_t j{m_numbers_size.y}; j < m_size.y; ++j){
                get(i,j).draw.paint = get(i,j).draw.status;
            }
        }
    }
}

GameBoard::GameBoard()
{
    addHandles();
    m_hide = true;
}

GameBoard::~GameBoard()
{
    REMOVE_HANDLE(drawable);
    //REMOVE_HANDLE(mouseMoved);
    REMOVE_HANDLE(mouseButtonReleased);
}

void GameBoard::save(const std::string &file_name)
{
    if(mode == Mode::Create){
        for(std::uint32_t i{m_numbers_size.x}; i < m_size.x; ++i){
            for(std::uint32_t j{m_numbers_size.y}; j < m_size.y; ++j) {
                get(i,j).draw.paint = Cell::Draw::Status::Empty;
            }
        }
    }

    FILE *file = fopen(file_name.c_str(), "wb");
    std::uint32_t buffer[2] = {m_board_size.x, m_board_size.y};
    fwrite(&buffer, 1, sizeof(buffer), file);
    fwrite(m_board.get(), sizeof(Cell), m_size.x * m_size.y, file);
    fclose(file);
}

void GameBoard::m_draw(sf::RenderWindow &window)
{
    //std::cout<<"GameBoard::m_draw"<<std::endl;

    for(std::uint32_t i{0}; i < m_size.x; ++i){
        auto *line = &line_y;
        if(i % 5 == 0)
            line = &line_y_s;
        line->setPosition(static_cast<std::float_t>(i) * (m_a),
                          i < m_numbers_size.x ? (m_a) * static_cast<std::float_t>(m_numbers_size.y) : 0.f);
        window.draw(*line);
    }
    for(std::uint32_t i{0}; i < m_size.y; ++i){
        auto *line = &line_x;
        if(i % 5 == 0)
            line = &line_x_s;
        line->setPosition(i < m_numbers_size.y ?
                          m_a * static_cast<std::float_t>(m_numbers_size.x) :
                          0.f,
                          static_cast<std::float_t>(i) * m_a);
        window.draw(*line);
    }


    for(std::uint32_t i{0}; i < m_size.x; ++i)
        for(std::uint32_t j{0}; j < m_size.y; ++j) {
            auto *it = &get(i, j);
            if(it->draw.paint == Cell::Draw::Status::Empty)
                continue;
            auto x = static_cast<std::float_t>(i) * m_a,
            y = static_cast<std::float_t>(j) * m_a;

            if(it->draw.status == Cell::Draw::Status::Number) {
                m_text.setString(std::to_string(it->number));
                m_text.setPosition(x, y);
                window.draw(m_text);
            }else if(it->draw.paint == Cell::Draw::Status::Fill){
                square.setPosition(x, y);
                window.draw(square);
            }
            if(it->draw.paint == Cell::Draw::Status::Cross){
                cross.setPosition(x, y);
                window.draw(cross);
            }
        }
}

void GameBoard::m_handle(const sf::Event &event, sf::RenderWindow &window)
{
    std::cout<<"handle"<<std::endl;
    if(event.type == sf::Event::EventType::MouseButtonReleased){
        auto cords = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        size x = static_cast<size>(cords.x / m_a);
        size y = static_cast<size>(cords.y / m_a);

#ifdef MODERATE
            if (event.mouseButton.button == sf::Mouse::Right)
                std::cout << "hightlights.emplace_back(" << x << ", " << y << ");" << std::endl;
            else
                std::cout <<
                "hightlights.erase(std::find(hightlights.begin(), hightlights.end(), std::pair<std::uint32_t, std::uint32_t>{"
                << x << ", " << y << "}));" << std::endl;
#endif

        Cell *cell_p = &get(x, y);
        bool update{false};
        if(cell_p->draw.status != Cell::Status::Number && event.mouseButton.button == sf::Mouse::Right) {
            update = cell_p->click(true);
        }else if(cell_p->draw.status != Cell::Status::Number && event.mouseButton.button == sf::Mouse::Left){
            update = cell_p->click(false);
        }else if(cell_p->draw.status != Cell::Status::Number && event.mouseButton.button == sf::Mouse::Middle){
            auto *it = m_board.get() + x * m_size.y + y;
            it->draw.paint = it->draw.status;
            update = true;
        }
        if(update) {
            updateNumbers(x, y);
            m_updated = true;
        }
    }else if(event.type == sf::Event::EventType::MouseMoved){
        //Todo lighting

    }else {
        assert(false);
    }
}


bool GameBoard::isWin()
{
    if(mode != Mode::Play || !m_updated)
        return false;
    m_updated = false;
    for (auto *it = m_board.get(), *end = it + m_size.x * m_size.y; it != end; ++it){
        if(it->draw.status == Cell::Status::Number || it->draw.paint == it->draw.status ||
                (it->draw.status == Cell::Draw::Status::Cross && it->draw.paint == Cell::Draw::Status::Empty))
            continue;
        return false;
    }
    m_updated = true;
    return true;
}

void GameBoard::updateNumbers(GameBoard::size x, GameBoard::size y)
{
    if(mode == Mode::Create) {
        for (size i{0}; i < m_numbers_size.x; ++i) {
            auto *it = &get(i, y);
            it->number = 0;
            it->draw.paint = Cell::Draw::Status::Empty;
        }
        for (size i{0}; i < m_numbers_size.y; ++i) {
            auto *it = &get(x, i);
            it->number = 0;
            it->draw.paint = Cell::Draw::Status::Empty;
        }

        size streak{0};
        for (size i{m_numbers_size.x}; i < m_size.x; ++i) {
            auto *it = &get(i, y);
            if (it->draw.paint == Cell::Draw::Status::Fill) {
                ++streak;
            } else if(streak){
                for (size j{m_numbers_size.x}; j != 0; --j) {
                    auto *cell = &get(j - 1, y);
                    if (cell->draw.paint == Cell::Draw::Status::Empty) {
                        *cell = Cell(streak);
                        break;
                    }
                }
                streak = 0;
            }
        }
        if (streak)
            for (size j{m_numbers_size.x}; j != 0; --j) {
                auto *cell = &get(j - 1, y);
                if (cell->draw.paint == Cell::Draw::Status::Empty) {
                    *cell = Cell(streak);
                    streak = 0;
                    break;
                }
            }

        for (size i{m_numbers_size.y}; i < m_size.y; ++i) {
            auto *it = &get(x, i);
            if (it->draw.paint == Cell::Draw::Status::Fill) {
                ++streak;
            } else if(streak){
                for (size j{m_numbers_size.y}; j != 0; --j) {
                    auto *cell = &get(x, j - 1);
                    if (cell->draw.paint == Cell::Draw::Status::Empty) {
                        *cell = Cell(streak);
                        break;
                    }
                }
                streak = 0;
            }
        }

        if (streak)
            for (size j{m_numbers_size.y}; j != 0; --j) {
                auto *cell = &get(x, j - 1);
                if (cell->draw.paint == Cell::Draw::Status::Empty) {
                    *cell = Cell(streak);
                    break;
                }
            }
    }else{
        for (size i{0}; i < m_numbers_size.x; ++i) {
            auto *it = &get(i, y);
            if(it->number !=0)
                it->draw.paint = Cell::Draw::Status::Fill;
        }
        for (size i{0}; i < m_numbers_size.y; ++i) {
            auto *it = &get(x, i);
            if(it->number !=0)
                it->draw.paint = Cell::Draw::Status::Fill;
        }

        size streak{0}, last{m_numbers_size.x};
        for(size j{m_numbers_size.x}; j != 0; --j) {
            auto *cell = &get(j - 1, y);
            if(cell->number == 0)
                break;
            bool find = false;
            for (size i{last}; i < m_size.x; ++i) {
                auto *it = &get(i, y);
                if(find){
                    if (it->draw.paint == Cell::Draw::Status::Fill) {
                        ++streak;
                    }else{
                        last = i;
                        break;
                    }
                }else if (it->draw.paint == Cell::Draw::Status::Fill){
                    find = true;
                    --i;
                }
                last = i;
            }
            if(streak) {
                ++last;
                if (streak >= cell->number)
                    cell->draw.paint = Cell::Draw::Status::Cross;
                else
                    cell->draw.paint = Cell::Draw::Status::Fill;
                streak = 0;
            }
        }
        last = m_numbers_size.y;
        for(size j{m_numbers_size.y}; j != 0; --j) {
            auto *cell = &get(x, j - 1);
            if(cell->number == 0)
                break;
            bool find = false;
            for (size i{last}; i < m_size.y; ++i) {
                auto *it = &get(x, i);
                if(find) {
                    if (it->draw.paint == Cell::Draw::Status::Fill) {
                        ++streak;
                    } else {
                        last = i;
                        break;
                    }
                }else if (it->draw.paint == Cell::Draw::Status::Fill){
                    find = true;
                    --i;
                }
                last = i;
            }
            if(streak) {
                ++last;
                if (streak >= cell->number)
                    cell->draw.paint = Cell::Draw::Status::Cross;
                else
                    cell->draw.paint = Cell::Draw::Status::Fill;
                streak = 0;
            }
        }
        //Todo add auto cross;

    }
}

void GameBoard::init()
{
    std::cout<<"init"<<std::endl;
    square = sf::RectangleShape{sf::Vector2f{m_a, m_a}};
    square.setFillColor(sf::Color::Black);

    highlight_square = sf::RectangleShape{sf::Vector2f{m_a, m_a}};
    auto color = sf::Color::Blue;
    color.a = 180;
    highlight_square.setFillColor(color);

    cross.setTexture(Resources::resources.cross_texture);
    cross.setScale(m_a / cross.getLocalBounds().width, m_a / cross.getLocalBounds().height);

    m_text.setCharacterSize(static_cast<size>(m_a));
    m_text.setFillColor(sf::Color::Black);

    line_x.setFillColor(sf::Color::Black);
    line_y.setFillColor(sf::Color::Black);

    line_x_s.setFillColor(sf::Color::Black);
    line_y_s.setFillColor(sf::Color::Black);

    line_y.setRotation(90);
    line_y_s.setRotation(90);
}

sf::Vector2u GameBoard::getSize() const
{ return m_size; }

void GameBoard::highlightAt(GameBoard::size a_x, GameBoard::size a_y, sf::RenderWindow &window, bool fill)
{
    highlight_square.setPosition(m_a * static_cast<std::float_t>(a_x), m_a * static_cast<std::float_t>(a_y));
    window.draw(highlight_square);
    if(!fill)
        return;
    get(a_x, a_y).draw.paint = Cell::Draw::Status::Fill;
    updateNumbers(a_x, a_y);
}

GameBoard::Cell &GameBoard::get(GameBoard::size x, GameBoard::size y)
{
    assert(x < m_size.x);
    assert(y < m_size.y);
    return *(m_board.get() + x * m_size.y + y);
}

void GameBoard::addHandles()
{
    ADD_HANDLE(drawable);
    //ADD_HANDLE(mouseMoved);
    ADD_HANDLE(mouseButtonReleased);
}


GameBoard::Cell::Cell(std::uint32_t n)
    : number{n}
    , draw{n ? Status::Fill : Status::Empty, Status::Number}
{}

bool GameBoard::Cell::click(bool right_click)
{
    if(draw.status != Status::Number) {
        switch (draw.paint) {
            case Status::Empty:
                draw.paint = right_click ? Status::Cross : Status::Fill;
                break;
            case Status::Fill:
                draw.paint = right_click || mode == Mode::Create ? Status::Cross : Status::Empty;
                break;
            case Status::Cross:
                draw.paint = right_click && mode != Mode::Create ? Status::Empty : Status::Fill;
                break;
            case Status::Number:
            case Status::Error:
            case Status::Size:
                std::cerr << __PRETTY_FUNCTION__ << ": non valid draw.status" << std::endl;
                break;
        }
        if (mode == Mode::Create)
            draw.status = draw.paint;
        return true;
    }else{
        return false;
    }
}

GameBoard::Cell::Cell()
{
    switch(GameBoard::mode){
        case Mode::Create:
            draw.paint = draw.status = Status::Cross;
            break;
        case Mode::Play:
        case Mode::Tutorial:
            draw.paint = Status::Empty;
            draw.status = Status::Cross;
            break;
        case Mode::Error:
        case Mode::Size:
            std::cerr << __PRETTY_FUNCTION__ << ": non valid GameBoard::mode" << std::endl;
            break;
    }
}

#undef WINDOW_CSX
#undef WINDOW_CSY
