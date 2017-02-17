/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef NETWORKGAME_HPP_INCLUDED
#define NETWORKGAME_HPP_INCLUDED

/**
 * @file NetworkGame.hpp
 * @author Lucas LAZARE, Julien BARBIER
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 * @brief Defis both GameServer and GameClient classes
 */

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

#define SQUAREINNERCOLOR                sf::Color(255,12,125,100)
#define SQUAREOUTERCOLOR                sf::Color::Yellow
#define DUCK_PATH                       std::string("ducks/")
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

#define PORT                            7482
#define CLIENT_CONNECTION_ATTEMPT_NBR   31    // How many time should the client look for a server ?
#define CLIENT_CONNECTION_REFRESH_RATE  sf::milliseconds(200)     // At which rate should the client look for a server ?
#define CLIENT_CONNECTION_TIMEOUT       sf::milliseconds(300)

/**
 * @class GameServer NetworkGame.hpp
 * @brief Defines the GameServer class
 * @details This class is used to host a network game
 */
class GameServer {

public:

	/**
	 * @brief GameServer destructor
	 */
	~GameServer( );

	/**
	 * @brief GameServer constructor
	 * @param ressources_path Path to the ressources (from the executable root)
	 * @param biome_path_      Path to the biome (from the executable root)
	 * @param map_file_name   Path to the map (from the executable root)
	 */
	GameServer(std::string ressources_path, std::string biome_path_, std::string map_file_name);

	/**
	 * @brief Launches a Network Game
	 */
	void launch(sf::RenderWindow& game_window);

	/**
	 * @brief Listens for clients broadcasting the network
	 * @param window An sf::RenderWindow
	 * @return true if all clients joined, false otherwise
	 */
	bool getClients(sf::RenderWindow& window);

private:

	/**
	 * @brief Starts a Network Game
	 */
	void start(sf::RenderWindow& game_window);

	/**
	 * @brief
	 * @param selected_player
	 * @return
	 */
	std::vector<sf::Keyboard::Key> loadKeys(std::string selected_player);

	std::vector<sf::TcpSocket*> clients;
	Map game_map;
	std::vector<Duck> player;
	std::vector<Coord> player_spawn;
	std::vector<Direction> player_initial_dir;
	std::string ressources, biome_path;
	std::vector<sf::Texture**> duck_texture;
	sf::Texture map_texture[9];
	sf::Texture egg_texture;
	sf::Texture explosion_texture;
	unsigned int game_speed;
	unsigned short egg_victory;
	unsigned int map_height, map_width;
	unsigned short los;
	bool is_blind, loop;
};


class GameClient {

public:

	~GameClient( );

	GameClient(std::string bp, bool is_local) {
		biome_path = bp;
		local = is_local;
	}

	void launch(sf::RenderWindow& game_window);

private :

	void start(sf::RenderWindow& game_window);

	std::vector<sf::Keyboard::Key> loadKeys(std::string selected_player);

	unsigned int map_height, map_width, game_speed;
	std::vector<Direction> player_initial_dir;
	Direction direction;
	bool local;
	std::vector<Duck> player;
	sf::TcpSocket server;
	Map game_map;
	std::vector<sf::Texture**> duck_texture;
	sf::Texture map_texture[9];
	sf::Texture egg_texture;
	sf::Texture explosion_texture;
	sf::Sprite explosion_sprite;
	std::string biome_path;
	unsigned short los;
	bool is_blind, loop;
};

#endif /* NETWORKGAME_HPP_INCLUDED */
