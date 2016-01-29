/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef NETWORKGAME_HPP_INCLUDED
#define NETWORKGAME_HPP_INCLUDED

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	#include <conio.h>
	#define OS_WINDOWS
#else
	#include <ctype.h>
	#include <termios.h>
#endif

#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
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

#define DUCK_PATH                   std::string("ducks/")
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

#define PORT                        7482

class GameServer{

public:

	~GameServer();
	GameServer(std::string ressources_path, std::string biome_path, std::string map_file_name);
	void launch();

private:

	void getClients(std::string biome_path, std::vector< std::vector<Area> > map_as_area);
	void start();
	char instantGetChar();

	std::vector<sf::TcpSocket*> clients;
	Map game_map;
	std::vector<Duck> player;
	std::vector<Coord> player_spawn;
	std::vector<Direction> player_initial_dir;
	unsigned char winner;
	unsigned char player_number;
	unsigned int game_speed;
	unsigned short egg_victory;
	unsigned int map_height, map_width;
};


class GameClient{

	public:

		~GameClient(){};
		GameClient(bool is_local = false){local = is_local;}
		GameClient(std::string bp="nope", bool is_local = false){local_biome_path = bp; local = is_local;}
		void launch(sf::RenderWindow& game_window);

	private :

		void start(sf::RenderWindow& game_window);
		std::vector<sf::Keyboard::Key> loadKeys(std::string selected_player);

		bool local;
		std::vector<Duck> player;
		sf::TcpSocket server;
		Map game_map;
		std::vector<sf::Texture**> duck_texture;
		sf::Texture map_texture[9];
		sf::Texture egg_texture;
		sf::Texture explosion_texture;
		sf::Sprite explosion_sprite;
		std::string local_biome_path;
};

#endif /* NETWORKGAME_HPP_INCLUDED */
