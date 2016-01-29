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
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    #include <conio.h>
	#include <windows.h>
	#define OS_WINDOWS
#else
	#include <unistd.h>
	#include <iostream>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

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
#define TEXTURE_WATER_UP_LEFT       "WaterLU"
#define TEXTURE_WATER_LEFT_RIGHT    "WaterLR"
#define TEXTURE_WATER_LEFT_DOWN     "WaterDL"
#define TEXTURE_WATER_RIGHT_DOWN    "WaterRD"
#define TEXTURE_EMPTY_TILE          "EmptyTile"
#define TEXTURE_OBSTACLE            "Obstacle"
#define TEXTURE_WARP                "Warp"
#define TEXTURE_EGG                 "Egg"
#define TEXTURE_EXPLOSION           "Explosion"
#define FILETYPE                    ".png"
#define IDENTIFIER_WATER_UP_DOWN    '|'
#define IDENTIFIER_WATER_UP_RIGHT   '9'
#define IDENTIFIER_WATER_UP_LEFT    '7'
#define IDENTIFIER_WATER_LEFT_RIGHT '-'
#define IDENTIFIER_WATER_LEFT_DOWN  '1'
#define IDENTIFIER_WATER_RIGHT_DOWN '3'
#define IDENTIFIER_EMPTY_TILE       '.'
#define IDENTIFIER_WARP             'x'
#define IDENTIFIER_OBSTACLE         'o'

/**
 * @class Game Game.hpp
 * @brief Defines the Game Class
 */
class Game{

public:

	Game();
	Game(std::string path1, std::string path2, std::string path3);
	~Game();
	void launch(sf::RenderWindow& game_window, std::string map_name);

private:

	void getMapFile();
	bool loadMap();
	void start(sf::RenderWindow& game_window);
	bool someoneWon();
	std::vector<sf::Keyboard::Key> loadKeys(std::string selected_player);
	void printExplosion(sf::RenderWindow& game_window, Coord coord);
	void pauseGame(sf::RenderWindow& game_window, bool player_request);

	Map game_map;
	std::vector<Duck> player;
	std::vector<Coord> player_spawn;
	std::vector<Direction> player_initial_dir;
	unsigned char winner;
	std::vector<sf::Texture**> duck_texture;
	sf::Texture map_texture[9];
	sf::Texture egg_texture, explosion_texture;
	sf::Sprite explosion_sprite;
	unsigned int pxl_length, pxl_height;
	std::string path, biome_path, ducks_path, map_path;
	unsigned char player_number;
	unsigned int game_speed;
	unsigned short egg_victory;
};

#endif /* GAME_HPP_INCLUDED */
