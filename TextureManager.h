#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace std;

class TextureManager {
    static unordered_map<string, sf::Texture> textures;

public:
    static sf::Texture& getTexture(string textureName);
};

class Tile{
public:
    bool mines = false;
    bool flag = false;
    bool revealedTile = false;
    int neighboringMines = 0; // size of this are next to the tiles, find a
};
// if object.mines == false then;