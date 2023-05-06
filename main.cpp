#include <iostream>
#include <fstream>
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
using namespace std;

int main() {
    string width;
    string height;
    string numMines;
    ifstream file("boards/config.cfg");
    getline(file, width);
    getline(file, height);
    getline(file, numMines);

    Board board;
    board.createBoard(stoi(width), stoi(height), stoi(numMines));

    bool debug = false;

    sf::RenderWindow window(sf::VideoMode(32*stoi(width), 37*stoi(height)), "Minesweeper");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            window.clear();

            board.setFlagCounter();

            int tilesLeft = board.findHiddenTiles();

            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);

                    if (mousePosition.x >= 32*(board.width-2)/2 && mousePosition.x <= 32*(board.width-2)/2 + 64 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64) {
                        board.resetBoard();
                        debug = false;
                    } else if (mousePosition.x >= 32*(board.width-2)/2 + 128 && mousePosition.x <= 32*(board.width-2)/2 + 192 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64 && !debug && !board.gameOver) {
                        debug = true;
                        for (int j = 0; j < board.tiles.size(); j++) {
                            if (board.tiles[j].isMine) {
                                board.tiles[j].clicked = true;
                                board.tiles[j].background.setTexture(board.textures["tilerevealed"]);
                                board.tiles[j].foreground.setTexture(board.textures["mine"]);
                            }
                        }
                    } else if (mousePosition.x >= 32*(board.width-2)/2 + 128 && mousePosition.x <= 32*(board.width-2)/2 + 192 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64 && debug && !board.gameOver) {
                        debug = false;
                        for (int j = 0; j < board.tiles.size(); j++) {
                            if (board.tiles[j].isMine) {
                                board.tiles[j].clicked = false;
                                board.tiles[j].background.setTexture(board.textures["tilehidden"]);
                            }
                        }
                    } else if (mousePosition.x >= 32*(board.width-2)/2 + 192 && mousePosition.x <= 32*(board.width-2)/2 + 256 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64) {
                        board.testBoard1();
                        debug = false;
                    } else if (mousePosition.x >= 32*(board.width-2)/2 + 256 && mousePosition.x <= 32*(board.width-2)/2 + 320 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64) {
                        board.testBoard2();
                        debug = false;
                    } else if (mousePosition.x >= 32*(board.width-2)/2 + 320 && mousePosition.x <= 32*(board.width-2)/2 + 384 && mousePosition.y >= 32*board.height && mousePosition.y <= 32*board.height + 64) {
                        board.testBoard3();
                        debug = false;
                    }



                    for (int i = 0; i < board.tiles.size(); i++) {

                        if (mousePosition.x/32 + 1 + (board.width * (mousePosition.y/32)-1) == i && !board.tiles[i].flagged) {

                            if (board.tiles[i].isMine && !debug) {
                                board.gameOver = true;
                                board.bottomButtons[1].background.setTexture(board.textures["facelose"]);
                                board.tiles[i].clicked = true;
                                board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                board.tiles[i].foreground.setTexture(board.textures["mine"]);
                                for (int j = 0; j < board.tiles.size(); j++) {
                                    board.tiles[j].clicked = true;
                                    if (board.tiles[j].isMine) {
                                        board.tiles[j].background.setTexture(board.textures["tilerevealed"]);
                                        board.tiles[j].foreground.setTexture(board.textures["mine"]);
                                    }
                                }
                            } else if (!board.tiles[i].clicked){
                                if (tilesLeft == 1 || (tilesLeft == 0 && board.numMines == 1)) {
                                    board.flagMineCounter = 0;
                                    board.gameWon = true;
                                    board.gameOver = true;
                                    board.bottomButtons[1].background.setTexture(board.textures["facewin"]);
                                    for (int j = 0; j < board.tiles.size(); j++) {
                                        if (board.tiles[j].isMine) {
                                            board.tiles[j].flagged = true;
                                            board.tiles[j].extra.setTexture(board.textures["flag"]);
                                        }
                                    }
                                }

                                if (board.tiles[i].neighborMines == 0) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.revealTiles(&board.tiles[i]);
                                }
                                board.tiles[i].clicked = true;
                                if (board.tiles[i].neighborMines == 1) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number1"]);
                                }
                                if (board.tiles[i].neighborMines == 2) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number2"]);
                                }
                                if (board.tiles[i].neighborMines == 3) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number3"]);
                                }
                                if (board.tiles[i].neighborMines == 4) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number4"]);
                                }
                                if (board.tiles[i].neighborMines == 5) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number5"]);
                                }
                                if (board.tiles[i].neighborMines == 6) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number6"]);
                                }
                                if (board.tiles[i].neighborMines == 7) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number7"]);
                                }
                                if (board.tiles[i].neighborMines == 8) {
                                    board.tiles[i].background.setTexture(board.textures["tilerevealed"]);
                                    board.tiles[i].foreground.setTexture(board.textures["number8"]);
                                }
                            }
                        }
                    }


                    }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);

                    for (int i = 0; i < board.tiles.size(); i++) {
                        if (mousePosition.x/32 + 1 + (board.width * (mousePosition.y/32)-1) == i) {
                            if (!board.tiles[i].flagged && !board.tiles[i].clicked && !board.gameOver) {
                                board.flagMineCounter--;
                                board.tiles[i].flagged = true;
                                board.tiles[i].extra.setTexture(board.textures["flag"]);
                            } else {
                                if (!board.gameOver) {
                                    board.tiles[i].flagged = false;
                                    if (!board.tiles[i].clicked) {
                                        board.flagMineCounter++;
                                    }
                                }

                            }
                        }
                    }



                }
                }
            }


        for (int i = 0; i < board.tiles.size(); i++) {
            window.draw(board.tiles[i].background);
            if (board.tiles[i].flagged ) {
                window.draw(board.tiles[i].extra);
            }
            if (board.tiles[i].clicked ) {
                window.draw(board.tiles[i].foreground);
            }

        }

        for (int i = 0; i < board.bottomButtons.size(); i++) {
            window.draw(board.bottomButtons[i].background);
        }

        for (int i = 0; i < board.counterSprites.size(); i++) {
            window.draw(board.counterSprites[i]);
        }

        window.display();
    }

    return 0;
}
