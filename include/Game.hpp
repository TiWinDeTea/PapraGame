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
#include <SFML/Audio.hpp>
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

#ifndef PAPRAGAME_PATHS_DEFINED
#define PAPRAGAME_PATHS_DEFINED
	#define TEXTURE_DUCK_LEFT           std::string("DuckL_")
	#define TEXTURE_DUCK_RIGHT          std::string("DuckR_")
	#define TEXTURE_DUCK_UP             std::string("DuckU_")
	#define TEXTURE_DUCK_DOWN           std::string("DuckD_")
	#define TEXTURE_DUCKY_LEFT          std::string("DuckyL_")
	#define TEXTURE_DUCKY_RIGHT         std::string("DuckyR_")
	#define TEXTURE_DUCKY_UP            std::string("DuckyU_")
	#define TEXTURE_DUCKY_DOWN          std::string("DuckyD_")
	#define TEXTURE_WATER_UP_DOWN       std::string("WaterUD")
	#define TEXTURE_WATER_UP_RIGHT      std::string("WaterUR")
	#define TEXTURE_WATER_UP_LEFT       std::string("WaterLU")
	#define TEXTURE_WATER_LEFT_RIGHT    std::string("WaterLR")
	#define TEXTURE_WATER_LEFT_DOWN     std::string("WaterDL")
	#define TEXTURE_WATER_RIGHT_DOWN    std::string("WaterRD")
	#define TEXTURE_EMPTY_TILE          std::string("EmptyTile")
	#define TEXTURE_OBSTACLE            std::string("Obstacle")
	#define TEXTURE_WARP                std::string("Warp")
	#define TEXTURE_EGG                 std::string("Egg")
	#define TEXTURE_EXPLOSION           std::string("Explosion")
 	#define TEXTURE_PAUSE_MENU          std::string("menu/PauseMenu")
 	#define TEXTURE_VICTORY             std::string("menu/VictoryMenu")
	#define FILETYPE                    std::string(".png")
	#define IDENTIFIER_WATER_UP_DOWN    '|'
	#define IDENTIFIER_WATER_UP_RIGHT   '9'
	#define IDENTIFIER_WATER_UP_LEFT    '7'
	#define IDENTIFIER_WATER_LEFT_RIGHT '-'
	#define IDENTIFIER_WATER_LEFT_DOWN  '1'
	#define IDENTIFIER_WATER_RIGHT_DOWN '3'
	#define IDENTIFIER_EMPTY_TILE       '.'
	#define IDENTIFIER_WARP             'x'
	#define IDENTIFIER_OBSTACLE         'o'
#endif /* PAPRAGAME_PATHS_DEFINED */

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

	bool loadMap();
	void start(sf::RenderWindow& game_window);
	std::vector<sf::Keyboard::Key> loadKeys(std::string selected_player);
	void printExplosion(sf::RenderWindow& game_window, Coord coord);
	bool pauseGame(sf::RenderWindow& game_window, bool player_request);
	void printVictory(sf::RenderWindow& game_window);

	Map game_map;
	std::vector<Duck> player;
	std::vector<Coord> player_spawn;
	std::vector<Direction> player_initial_dir;
	unsigned char winner;
	std::vector<sf::Texture**> duck_texture;
	sf::Texture map_texture[9];
	sf::Texture egg_texture, explosion_texture, pause_menu_texture, victory_texture;
	sf::Sprite explosion_sprite, pause_menu_sprite, victory_sprite;
	unsigned int pxl_length, pxl_height;
	std::string path, biome_path, ducks_path, map_path;
	unsigned char player_number;
	unsigned int game_speed;
	unsigned short egg_victory;
	unsigned short los;
	bool is_blind, loop;
};

#endif /* GAME_HPP_INCLUDED */
