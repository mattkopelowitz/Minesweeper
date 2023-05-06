#pragma once
#include <iostream>
#include <vector>
using namespace std;
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>
#include <fstream>

class Board {
public:
    struct Tile {

        sf::Sprite background, foreground, extra;

        Tile* top = nullptr;
        Tile* bottom = nullptr;
        Tile* left = nullptr;
        Tile* right = nullptr;
        Tile* topLeft = nullptr;
        Tile* topRight = nullptr;
        Tile* bottomLeft = nullptr;
        Tile* bottomRight = nullptr;

        bool isMine = false, flagged = false, clicked = false;
        int neighborMines = 0;

        vector<Tile*> neighbors;

        Tile() {
            neighbors.push_back(top);
            neighbors.push_back(topRight);
            neighbors.push_back(right);
            neighbors.push_back(bottomRight);
            neighbors.push_back(bottom);
            neighbors.push_back(bottomLeft);
            neighbors.push_back(left);
            neighbors.push_back(topLeft);
        }


    };
public:
    vector<Tile> tiles;

    int width, height, mines;

    map<string, sf::Texture> textures;
    map<string, sf::Sprite> digits;

    bool gameOver = false;
    bool gameWon = false;

    int flagMineCounter = mines;

    vector<sf::Sprite> counterSprites;
    vector<Tile> bottomButtons;
    vector<Tile> flagCounter;

    int hiddenTiles;

    int numMines;

    void createBoard(int w, int h, int m) {

        tiles.clear();
        width = w;
        height = h;
        mines = m;
        gameOver = false;
        gameWon = false;
        hiddenTiles = width*height-mines;

        numMines = mines;
        flagMineCounter = mines;


        loadTextures();

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                Tile newTile;
                newTile.background.setTexture(textures["tilehidden"]);
                newTile.extra.setTexture(textures["flag"]);
                newTile.background.setPosition(32*j, 32*i);
                newTile.foreground.setPosition(32*j, 32*i);
                newTile.extra.setPosition(32*j, 32*i);
                tiles.push_back(newTile);
            } //add tiles to the board

