/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>

void Game::launch(){

	std::string duck_textures_path[4] = {
		TEXTURE_DUCK_UP,
		TEXTURE_DUCK_DOWN,
		TEXTURE_DUCK_LEFT,
		TEXTURE_DUCK_RIGHT
	};
	std::string ducky_textures_path[4] = {
		TEXTURE_DUCKY_UP,
		TEXTURE_DUCKY_DOWN,
		TEXTURE_DUCKY_LEFT,
		TEXTURE_DUCKY_RIGHT
	};
	std::string map_textures_path[7] = {
		TEXTURE_OBSTACLE,
		TEXTURE_EMPTY_TILE,
		TEXTURE_WATER_UP_RIGHT,
		TEXTURE_WATER_RIGHT_DOWN,
		TEXTURE_WATER_LEFT_DOWN,
		TEXTURE_WATER_UP_DOWN,
		TEXTURE_WATER_LEFT_DOWN
	};
	bool loading_success = egg_texture.loadFromFile(path + TEXTURE_EGG + FILETYPE);
	char player_id;

	for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
		player_id = static_cast<char>(i + '0');
		for (unsigned int j = 4 ; j-- ;) {
			loading_success = loading_success && duck_texture[i][0][j].loadFromFile(path + duck_textures_path[j] + player_id + FILETYPE);
			loading_success = loading_success && duck_texture[i][1][j].loadFromFile(path + ducky_textures_path[j] + player_id + FILETYPE);
		}
	}
	for (unsigned char i = 7 ; i-- ;) {
		loading_success = loading_success && map_texture[i].loadFromFile(path + map_textures_path[i] + FILETYPE);
	}

	if (!loading_success || !(this->loadMap())) {
		std::cout << "Failed to load game ressources" << std::endl;
	}
	else {
		winner = 0;
		game_window.create(sf::VideoMode(pxl_length, pxl_height), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);
		for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
			player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i]);
		}

		this->start();
	}
}

bool Game::loadMap(){
	std::ifstream map_file(path + MAPFILE, std::ios::in | std::ios::binary);
	unsigned int x_map_size, y_map_size;
	std::string value;

	if (map_file) {
		map_file >> x_map_size;
		map_file >> y_map_size;
		pxl_height = 32*y_map_size;
		pxl_length = 32*x_map_size;
		for (unsigned int i = 0 ; i < PLAYER_NUMBER ; ++i) {
			map_file >> player_spawn[i].x;
			map_file >> player_spawn[i].y;
			map_file >> value;
			if (value == "UP")
				player_initial_dir[i] = UP;
			else if (value == "DOWN")
				player_initial_dir[i] = DOWN;
			else if (value == "LEFT")
				player_initial_dir[i] = LEFT;
			else if (value == "RIGHT")
				player_initial_dir[i] = RIGHT;
			else{
				std::cout << "Not a valid direction : " << value << std::endl;
				std::cout << "(On player " << i << ")" << std::endl;
			}
		}
		std::vector< std::vector<Area> > map_interpreted;
		for (unsigned int i = 0 ; i < x_map_size ; ++i) {
			map_interpreted.push_back(std::vector<Area>());
			map_file >> value;
			for (unsigned int j = 0 ; j < y_map_size ; ++j) {
				switch(value[j])
				{
					case IDENTIFIER_EMPTY_TILE:
						map_interpreted[i].push_back(GRASS);
						break;
					case IDENTIFIER_OBSTACLE:
						map_interpreted[i].push_back(ROCK);
						break;
					case IDENTIFIER_WATER_LEFT_DOWN:
						map_interpreted[i].push_back(WATER_DL);
						break;
					case IDENTIFIER_WATER_LEFT_RIGHT:
						map_interpreted[i].push_back(WATER_LR);
						break;
					case IDENTIFIER_WATER_RIGHT_DOWN:
						map_interpreted[i].push_back(WATER_RD);
						break;
					case IDENTIFIER_WATER_UP_DOWN:
						map_interpreted[i].push_back(WATER_UD);
						break;
					case IDENTIFIER_WATER_UP_LEFT:
						map_interpreted[i].push_back(WATER_LU);
						break;
					case IDENTIFIER_WATER_UP_RIGHT:
						map_interpreted[i].push_back(WATER_UR);
						break;
					default:
						std::cout << "Bad map" << std::endl;
						return false;
						break;
				}
			}
		}

		game_map = Map(x_map_size, y_map_size, map_interpreted, map_texture, egg_texture);
		return true;
	}
	else
		return false;
}

void Game::start()
{
	std::vector<std::vector<sf::Keyboard::Key>> players_keys;
	players_keys.push_back(new std::vector<sf::Keyboard::Key>);
	players_keys[0].push_back(sf::Keyboard::Up);
	players_keys[0].push_back(sf::Keyboard::Down);
	players_keys[0].push_back(sf::Keyboard::Left);
	players_keys[0].push_back(sf::Keyboard::Right);
	players_keys.push_back(new std::vector<sf::Keyboard::Key>);
	players_keys[1].push_back(sf::Keyboard::Z);
	players_keys[1].push_back(sf::Keyboard::S);
	players_keys[1].push_back(sf::Keyboard::Q);
	players_keys[1].push_back(sf::Keyboard::D);
	sf::Event event;
	while (game_window.isOpen())
	{
		game_window.clear();
		while (game_window.pollEvent(event))
		{
			switch(event.type){

				case sf::Event::Closed:

					game_window.close();
					break;

				case sf::Event::KeyPressed:
					for(unsigned char i = PLAYER_NUMBER; i--;){
						if(sf::Keyboard::isKeyPressed(players_keys[i][1]))
						    player[i].setDirection(UP);
						else if(sf::Keyboard::isKeyPressed(players_keys[i][2]))
						    player[i].setDirection(DOWN);
						else if(sf::Keyboard::isKeyPressed(players_keys[i][3]))
						    player[i].setDirection(LEFT);
						else if(sf::Keyboard::isKeyPressed(players_keys[i][4]))
						    player[i].setDirection(RIGHT);
					}
					break;

				default:
					break;
			}
		}

		sf::sleep(sf::milliseconds(100));
		game_map.printAll(game_window);
		game_window.display();
	}
}
