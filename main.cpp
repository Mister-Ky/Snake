#pragma once

#ifndef SNAKE_GAME
#define SNAKE_GAME

#define NOMINMAX

#include <deque>
#include <fstream>
#include "icon.hpp"
#include "sansation.hpp"
#include <SFML/Graphics.hpp>

struct GameObject {
    byte x;
    byte y;
    GameObject() : x(0), y(0) {}
    GameObject(byte col, byte row) : x(col), y(row) {}

    inline bool operator ==(const GameObject& right) const {
        return x == right.x && y == right.y;
    }
    inline bool operator !=(const GameObject& right) const {
        return !(*this == right);
    }
};
enum Direction { UP, DOWN, LEFT, RIGHT };
typedef GameObject SnakeSegment;
typedef GameObject Apple;
typedef std::deque<SnakeSegment> SnakeContainer;

class Game 
{
public:
    Game() = default;

    struct GameSettings {
        byte SIZES;
        byte COLS;
        byte ROWS;
        byte framerateWindow;
        bool VSync;
        bool multiPlayer;
        bool isolation;
        bool revival;

        GameSettings() : SIZES(25), COLS(30), ROWS(30), framerateWindow(10), VSync(false), multiPlayer(false), isolation(false), revival(true) {}
    };
private:
    bool gameOver = false;
    unsigned short score = 0;
    Direction dir = RIGHT;
    const std::string titleWindow = "Snake";
    const std::string scoreText = "Score :  ";
    const std::string deadText = "You are dead, your score :  ";
    const std::string respawnText = " | Press R to respawn";
    std::string titleGame = scoreText;

    sf::Color snakeColor = sf::Color::Green;
    const sf::Color segmentOver = sf::Color(185, 0, 0);
    const sf::Color appleColor = sf::Color(150, 0, 0);

    byte nextX = 0;
    byte nextY = 0;
public:
    GameSettings settings;

    sf::Image icon;
    sf::Font font;

    void GAMEOVER()
    {
        gameOver = true;
        snakeColor = sf::Color::Red;
        titleGame = deadText;
    }

    void moveSnake(SnakeContainer& snake, Direction& dir)
    {
        if (!gameOver)
        {
            nextX = snake.front().x;
            nextY = snake.front().y;

            switch (dir) {
            case UP:
                nextY--;
                break;
            case DOWN:
                nextY++;
                break;
            case LEFT:
                nextX--;
                break;
            case RIGHT:
                nextX++;
                break;
            }

            if (settings.isolation)
            {
                if (nextX >= settings.COLS) nextX = 0;
                else if (nextX < 0) nextX = settings.COLS - 1;
                else if (nextY >= settings.ROWS) nextY = 0;
                else if (nextY < 0) nextY = settings.ROWS - 1;
            }
            else if (nextX >= settings.COLS || nextX < 0 || nextY >= settings.ROWS || nextY < 0) GAMEOVER();

            for (const auto& segment : snake) {
                if (GameObject(nextX, nextY) == segment) {
                    GAMEOVER();
                    return;
                }
            }
            if (gameOver) return;
            snake.push_front(SnakeSegment(nextX, nextY));
            snake.pop_back();
        }
    }

    void generateApple(Apple& apple, const SnakeContainer& snake)
    {
        while (true) {
            apple.x = rand() % settings.COLS;
            apple.y = rand() % settings.ROWS;

            bool collision = false;
            for (const auto& segment : snake) {
                if (apple == segment) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                break;
            }
        }
    }

    void keyProc(sf::Event& event)
    {
        if (event.key.code == sf::Keyboard::Up && dir != DOWN) {
            dir = UP;
        }
        else if (event.key.code == sf::Keyboard::Down && dir != UP) {
            dir = DOWN;
        }
        else if (event.key.code == sf::Keyboard::Left && dir != RIGHT) {
            dir = LEFT;
        }
        else if (event.key.code == sf::Keyboard::Right && dir != LEFT) {
            dir = RIGHT;
        }
    }

    void initSnake(SnakeContainer* snake)
    {
        snake->clear();
        snake->push_front(SnakeSegment(3, 5));
        snake->push_front(SnakeSegment(4, 5));
        snake->push_front(SnakeSegment(5, 5));
    }

    void init(sf::RenderWindow* window, sf::Text* text, SnakeContainer* snake)
    {
        window->setFramerateLimit(settings.framerateWindow);
        window->setVerticalSyncEnabled(settings.VSync);
        window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        text->setFont(font);
        text->setCharacterSize(unsigned int(std::min(window->getSize().x * 0.04, window->getSize().y * 0.04)));

        initSnake(snake);
    }

    void restartGame(Apple* apple, SnakeContainer* snake)
    {
        score = 0;
        titleGame = scoreText;
        snakeColor = sf::Color::Green;
        dir = RIGHT;
        initSnake(snake);
        generateApple(*apple, *snake);
        gameOver = false;
    }