        for (int i = 0; i < tiles.size(); i++) {

            if (i == 0) { //top left
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width-1) { //top right
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i == (width-1)*height) { //bottom left
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width*height-1) { //bottom right
                tiles[i].top = &tiles[i-width];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i < width) { //top edge
                tiles[i].top = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];

            } else if (i % width == 0) { //left edge
                tiles[i].left = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].right = &tiles[i+1];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].bottomRight = &tiles[i-width+1];
                tiles[i].top = &tiles[i-width];
                tiles[i].bottom = &tiles[i+width];

            } else if (i % width == width-1) { //right edge
                tiles[i].right = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].top = &tiles[i-width];

            } else if (i > ((width-1) * height)) { //bottom edge
                tiles[i].bottom = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
            } else {
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
            }

        } //link all tiles to each other

        srand(time(NULL));
        while (numMines > 0) {

            int random = rand() % (width*height);

            if (!tiles[random].isMine) {
                tiles[random].isMine = true;
                numMines--;
            }
        } //set mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].topLeft && tiles[i].topLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].topRight && tiles[i].topRight->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].top && tiles[i].top->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].left && tiles[i].left->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].right && tiles[i].right->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomLeft && tiles[i].bottomLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottom && tiles[i].bottom->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomRight && tiles[i].bottomRight->isMine) {
                tiles[i].neighborMines++;
            }
        } //check num of neighboring mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].isMine) {
                tiles[i].foreground.setTexture(textures["mine"]);
            }

        } //set foreground textures to mines

        //digits
        Tile digits;
        //digits.background.setTexture(textures["digits"]);
        //digits.background.setPosition(32*(width-2)/width, 32*height);
        digits.left = nullptr;
        digits.topLeft = nullptr;
        digits.top = nullptr;
        digits.topRight = nullptr;
        digits.right = nullptr;
        digits.bottomRight = nullptr;
        digits.bottom = nullptr;
        digits.bottomLeft = nullptr;
        bottomButtons.push_back(digits);

        //happy face
        Tile newTile;
        newTile.background.setTexture(textures["facehappy"]);
        newTile.background.setPosition(32*(width-2)/2, 32*height);
        newTile.left = nullptr;
        newTile.topLeft = nullptr;
        newTile.top = nullptr;
        newTile.topRight = nullptr;
        newTile.right = nullptr;
        newTile.bottomRight = nullptr;
        newTile.bottom = nullptr;
        newTile.bottomLeft = nullptr;
        bottomButtons.push_back(newTile);

        //debug
        Tile debug;
        debug.background.setTexture(textures["debug"]);
        debug.background.setPosition(32*(width-2)/2 + 128, 32*height);
        debug.left = nullptr;
        debug.topLeft = nullptr;
        debug.top = nullptr;
        debug.topRight = nullptr;
        debug.right = nullptr;
        debug.bottomRight = nullptr;
        debug.bottom = nullptr;
        debug.bottomLeft = nullptr;
        bottomButtons.push_back(debug);

        //test1
        Tile test1;
        test1.background.setTexture(textures["test1"]);
        test1.background.setPosition(32*(width-2)/2 + 192, 32*height);
        test1.left = nullptr;
        test1.topLeft = nullptr;
        test1.top = nullptr;
        test1.topRight = nullptr;
        test1.right = nullptr;
        test1.bottomRight = nullptr;
        test1.bottom = nullptr;
        test1.bottomLeft = nullptr;
        bottomButtons.push_back(test1);

        //test2
        Tile test2;
        test2.background.setTexture(textures["test2"]);
        test2.background.setPosition(32*(width-2)/2 + 256, 32*height);
        test2.left = nullptr;
        test2.topLeft = nullptr;
        test2.top = nullptr;
        test2.topRight = nullptr;
        test2.right = nullptr;
        test2.bottomRight = nullptr;
        test2.bottom = nullptr;
        test2.bottomLeft = nullptr;
        bottomButtons.push_back(test2);

        //test3
        Tile test3;
        test3.background.setTexture(textures["test3"]);
        test3.background.setPosition(32*(width-2)/2 + 320, 32*height);
        test3.left = nullptr;
        test3.topLeft = nullptr;
        test3.top = nullptr;
        test3.topRight = nullptr;
        test3.right = nullptr;
        test3.bottomRight = nullptr;
        test3.bottom = nullptr;
        test3.bottomLeft = nullptr;
        bottomButtons.push_back(test3);

        setFlagCounter();

    }

    void resetBoard() {
        tiles.clear();
        bottomButtons.clear();
        flagMineCounter = mines;
        createBoard(width, height, mines);
    }

    void testBoard1() {
        tiles.clear();
        bottomButtons.clear();
        ifstream file("boards/testboard1.brd");
        string stream;
        int m = 0;
        int count = 0;
        vector<char> mineField;
        while (getline(file, stream)) {
            width = stream.size();
            count++;
            for (int i = 0; i < stream.size(); i++) {
                mineField.push_back(stream[i]);
            }
        }
        tiles.clear();
        height = count;
        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                m++;
            }
        }
        gameOver = false;
        gameWon = false;
        hiddenTiles = width*height-m;

        int numMines = m;
        flagMineCounter = m;

        loadTextures();

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                Tile newTile;
                newTile.background.setTexture(textures["tilehidden"]);
                newTile.extra.setTexture(textures["flag"]);
                newTile.background.setPosition(32*j, 32*i);
                newTile.foreground.setPosition(32*j, 32*i);
                newTile.extra.setPosition(32*j, 32*i);
                tiles.push_back(newTile);
            } //add tiles to the board

        for (int i = 0; i < tiles.size(); i++) {

            if (i == 0) { //top left
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width-1) { //top right
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i == (width-1)*height) { //bottom left
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width*height-1) { //bottom right
                tiles[i].top = &tiles[i-width];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i < width) { //top edge
                tiles[i].top = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];

            } else if (i % width == 0) { //left edge
                tiles[i].left = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].right = &tiles[i+1];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].bottomRight = &tiles[i-width+1];
                tiles[i].top = &tiles[i-width];
                tiles[i].bottom = &tiles[i+width];

            } else if (i % width == width-1) { //right edge
                tiles[i].right = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].top = &tiles[i-width];

            } else if (i > ((width-1) * height)) { //bottom edge
                tiles[i].bottom = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
            } else {
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
            }

        } //link all tiles to each other

        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                tiles[i].isMine = true;
            }
        }//set mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].topLeft && tiles[i].topLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].topRight && tiles[i].topRight->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].top && tiles[i].top->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].left && tiles[i].left->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].right && tiles[i].right->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomLeft && tiles[i].bottomLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottom && tiles[i].bottom->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomRight && tiles[i].bottomRight->isMine) {
                tiles[i].neighborMines++;
            }
        } //check num of neighboring mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].isMine) {
                tiles[i].foreground.setTexture(textures["mine"]);
            }

        } //set foreground textures to mines

        //digits
        Tile digits;
        //digits.background.setTexture(textures["digits"]);
        //digits.background.setPosition(32*(width-2)/width, 32*height);
        digits.left = nullptr;
        digits.topLeft = nullptr;
        digits.top = nullptr;
        digits.topRight = nullptr;
        digits.right = nullptr;
        digits.bottomRight = nullptr;
        digits.bottom = nullptr;
        digits.bottomLeft = nullptr;
        bottomButtons.push_back(digits);

        //happy face
        Tile newTile;
        newTile.background.setTexture(textures["facehappy"]);
        newTile.background.setPosition(32*(width-2)/2, 32*height);
        newTile.left = nullptr;
        newTile.topLeft = nullptr;
        newTile.top = nullptr;
        newTile.topRight = nullptr;
        newTile.right = nullptr;
        newTile.bottomRight = nullptr;
        newTile.bottom = nullptr;
        newTile.bottomLeft = nullptr;
        bottomButtons.push_back(newTile);

        //debug
        Tile debug;
        debug.background.setTexture(textures["debug"]);
        debug.background.setPosition(32*(width-2)/2 + 128, 32*height);
        debug.left = nullptr;
        debug.topLeft = nullptr;
        debug.top = nullptr;
        debug.topRight = nullptr;
        debug.right = nullptr;
        debug.bottomRight = nullptr;
        debug.bottom = nullptr;
        debug.bottomLeft = nullptr;
        bottomButtons.push_back(debug);

        //test1
        Tile test1;
        test1.background.setTexture(textures["test1"]);
        test1.background.setPosition(32*(width-2)/2 + 192, 32*height);
        test1.left = nullptr;
        test1.topLeft = nullptr;
        test1.top = nullptr;
        test1.topRight = nullptr;
        test1.right = nullptr;
        test1.bottomRight = nullptr;
        test1.bottom = nullptr;
        test1.bottomLeft = nullptr;
        bottomButtons.push_back(test1);

        //test2
        Tile test2;
        test2.background.setTexture(textures["test2"]);
        test2.background.setPosition(32*(width-2)/2 + 256, 32*height);
        test2.left = nullptr;
        test2.topLeft = nullptr;
        test2.top = nullptr;
        test2.topRight = nullptr;
        test2.right = nullptr;
        test2.bottomRight = nullptr;
        test2.bottom = nullptr;
        test2.bottomLeft = nullptr;
        bottomButtons.push_back(test2);

        //test3
        Tile test3;
        test3.background.setTexture(textures["test3"]);
        test3.background.setPosition(32*(width-2)/2 + 320, 32*height);
        test3.left = nullptr;
        test3.topLeft = nullptr;
        test3.top = nullptr;
        test3.topRight = nullptr;
        test3.right = nullptr;
        test3.bottomRight = nullptr;
        test3.bottom = nullptr;
        test3.bottomLeft = nullptr;
        bottomButtons.push_back(test3);

    }

    void testBoard2() {
        tiles.clear();
        bottomButtons.clear();
        ifstream file("boards/testboard2.brd");
        string stream;
        int count = 0;
        int m = 0;
        vector<char> mineField;
        while (getline(file, stream)) {
            width = stream.size();
            count++;
            for (int i = 0; i < stream.size(); i++) {
                mineField.push_back(stream[i]);
            }
        }
        tiles.clear();
        height = count;
        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                m++;
            }
        }
        gameOver = false;
        gameWon = false;
        hiddenTiles = width*height-m;

        int numMines = m;
        flagMineCounter = m;

        loadTextures();

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                Tile newTile;
                newTile.background.setTexture(textures["tilehidden"]);
                newTile.extra.setTexture(textures["flag"]);
                newTile.background.setPosition(32*j, 32*i);
                newTile.foreground.setPosition(32*j, 32*i);
                newTile.extra.setPosition(32*j, 32*i);
                tiles.push_back(newTile);
            } //add tiles to the board

        for (int i = 0; i < tiles.size(); i++) {

            if (i == 0) { //top left
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width-1) { //top right
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i == (width-1)*height) { //bottom left
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width*height-1) { //bottom right
                tiles[i].top = &tiles[i-width];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i < width) { //top edge
                tiles[i].top = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];

            } else if (i % width == 0) { //left edge
                tiles[i].left = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].right = &tiles[i+1];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].bottomRight = &tiles[i-width+1];
                tiles[i].top = &tiles[i-width];
                tiles[i].bottom = &tiles[i+width];

            } else if (i % width == width-1) { //right edge
                tiles[i].right = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].top = &tiles[i-width];

            } else if (i > ((width-1) * height)) { //bottom edge
                tiles[i].bottom = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
            } else {
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
            }

        } //link all tiles to each other

        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                tiles[i].isMine = true;
            }
        } //set mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].topLeft && tiles[i].topLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].topRight && tiles[i].topRight->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].top && tiles[i].top->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].left && tiles[i].left->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].right && tiles[i].right->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomLeft && tiles[i].bottomLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottom && tiles[i].bottom->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomRight && tiles[i].bottomRight->isMine) {
                tiles[i].neighborMines++;
            }
        } //check num of neighboring mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].isMine) {
                tiles[i].foreground.setTexture(textures["mine"]);
            }

        } //set foreground textures to mines

        //digits
        Tile digits;
        //digits.background.setTexture(textures["digits"]);
        //digits.background.setPosition(32*(width-2)/width, 32*height);
        digits.left = nullptr;
        digits.topLeft = nullptr;
        digits.top = nullptr;
        digits.topRight = nullptr;
        digits.right = nullptr;
        digits.bottomRight = nullptr;
        digits.bottom = nullptr;
        digits.bottomLeft = nullptr;
        bottomButtons.push_back(digits);

        //happy face
        Tile newTile;
        newTile.background.setTexture(textures["facehappy"]);
        newTile.background.setPosition(32*(width-2)/2, 32*height);
        newTile.left = nullptr;
        newTile.topLeft = nullptr;
        newTile.top = nullptr;
        newTile.topRight = nullptr;
        newTile.right = nullptr;
        newTile.bottomRight = nullptr;
        newTile.bottom = nullptr;
        newTile.bottomLeft = nullptr;
        bottomButtons.push_back(newTile);

        //debug
        Tile debug;
        debug.background.setTexture(textures["debug"]);
        debug.background.setPosition(32*(width-2)/2 + 128, 32*height);
        debug.left = nullptr;
        debug.topLeft = nullptr;
        debug.top = nullptr;
        debug.topRight = nullptr;
        debug.right = nullptr;
        debug.bottomRight = nullptr;
        debug.bottom = nullptr;
        debug.bottomLeft = nullptr;
        bottomButtons.push_back(debug);

        //test1
        Tile test1;
        test1.background.setTexture(textures["test1"]);
        test1.background.setPosition(32*(width-2)/2 + 192, 32*height);
        test1.left = nullptr;
        test1.topLeft = nullptr;
        test1.top = nullptr;
        test1.topRight = nullptr;
        test1.right = nullptr;
        test1.bottomRight = nullptr;
        test1.bottom = nullptr;
        test1.bottomLeft = nullptr;
        bottomButtons.push_back(test1);

        //test2
        Tile test2;
        test2.background.setTexture(textures["test2"]);
        test2.background.setPosition(32*(width-2)/2 + 256, 32*height);
        test2.left = nullptr;
        test2.topLeft = nullptr;
        test2.top = nullptr;
        test2.topRight = nullptr;
        test2.right = nullptr;
        test2.bottomRight = nullptr;
        test2.bottom = nullptr;
        test2.bottomLeft = nullptr;
        bottomButtons.push_back(test2);

        //test3
        Tile test3;
        test3.background.setTexture(textures["test3"]);
        test3.background.setPosition(32*(width-2)/2 + 320, 32*height);
        test3.left = nullptr;
        test3.topLeft = nullptr;
        test3.top = nullptr;
        test3.topRight = nullptr;
        test3.right = nullptr;
        test3.bottomRight = nullptr;
        test3.bottom = nullptr;
        test3.bottomLeft = nullptr;
        bottomButtons.push_back(test3);

    }

    void testBoard3() {
        tiles.clear();
        bottomButtons.clear();

        ifstream file("boards/testboard3.brd");
        string stream;
        int count = 0;
        int m = 0;
        vector<char> mineField;
        while (getline(file, stream)) {
            width = stream.size();
            count++;
            for (int i = 0; i < stream.size(); i++) {
                mineField.push_back(stream[i]);
            }
        }
        tiles.clear();
        height = count;
        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                m++;
            }
        }
        gameOver = false;
        gameWon = false;
        hiddenTiles = width*height-m;

        int numMines = m;
        flagMineCounter = m;

        loadTextures();

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                Tile newTile;
                newTile.background.setTexture(textures["tilehidden"]);
                newTile.extra.setTexture(textures["flag"]);
                newTile.background.setPosition(32*j, 32*i);
                newTile.foreground.setPosition(32*j, 32*i);
                newTile.extra.setPosition(32*j, 32*i);
                tiles.push_back(newTile);
            } //add tiles to the board

        for (int i = 0; i < tiles.size(); i++) {

            if (i == 0) { //top left
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width-1) { //top right
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i == (width-1)*height) { //bottom left
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].right = &tiles[i+1];
            } else if (i == width*height-1) { //bottom right
                tiles[i].top = &tiles[i-width];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
            } else if (i < width) { //top edge
                tiles[i].top = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];

            } else if (i % width == 0) { //left edge
                tiles[i].left = nullptr;
                tiles[i].topLeft = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].right = &tiles[i+1];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].bottomRight = &tiles[i-width+1];
                tiles[i].top = &tiles[i-width];
                tiles[i].bottom = &tiles[i+width];

            } else if (i % width == width-1) { //right edge
                tiles[i].right = nullptr;
                tiles[i].topRight = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].left = &tiles[i-1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].top = &tiles[i-width];

            } else if (i > ((width-1) * height)) { //bottom edge
                tiles[i].bottom = nullptr;
                tiles[i].bottomLeft = nullptr;
                tiles[i].bottomRight = nullptr;
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
            } else {
                tiles[i].topLeft = &tiles[i-width-1];
                tiles[i].top = &tiles[i-width];
                tiles[i].topRight = &tiles[i-width+1];
                tiles[i].left = &tiles[i-1];
                tiles[i].right = &tiles[i+1];
                tiles[i].bottomLeft = &tiles[i+width-1];
                tiles[i].bottom = &tiles[i+width];
                tiles[i].bottomRight = &tiles[i+width+1];
            }

        } //link all tiles to each other

        for (int i = 0; i < mineField.size(); i++) {
            if (mineField[i] == '1') {
                tiles[i].isMine = true;
            }
        }//set mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].topLeft && tiles[i].topLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].topRight && tiles[i].topRight->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].top && tiles[i].top->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].left && tiles[i].left->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].right && tiles[i].right->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomLeft && tiles[i].bottomLeft->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottom && tiles[i].bottom->isMine) {
                tiles[i].neighborMines++;
            }
            if (tiles[i].bottomRight && tiles[i].bottomRight->isMine) {
                tiles[i].neighborMines++;
            }
        } //check num of neighboring mines

        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].isMine) {
                tiles[i].foreground.setTexture(textures["mine"]);
            }

        } //set foreground textures to mines

        //digits
        Tile digits;
        //digits.background.setTexture(textures["digits"]);
        //digits.background.setPosition(32*(width-2)/width, 32*height);
        digits.left = nullptr;
        digits.topLeft = nullptr;
        digits.top = nullptr;
        digits.topRight = nullptr;
        digits.right = nullptr;
        digits.bottomRight = nullptr;
        digits.bottom = nullptr;
        digits.bottomLeft = nullptr;
        bottomButtons.push_back(digits);

        //happy face
        Tile newTile;
        newTile.background.setTexture(textures["facehappy"]);
        newTile.background.setPosition(32*(width-2)/2, 32*height);
        newTile.left = nullptr;
        newTile.topLeft = nullptr;
        newTile.top = nullptr;
        newTile.topRight = nullptr;
        newTile.right = nullptr;
        newTile.bottomRight = nullptr;
        newTile.bottom = nullptr;
        newTile.bottomLeft = nullptr;
        bottomButtons.push_back(newTile);

        //debug
        Tile debug;
        debug.background.setTexture(textures["debug"]);
        debug.background.setPosition(32*(width-2)/2 + 128, 32*height);
        debug.left = nullptr;
        debug.topLeft = nullptr;
        debug.top = nullptr;
        debug.topRight = nullptr;
        debug.right = nullptr;
        debug.bottomRight = nullptr;
        debug.bottom = nullptr;
        debug.bottomLeft = nullptr;
        bottomButtons.push_back(debug);

        //test1
        Tile test1;
        test1.background.setTexture(textures["test1"]);
        test1.background.setPosition(32*(width-2)/2 + 192, 32*height);
        test1.left = nullptr;
        test1.topLeft = nullptr;
        test1.top = nullptr;
        test1.topRight = nullptr;
        test1.right = nullptr;
        test1.bottomRight = nullptr;
        test1.bottom = nullptr;
        test1.bottomLeft = nullptr;
        bottomButtons.push_back(test1);

        //test2
        Tile test2;
        test2.background.setTexture(textures["test2"]);
        test2.background.setPosition(32*(width-2)/2 + 256, 32*height);
        test2.left = nullptr;
        test2.topLeft = nullptr;
        test2.top = nullptr;
        test2.topRight = nullptr;
        test2.right = nullptr;
        test2.bottomRight = nullptr;
        test2.bottom = nullptr;
        test2.bottomLeft = nullptr;
        bottomButtons.push_back(test2);

        //test3
        Tile test3;
        test3.background.setTexture(textures["test3"]);
        test3.background.setPosition(32*(width-2)/2 + 320, 32*height);
        test3.left = nullptr;
        test3.topLeft = nullptr;
        test3.top = nullptr;
        test3.topRight = nullptr;
        test3.right = nullptr;
        test3.bottomRight = nullptr;
        test3.bottom = nullptr;
        test3.bottomLeft = nullptr;
        bottomButtons.push_back(test3);

    }

    int findHiddenTiles() {
        int count = 0;
        for (int i = 0; i < tiles.size(); i++) {
            if (!tiles[i].clicked && !tiles[i].isMine) {
                count++;
            }
        }
        return count;
    }

    void revealTiles(Tile* t) {
        //hiddenTiles--;
        findHiddenTiles();
        t->clicked = true;
        t->background.setTexture(textures["tilerevealed"]);
        if (t->neighborMines > 0) {
            t->foreground.setTexture(textures[findTexture(t->neighborMines)]);
        }
            if (t->topLeft != nullptr) {
                if (t->topLeft->clicked || t->topLeft->isMine || t->topLeft->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->topLeft);
                }
            }

            if (t->top != nullptr) {
                if (t->top->clicked || t->top->isMine || t->top->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->top);
                }
            }

            if (t->topRight != nullptr) {
                if (t->topRight->clicked || t->topRight->isMine || t->topRight->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->topRight);
                }
            }

            if (t->right != nullptr) {
                if (t->right->clicked || t->right->isMine || t->right->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->right);
                }
            }

            if (t->bottomRight != nullptr) {
                if (t->bottomRight->clicked || t->bottomRight->isMine || t->bottomRight->flagged ||
                    t->neighborMines > 0) {
                } else {
                    revealTiles(t->bottomRight);
                }
            }

            if (t->bottom != nullptr) {
                if (t->bottom->clicked || t->bottom->isMine || t->bottom->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->bottom);
                }
            }

            if (t->bottomLeft != nullptr) {
                if (t->bottomLeft->clicked || t->bottomLeft->isMine || t->bottomLeft->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->bottomLeft);
                }
            }

            if (t->left != nullptr) {
                if (t->left->clicked || t->left->isMine || t->left->flagged || t->neighborMines > 0) {
                } else {
                    revealTiles(t->left);
                }
            }
    }

    void setFlagCounter() {
        counterSprites.clear();
        string number = "" + flagMineCounter;
        bool isNeg = false;
        int counter = flagMineCounter;
        int x = 100;
        int y = 32*height;

        if (flagMineCounter < 0) {
            isNeg = true;
            counter *= -1;
        }
        int currentDigit;
        while (counter > 0) {
            currentDigit = counter % 10;
            sf::Sprite digit;
            digit.setTexture(textures["digits"]);
            if (currentDigit == 0) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21, 0, 21, 32));
            } else if (currentDigit == 1) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 21, 0, 21, 32));
            } else if (currentDigit == 2) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 42, 0, 21, 32));
            } else if (currentDigit == 3) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 63, 0, 21, 32));
            } else if (currentDigit == 4) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 84, 0, 21, 32));
            } else if (currentDigit == 5) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 105, 0, 21, 32));
            } else if (currentDigit == 6) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 126, 0, 21, 32));
            } else if (currentDigit == 7) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 147, 0, 21, 32));
            } else if (currentDigit == 8) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 168, 0, 21, 32));
            } else if (currentDigit == 9) {
                digit.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 189, 0, 21, 32));
            }

            string curr = "" + currentDigit;
            x -= 21;
            digit.setPosition(x, y);
            counterSprites.push_back(digit);
            counter /= 10;
        }
        if (flagMineCounter == 0) {
            sf::Sprite zero;
            zero.setTexture(textures["digits"]);
            zero.setTextureRect(sf::IntRect((flagMineCounter/100)*21, 0, 21, 32));
            zero.setPosition(79, y);
            counterSprites.push_back(zero);
        }
        if (isNeg) {
            sf::Sprite negSign;
            negSign.setTexture(textures["digits"]);
            negSign.setTextureRect(sf::IntRect((flagMineCounter/100)*21 + 210, 0, 21, 32));
            //negSign = digits["0"];
            x -= 21;
            negSign.setPosition(x, y);
            counterSprites.push_back(negSign);
        }


    }

    string findTexture(int n) {
        if (n == 1) {
            return "number1";
        }
        if (n == 2) {
            return "number2";
        }
        if (n == 3) {
            return "number3";
        }
        if (n == 4) {
            return "number4";
        }
        if (n == 5) {
            return "number5";
        }
        if (n == 6) {
            return "number6";
        }
        if (n == 7) {
            return "number7";
        }
        if (n == 8) {
            return "number8";
        }
    }

    void loadTextures() {

        sf::Texture temp;
        temp.loadFromFile("images/test_3.png");
        textures.emplace("test3", temp);

        temp.loadFromFile("images/digits.png");
        textures.emplace("digits", temp);

        sf::Sprite sprite;
        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect((flagMineCounter/100)*21, 0, 21, 32));
        digits.emplace("0", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("1", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("2", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("3", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("4", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("5", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("6", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("7", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("8", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("9", sprite);

        sprite.setTexture(temp);
        sprite.setTextureRect(sf::IntRect(20, 0, 21, 32));
        digits.emplace("-", sprite);

        temp.loadFromFile("images/test_1.png");
        textures.emplace("test1", temp);

        temp.loadFromFile("images/test_2.png");
        textures.emplace("test2", temp);

        temp.loadFromFile("images/debug.png");
        textures.emplace("debug", temp);

        temp.loadFromFile("images/face_lose.png");
        textures.emplace("facelose", temp);

        temp.loadFromFile("images/face_happy.png");
        textures.emplace("facehappy", temp);

        temp.loadFromFile("images/face_win.png");
        textures.emplace("facewin", temp);

        temp.loadFromFile("images/tile_revealed.png");
        textures.emplace("tilerevealed", temp);

        temp.loadFromFile("images/tile_hidden.png");
        textures.emplace("tilehidden", temp);

        temp.loadFromFile("images/mine.png");
        textures.emplace("mine", temp);

        temp.loadFromFile("images/flag.png");
        textures.emplace("flag", temp);

        temp.loadFromFile("images/number_1.png");
        textures.emplace("number1", temp);

        temp.loadFromFile("images/number_2.png");
        textures.emplace("number2", temp);

        temp.loadFromFile("images/number_3.png");
        textures.emplace("number3", temp);

        temp.loadFromFile("images/number_4.png");
        textures.emplace("number4", temp);

        temp.loadFromFile("images/number_5.png");
        textures.emplace("number5", temp);

        temp.loadFromFile("images/number_6.png");
        textures.emplace("number6", temp);

        temp.loadFromFile("images/number_7.png");
        textures.emplace("number7", temp);

        temp.loadFromFile("images/number_8.png");
        textures.emplace("number8", temp);
    } //load textures from images


};
