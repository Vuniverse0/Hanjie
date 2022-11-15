#include <cassert>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


#include "Button.hpp"
#include "GameBoard.hpp"
#include "Resources.hpp"
#include "Settings.hpp"
#include "Sprite.hpp"
#include "TextField.hpp"


int main()
{
    const unsigned int FPS = 60;
    bool redraw = true;
    std::int32_t width = MENU_WIDTH, height = MENU_HEIGHT;

    sf::RenderWindow window(sf::VideoMode(width, height), "Hanjie");
    window.setFramerateLimit(FPS);

    sf::Clock clock;
    TextField field{};
    GameBoard board{};
    Sprite rules{Resources::resources.rules_texture};

    field.hide();
    board.hide();
    rules.hide();

    std::vector<std::pair<std::uint32_t, std::uint32_t>> highlights(0);

    std::function<std::array<Button, 3>&()> get_buttons;

    auto showMenu = [&](){
        for (auto &item : get_buttons())
            item.show();
        board.hide();
        field.hide();
        GameBoard::mode = GameBoard::Mode::Error;
        window.setSize(sf::Vector2u(MENU_WIDTH, MENU_HEIGHT));
    };

    auto tutorial = ([&](){
        if(GameBoard::mode != GameBoard::Mode::Tutorial){
            for (auto &item : get_buttons())
                item.hide();
            rules.show();
            GameBoard::mode = GameBoard::Mode::Tutorial;
            return;
        }
        static std::uint32_t tutorial_steps{0};
        switch (tutorial_steps++) {
            case 0 :
                rules.hide();
                GameBoard::mode = GameBoard::Mode::Play;
                board = GameBoard{window, TUTORIAL_FILE};
                GameBoard::mode = GameBoard::Mode::Tutorial;
            case 1 :
                highlights = {{2, 6}, {3, 6}, {4, 6}, {5, 6}, {7, 6}, {9, 6}};
                break;
            case 2 :
                highlights = {{8, 4}, {6, 4}, {6, 5}, {8, 5}};
                break;
            case 3 :
                highlights = {{3, 7}, {4, 7}, {5, 7}, {9, 7}};
                break;
            case 4 :
                highlights = {{3, 8}, {4, 8}, {4, 9}, {6, 8}, {7, 9}, {8, 8}};
                break;
            case 5 :
                highlights.clear();
                break;
            default :
                tutorial_steps = 0;
                showMenu();
                break;
        }
    });

    auto play = ([&]{
        static std::uint32_t play_steps{0};

        switch (play_steps++) {
            case 0 : {
                field.show();
                field.setTitle("Enter name: ");
                field.transform().setPosition(window.mapPixelToCoords(sf::Vector2i{width/2 - 200, height/2 + 200}));
                get_buttons()[0].hide(), get_buttons()[2].hide();
                break;
            }case 1 : {
                auto tmp = fopen(field.getString().c_str(), "rb");
                if(tmp) {
                    fclose(tmp);
                    GameBoard::mode = GameBoard::Mode::Play;
                    (board = GameBoard{window, field.getString()}).show();
                    get_buttons()[1].hide();
                    field.hide();
                }else {
                    --play_steps;
                    field.clear();
                }
                break;
            }case 2 : {
                if (board.isWin()) {
                    field.setTitle("Win").lock().show();
                }else{
                    field.setTitle("Enter save name: ").show();
                    board.hide();
                }
                board.hide();
                break;
            }case 3 : {
                if(board.isWin() || field.getString().empty())
                    break;
                auto tmp = fopen(field.getString().c_str(), "rb");
                if(tmp) {
                    fclose(tmp);
                    --play_steps, field.clear();
                    break;
                }else{
                    board.save(field.getString());
                }
            }default: {
                play_steps = 0;
                showMenu();
            }
    }
    });

    auto create = ([&]{
        static std::uint32_t create_steps{0};

        switch (create_steps++) {
            case 0 : {
                field.show();
                field.setTitle("Enter size of field x, y: ");
                field.transform().setPosition(window.mapPixelToCoords(sf::Vector2i{width / 2 - 200, height / 2}));
                get_buttons()[0].hide(), get_buttons()[1].hide();
                break;
            }case 1 : {
                std::stringstream ss{field.getString()};
                std::uint32_t x_size{0}, y_size{0};
                ss >> x_size >> y_size;
                if (x_size == 0 || y_size == 0)
                    field.setTitle("Enter name for open: ");
                else
                    GameBoard::mode = GameBoard::Mode::Create,
                    (board = GameBoard{window, x_size, y_size}).show(),
                    ++create_steps, get_buttons()[2].hide(), field.hide();
                break;
            }case 2 : {
                auto tmp = fopen(field.getString().c_str(), "rb");
                if(tmp){
                    fclose(tmp);
                    GameBoard::mode = GameBoard::Mode::Create;
                    (board = GameBoard{window, field.getString()}).show();
                    get_buttons()[2].hide();
                    field.hide();
                }else{
                    --create_steps;
                    field.clear();
                }
                break;
            }case 3 : {
                field.setTitle("Enter name for save: ");
                field.show();
                board.hide();
                get_buttons()[2].show();
                window.setSize(sf::Vector2u(MENU_WIDTH, MENU_HEIGHT));
                break;
            }case 4 : {
                if(field.getString().empty()){
                    --create_steps, field.clear();
                    break;
                }
            }default :
                board.save(field.getString());
                create_steps = 0;
                showMenu();
                break;
        }
    });


    std::array<Button, 3> buttons{ Button
        {"Tutorial", sf::Vector2i{width/2, height/2}, tutorial},
        {"Play", {width/2, height/2+100}, play},
        {"Create", {width/2, height/2+200}, create}
    };

    get_buttons = [&buttons]()->std::array<Button, 3>&{ return buttons; };

    sf::Event event{};
    while (window.isOpen()){
        if(GameBoard::mode == GameBoard::Mode::Play && board.isWin()){
            //board->hide();
            //playerInput = "Win";
            //playerText.scale(5, 5);
            //playerText.setPosition(width/2 - 100, height/2 - 100);
        }
        //Wait until 1/60th of a second has passed, then update everything.
        if (clock.getElapsedTime().asSeconds() >= 1.0f / FPS)
        {
            redraw = true; //We're ready to redraw everything

            clock.restart();
        }

        //Handle input
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    break;
                case sf::Event::LostFocus:
                    break;
                case sf::Event::GainedFocus:
                    break;
                case sf::Event::TextEntered:
                    for(auto item : Entry::textEntered)
                        item->handle(event, window);
                    break;
                case sf::Event::KeyPressed:
                    break;
                case sf::Event::KeyReleased:
                    if(event.key.code == sf::Keyboard::Enter)
                        switch (GameBoard::mode) {
                            case GameBoard::Mode::Create:
                                std::cout<<"KEY RELEASED"<<std::endl;
                                create();
                                break;
                            case GameBoard::Mode::Play:
                                play();
                                break;
                            case GameBoard::Mode::Tutorial:
                                tutorial();
                                break;
                            case GameBoard::Mode::Error:
                            case GameBoard::Mode::Size:
                                break;
                        }
                        break;
                case sf::Event::MouseWheelMoved:
                    break;
                case sf::Event::MouseWheelScrolled:
                    break;
                case sf::Event::MouseButtonPressed:
                    break;
                case sf::Event::MouseButtonReleased:
                    if(GameBoard::mode == GameBoard::Mode::Tutorial){
                        tutorial();
                        break;
                    }else{
                        for (auto item : Entry::mouseButtonReleased)
                            item->handle(event, window);
                    }
                    if(GameBoard::mode == GameBoard::Mode::Play && board.isWin()) {
                        std::cout<<"win"<<std::endl;
                        play();
                    }
                    break;
                case sf::Event::MouseMoved:
                    for(auto item : Entry::mouseMoved)
                        item->handle(event, window);
                    break;
                case sf::Event::MouseEntered:
                    break;
                case sf::Event::MouseLeft:
                    break;
                case sf::Event::JoystickButtonPressed:
                    break;
                case sf::Event::JoystickButtonReleased:
                    break;
                case sf::Event::JoystickMoved:
                    break;
                case sf::Event::JoystickConnected:
                    break;
                case sf::Event::JoystickDisconnected:
                    break;
                case sf::Event::TouchBegan:
                    break;
                case sf::Event::TouchMoved:
                    break;
                case sf::Event::TouchEnded:
                    break;
                case sf::Event::SensorChanged:
                    break;
                case sf::Event::Count:
                    break;
            }
        }

        ///Draw stuff if ready
        if (redraw)
        {
            redraw = false;
            window.clear(sf::Color::White);
            for(auto& item : Entry::drawable)
                item->draw(window);
            if(GameBoard::mode == GameBoard::Mode::Tutorial)
                for(auto& item : highlights)
                    board.highlightAt(item.first, item.second, window);
            window.display();
        }
    }

    return 0;
}
