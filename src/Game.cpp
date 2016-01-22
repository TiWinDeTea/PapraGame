/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>

void Game::launch(){

	std::string duck_textures_path[4] = {
		ducks_path + TEXTURE_DUCK_UP,
		ducks_path + TEXTURE_DUCK_DOWN,
		ducks_path + TEXTURE_DUCK_LEFT,
		ducks_path + TEXTURE_DUCK_RIGHT
	};
	std::string ducky_textures_path[4] = {
		ducks_path + TEXTURE_DUCKY_UP,
		ducks_path + TEXTURE_DUCKY_DOWN,
		ducks_path + TEXTURE_DUCKY_LEFT,
		ducks_path + TEXTURE_DUCKY_RIGHT
	};
	std::string map_textures_path[7] = {
		biome_path + TEXTURE_OBSTACLE,
		biome_path + TEXTURE_EMPTY_TILE,
		biome_path + TEXTURE_WATER_UP_RIGHT,
		biome_path + TEXTURE_WATER_RIGHT_DOWN,
		biome_path + TEXTURE_WATER_LEFT_DOWN,
		biome_path + TEXTURE_WATER_UP_DOWN,
		biome_path + TEXTURE_WATER_LEFT_DOWN
	};
	bool loading_success = egg_texture.loadFromFile(path + ducks_path + TEXTURE_EGG + FILETYPE);
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

		game_map = Map(x_map_size, y_map_size, map_interpreted, map_texture, &egg_texture);
		return true;
	}
	else
		return false;
}

void Game::start()
{
	unsigned int tmp(0);
	std::vector< std::vector<sf::Keyboard::Key> > player_keys;
	player_keys.push_back(std::vector<sf::Keyboard::Key>());
	player_keys[0].push_back(sf::Keyboard::Up);
	player_keys[0].push_back(sf::Keyboard::Down);
	player_keys[0].push_back(sf::Keyboard::Left);
	player_keys[0].push_back(sf::Keyboard::Right);
	player_keys.push_back(std::vector<sf::Keyboard::Key>());
	player_keys[1].push_back(sf::Keyboard::Z);
	player_keys[1].push_back(sf::Keyboard::S);
	player_keys[1].push_back(sf::Keyboard::Q);
	player_keys[1].push_back(sf::Keyboard::D);
	std::vector<Direction> player_dir;
	for(unsigned char i = PLAYER_NUMBER; i--;)
		player_dir.push_back(player[i].getDirection());

	Coord egg_coo;
	egg_coo.x = 5;
	egg_coo.y = 7;
	sf::Event event;
	game_map.popEgg(egg_coo, game_window);

	game_window.display();
	while (game_window.isOpen())
	{
		game_window.clear();
		game_map.print(game_window);
		while (game_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game_window.close();
			else if(event.type == sf::Event::KeyPressed){
				for(unsigned char i = PLAYER_NUMBER; i--;){
					if(sf::Keyboard::isKeyPressed(player_keys[i][0]))
					    player_dir[i] = UP;
					else if(sf::Keyboard::isKeyPressed(player_keys[i][1]))
					    player_dir[i] = DOWN;
					else if(sf::Keyboard::isKeyPressed(player_keys[i][2]))
					    player_dir[i] = LEFT;
					else if(sf::Keyboard::isKeyPressed(player_keys[i][3]))
					    player_dir[i] = RIGHT;
				}
			}
		}
		sf::sleep(sf::milliseconds(50));
		++tmp;

		if(tmp > 20){
			tmp = 0;
			for(unsigned char i = PLAYER_NUMBER; i--;)
				player[i].move(game_window, player_dir[i]);

			game_window.display();
		}
	}
}
