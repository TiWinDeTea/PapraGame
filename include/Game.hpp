/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

/**
 * @file Game.hpp
 * @author Lucas LAZARE
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 * @brief Defines the Game Class
 */

#include <Duck.hpp>
#include <Map.hpp>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>

#define PLAYER_NUMBER               2
#define TEXTURE_DUCK_LEFT           "DuckL_"
#define TEXTURE_DUCK_RIGHT          "DuckR_"
#define TEXTURE_DUCK_UP             "DuckU_"
#define TEXTURE_DUCK_DOWN           "DuckD_"
#define TEXTURE_DUCKY_LEFT          "DuckyL_"
#define TEXTURE_DUCKY_RIGHT         "DuckyR_"
#define TEXTURE_DUCKY_UP            "DuckyU_"
#define TEXTURE_DUCKY_DOWN          "DuckyD_"
#define TEXTURE_WATER_UP_DOWN       "WaterUD"
#define TEXTURE_WATER_UP_RIGHT      "WaterUR"
#define TEXTURE_WATER_UP_LEFT       "WaterUL"
#define TEXTURE_WATER_LEFT_RIGHT    "WaterLR"
#define TEXTURE_WATER_LEFT_DOWN     "WaterDL"
#define TEXTURE_WATER_RIGHT_DOWN    "WaterRD"
#define TEXTURE_EMPTY_TILE          "Grass"
#define TEXTURE_OBSTACLE            "Rock"
#define TEXTURE_EGG                 "Egg"
#define FILETYPE                    ".png"
#define MAPFILE                     "map"
#define IDENTIFIER_WATER_UP_DOWN    '|'
#define IDENTIFIER_WATER_UP_RIGHT   '9'
#define IDENTIFIER_WATER_UP_LEFT    '7'
#define IDENTIFIER_WATER_LEFT_RIGHT '-'
#define IDENTIFIER_WATER_LEFT_DOWN  '1'
#define IDENTIFIER_WATER_RIGHT_DOWN '3'
#define IDENTIFIER_EMPTY_TILE       '.'
#define IDENTIFIER_OBSTACLE         'o'

/**
 * @class Game Game.hpp
 * @brief Defines the Game Class
 */
class Game{

public:

	~Game(){};
	Game(){path = "res/";}
	Game(std::string ressources_path){path = ressources_path;}

	void launch();

private:

	bool loadMap();
	void start();
	bool someoneWon();

	Map game_map;
	Duck player[PLAYER_NUMBER];
	Coord player_spawn[PLAYER_NUMBER];
	Direction player_initial_dir[PLAYER_NUMBER];
	unsigned char winner;
	sf::Texture duck_texture[PLAYER_NUMBER][2][4];
	sf::Texture map_texture[8];
	sf::Texture egg_texture;
	sf::RenderWindow game_window;
	unsigned int pxl_length, pxl_height;
	std::string path;
};

#endif /* GAME_HPP_INCLUDED */
