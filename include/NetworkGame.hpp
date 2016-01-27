/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef NETWORKGAME_HPP_INCLUDED
#define NETWORKGAME_HPP_INCLUDED

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

#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class NetworkGame{

public:

	NetworkGame(Game& loaded_game){ game = loaded_game; isServer = true;}
	NetworkGame(){bool isServer = false;}
	void launch();

private:
	bool isServer;
	Game game;
};


class GameClient{
	
	public class : 
		
		~GameClient();
		GameClient(bool is_local = false){local = is_local; ducks_path = "ducks/";}
		GameClient(std::string bp="nope", bool is_local = false){biome_path = bp; local = is_local; ducks_path = "ducks/";}
		void launch(sf::RenderWindow& game_window);
		
	private class :		
		
		void start(sf::RenderWindow& game_window);
		
		bool local;
		std::vector<Duck> player;
		sf::TcpSocket server;
		Map game_map;
		std::vector<sf::Texture**> duck_texture;
		sf::Texture texture[9];
		sf::Texture egg_texture;
		sf::Texture explosion_texture;
		std::string ducks_path, biome_path;
		
}                                                                                                    
        
#endif /* NETWORKGAME_HPP_INCLUDED */