    void processEvents(sf::RenderWindow& window, Apple* apple, SnakeContainer* snake)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::R && settings.revival && gameOver)
                {
                    restartGame(apple, snake);
                }
                else if (!gameOver)
                {
                    keyProc(event);
                    break;
                }
            }
            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    void updateGame(SnakeContainer& snake, Apple& apple)
    {
        moveSnake(snake, dir);

        if (snake.front() == apple) {
            score++;
            generateApple(apple, snake);
            snake.push_back(SnakeSegment(-1, 0));
        }
    }

    void renderGame(sf::RenderWindow& window, sf::Text& text, sf::RectangleShape& gameObject, SnakeContainer& snake, Apple& apple)
    {
        window.clear();

        for (const auto& segment : snake) {
            gameObject.setPosition(float(segment.x * settings.SIZES), float(segment.y * settings.SIZES));
            gameObject.setFillColor(snakeColor);
            if (snake.front() == segment)
            {
                if (!gameOver)
                    gameObject.setFillColor(sf::Color(0, 150, 0));
                else
                    gameObject.setFillColor(segmentOver);
            }

            window.draw(gameObject);
        }

        gameObject.setPosition(float(apple.x * settings.SIZES), float(apple.y * settings.SIZES));
        gameObject.setFillColor(appleColor);
        window.draw(gameObject);

        text.setString(titleGame + std::to_string(score) + (gameOver ? (settings.revival ? respawnText : "") : ""));
        window.draw(text);

        window.display();
    }

    byte start()
    {
        sf::RenderWindow window(sf::VideoMode(settings.COLS * settings.SIZES, settings.ROWS * settings.SIZES), titleWindow, sf::Style::Close);
        sf::RectangleShape gameObject(sf::Vector2f(settings.SIZES, settings.SIZES));
        sf::Text text;
        SnakeContainer snake;
        Apple apple;

        init(&window, &text, &snake);
        generateApple(apple, snake);

        while (window.isOpen())
        {
            processEvents(window, &apple, &snake);
            updateGame(snake, apple);
            renderGame(window, text, gameObject, snake, apple);
        }
        return EXIT_SUCCESS;
    }
};

void createFileCmd()
{
    std::ofstream fileCmd = std::ofstream("modificationSnake.cmd");
    fileCmd << "@echo off\n";
    fileCmd << "start \"\" Snake.exe ";
    fileCmd << "-size 25 -cols 30 -rows 30 -framerate 10 -vsync false -multiplayer false -isolation false -revival true\n";
    fileCmd << "::  size 10 - 25\n";
    fileCmd << "::  cols 15 - 30\n";
    fileCmd << "::  rows 15 - 30\n";
    fileCmd << "::  framerate 8 - 20\n";
    fileCmd << "::  vsync false - true\n";
    fileCmd << "::  multiplayer false - true\n";
    fileCmd << "::  isolation false - true\n";
    fileCmd << "::  revival false - true\n";
    fileCmd << "exit\n";
    fileCmd.close();
}

bool setBoolFromArg(std::string& argS, bool& setting) {
    if (argS == "true") {
        setting = true;
    }
    else if (argS == "false") {
        setting = false;
    }
    else {
        return false;
    }
    return true;
}

typedef bool (*Handler)(std::string&, Game::GameSettings&);

bool handleSize(std::string& arg, Game::GameSettings& settings) {
    settings.SIZES = std::stoi(arg);
    return true;
}
bool handleCols(std::string& arg, Game::GameSettings& settings) {
    settings.COLS = std::stoi(arg);
    return true;
}
bool handleRows(std::string& arg, Game::GameSettings& settings) {
    settings.ROWS = std::stoi(arg);
    return true;
}
bool handleFramerateWindow(std::string& arg, Game::GameSettings& settings) {
    settings.framerateWindow = std::stoi(arg);
    return true;
}
bool handleVSync(std::string& arg, Game::GameSettings& settings) {
    if (!setBoolFromArg(arg, settings.VSync)) {
        return false;
    }
    return true;
}
bool handleMultiPlayer(std::string& arg, Game::GameSettings& settings) {
    if (!setBoolFromArg(arg, settings.multiPlayer)) {
        return false;
    }
    return true;
}
bool handleIsolation(std::string& arg, Game::GameSettings& settings) {
    if (!setBoolFromArg(arg, settings.isolation)) {
        return false;
    }
    return true;
}
bool handleRevival(std::string& arg, Game::GameSettings& settings) {
    if (!setBoolFromArg(arg, settings.revival)) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    Game game = Game();

    std::map<std::string, Handler> handlers;
    handlers["-size"] = handleSize;
    handlers["-cols"] = handleCols;
    handlers["-rows"] = handleRows;
    handlers["-framerate"] = handleFramerateWindow;
    handlers["-vsync"] = handleVSync;
    handlers["-multiPlayer"] = handleMultiPlayer;
    handlers["-isolation"] = handleIsolation;
    handlers["-revival"] = handleRevival;

    for (int i = 1; i < argc; i += 2) {
        std::string param(argv[i]);
        if (param == "-modification") {
            createFileCmd();
            return EXIT_SUCCESS;
        }
        else if (i + 1 < argc) {
            try {
                char* arg = argv[i + 1];
                std::string argS(arg);
                if (arg[0] == '-') {
                    return EXIT_FAILURE;
                }
                else {
                    auto handler = handlers.find(param);
                    if (handler != handlers.end()) {
                        if (!handler->second(argS, game.settings)) {
                            return EXIT_FAILURE;
                        }
                    }
                }
            }
            catch (std::invalid_argument) {
                return EXIT_FAILURE;
            }
            catch (std::out_of_range) {
                return EXIT_FAILURE;
            }
        }
    }
    game.settings.SIZES = std::clamp(game.settings.SIZES, byte(10), byte(25));
    game.settings.COLS = std::clamp(game.settings.COLS, byte(15), byte(30));
    game.settings.ROWS = std::clamp(game.settings.ROWS, byte(15), byte(30));
    game.settings.framerateWindow = std::clamp(game.settings.framerateWindow, byte(8), byte(20));

    if (!game.icon.loadFromMemory(iconBytes, sizeof(iconBytes))
        || !game.font.loadFromMemory(sansationBytes, sizeof(sansationBytes)))
        return EXIT_FAILURE;

    srand(static_cast<unsigned>(time(nullptr)));

    return game.start();
}

#endif
