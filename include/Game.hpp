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
#include <SFML/Graphics>
#include <SFML/Window>
#include <iostream>

#define PLAYER_NUMBER              2
#define TEXTURE_DUCK_LEFT          "DuckL_"
#define TEXTURE_DUCK_RIGHT         "DuckR_"
#define TEXTURE_DUCK_UP            "DuckU_"
#define TEXTURE_DUCK_DOWN          "DuckD_"
#define TEXTURE_DUCKY_LEFT         "DuckyL_"
#define TEXTURE_DUCKY_RIGHT        "DuckyR_"
#define TEXTURE_DUCKY_UP           "DuckyU_"
#define TEXTURE_DUCKY_DOWN         "DuckyD_"
#define TEXTURE_WATER_UP_DOWN      "WaterUD"
#define TEXTURE_WATER_UP_RIGHT     "WaterUR"
#define TEXTURE_WATER_UP_LEFT      "WaterUL"
#define TEXTURE_WATER_LEFT_RIGHT   "WaterLR"
#define TEXTURE_WATER_LEFT_DOWN    "WaterLD"
#define TEXTURE_WATER_RIGHT_DOWN   "WaterRD"
#define TEXTURE_EMPTY_TILE         "Grass"
#define TEXTURE_OBSTACLE           "Rock"
#define TEXTURE_EGG                "Egg"
#define FILETYPE                   ".png"

/**
 * @class Game Game.hpp
 * @brief Defines the Game Class
 */
class Game{

public:

	~Game(){};
	Game(){ressources_path = "res/";}
	Game(std::String path){ressources_path = path;}

	void launch();

private:

	void start();
	bool someoneWon();

	Map game_map;
	Duck player[PLAYER_NUMBER];
	unsigned char winner;
	sf::Texture duck_texture[4][2][PLAYER_NUMBER];
	sf::Texture map_texture[8];
	sf::Texture egg_texture;
}

#endif /* GAME_HPP_INCLUDED */
