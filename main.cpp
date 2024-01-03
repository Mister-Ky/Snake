#pragma once

#ifndef SNAKE_GAME
#define SNAKE_GAME

#define NOMINMAX

#include <deque>
#include <fstream>
#include <icon.hpp>
#include <sansation.hpp>
#include <SFML/Graphics.hpp>

//My discord - mister_ky
//I am not the author of the idea of the game, I just wrote the code that allows you to play the game

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction dir = RIGHT;

byte SIZES = 25;
byte COLS = 30;
byte ROWS = 30;
byte framerateWin = 10;
bool VSync = false;

sf::Image icon;
sf::Font font;
bool gameOver = false;
unsigned short score = 0;
std::string titleGame = "Score:  ";

sf::Color snakeColor = sf::Color::Green;
const sf::Color segOver = sf::Color(185, 0, 0);
const sf::Color appleColor = sf::Color(150, 0, 0);

struct GameObject {
    int x;
    int y;
    GameObject(int col, int row) : x(col), y(row) {}

    inline bool operator ==(const GameObject& right) const
    {
        return x == right.x && y == right.y;
    }
    inline bool operator !=(const GameObject& right) const
    {
        return !(*this == right);
    }
};

using SnakeSegment = GameObject;
using Apple = GameObject;
using SnakeContainer = std::deque<SnakeSegment>;

void moveSnake(SnakeContainer& snake, Direction& dir) {
    if (!gameOver) 
    {
        int nextX = snake.front().x;
        int nextY = snake.front().y;

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

        if (nextX >= COLS) nextX = 0;
        else if (nextX < 0) nextX = COLS - 1;
        else if (nextY >= ROWS) nextY = 0;
        else if (nextY < 0) nextY = ROWS - 1;

        for (const auto& segment : snake) {
            if (GameObject(nextX, nextY) == segment) {
                gameOver = true;
                snakeColor = sf::Color::Red;
                titleGame = "You are dead, your score:  ";
                return;
            }
        }

        snake.push_front(SnakeSegment(nextX, nextY));
        snake.pop_back();
    }
}

void generateApple(Apple& apple, const SnakeContainer& snake) {
    while (true) {
        apple.x = rand() % COLS;
        apple.y = rand() % ROWS;

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

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i += 2) {
        std::string param(argv[i]);
        if (param == "-edition") {
            std::ofstream fileCmd = std::ofstream("editionSnake.cmd");
            fileCmd << "@echo off\n";
            fileCmd << "start \"\" Snake.exe -size 25 -cols 30 -rows 30 -framerate 10 -vsync false\n";
            fileCmd << "::  size 10 - 25\n";
            fileCmd << "::  cols 15 - 30\n";
            fileCmd << "::  rows 15 - 30\n";
            fileCmd << "::  framerate 8 - 20\n";
            fileCmd << "::  vsync false - true\n";
            fileCmd << "exit\n";
            fileCmd.close();
            return EXIT_SUCCESS;
        }
        else if (i + 1 < argc) {
            try {
                char* arg = argv[i + 1];
                if (arg[0] == '-') {
                    return EXIT_FAILURE;
                }
                else if (param == "-size") {
                    SIZES = std::stoi(arg);
                }
                else if (param == "-cols") {
                    COLS = std::stoi(arg);
                }
                else if (param == "-rows") {
                    ROWS = std::stoi(arg);
                }
                else if (param == "-framerate") {
                    framerateWin = std::stoi(arg);
                }
                else if (param == "-vsync") {
                    std::string paramVSync(arg);
                    if (paramVSync == "true") {
                        VSync = true;
                    }
                    else if (paramVSync == "false") {
                        VSync = false;
                    }
                    else {
                        return EXIT_FAILURE;
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
    SIZES = std::clamp(SIZES, byte(10), byte(25));
    COLS = std::clamp(COLS, byte(15), byte(30));
    ROWS = std::clamp(ROWS, byte(15), byte(30));
    framerateWin = std::clamp(framerateWin, byte(8), byte(20));

    if (!icon.loadFromMemory(iconBytes, sizeof(iconBytes))
        || !font.loadFromMemory(sansationBytes, sizeof(sansationBytes)))
        return EXIT_FAILURE;

    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(COLS * SIZES, ROWS * SIZES), "Snake", sf::Style::Close);
    window.setFramerateLimit(framerateWin);
    window.setVerticalSyncEnabled(VSync);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(unsigned int(std::min(window.getSize().x * 0.04, window.getSize().y * 0.04)));

    SnakeContainer snake;
    snake.push_front(SnakeSegment(3, 5));
    snake.push_front(SnakeSegment(4, 5));
    snake.push_front(SnakeSegment(5, 5));

    Apple apple(0, 0);
    generateApple(apple, snake);

    sf::RectangleShape gameObject(sf::Vector2f(SIZES, SIZES));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (!gameOver)
                {
                    keyProc(event);
                    break;
                }
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        moveSnake(snake, dir);

        if (snake.front() == apple) {
            score++;
            generateApple(apple, snake);
            snake.push_back(SnakeSegment(-1, 0));
        }

        window.clear();

        for (const auto& segment : snake) {
            gameObject.setPosition(float(segment.x * SIZES), float(segment.y * SIZES));
            gameObject.setFillColor(snakeColor);
            if (snake.front() == segment)
            {
                if (!gameOver)
                    gameObject.setFillColor(sf::Color(0, 150, 0));
                else
                    gameObject.setFillColor(segOver);
            }

            window.draw(gameObject);
        }

        gameObject.setPosition(float(apple.x * SIZES), float(apple.y * SIZES));
        gameObject.setFillColor(appleColor);
        window.draw(gameObject);

        text.setString(titleGame + std::to_string(score));
        window.draw(text);

        window.display();
    }
    return EXIT_SUCCESS;
}

#endif
