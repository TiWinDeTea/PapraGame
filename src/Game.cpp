/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>

void Game::launch(){
	// Coordonnées initiales

	for (unsigned int i = 20 ; i-- ; ) {
		for (unsigned int j = 20 ; j--; ) {
			map_tiles[i][j] = ROCK;
		}
	}


	std::string duck_textures_path[4] = { TEXTURE_DUCK_UP, TEXTURE_DUCK_DOWN, TEXTURE_DUCK_LEFT, TEXTURE_DUCK_RIGHT };
	std::string ducky_textures_path[4] = {TEXTURE_DUCKY_UP, TEXTURE_DUCKY_DOWN, TEXTURE_DUCKY_LEFT, TEXTURE_DUCKY_RIGHT};
	std::string map_textures_path[8] = {
		TEXTURE_WATER_UP_DOWN,
		TEXTURE_WATER_UP_RIGHT,
		TEXTURE_WATER_UP_LEFT,
		TEXTURE_WATER_LEFT_RIGHT,
		TEXTURE_WATER_LEFT_DOWN,
		TEXTURE_WATER_RIGHT_DOWN,
		TEXTURE_EMPTY_TILE,
		TEXTURE_OBSTACLE };

	bool loading_success = true;
	char player_id;

	for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
		player_id = i + '0';
		for (unsigned int j = 4 ; j-- ;) {
			loading_success = loading_success && duck_texture[i][0][j].loadFromFile(ressources_path + duck_textures_path[j] + player_id + FILETYPE);
			loading_success = loading_success && duck_textures[i][1][j].loadFromFile(ressources_path + ducky_textures_path[j] + player_id + FILETYPE);
		}
	}
	for (unsigned char i = 8 ; i-- ;) {
		loading_success = loading_success && map_textures[i].loadFromFile(ressources_path + map_textures_path[i] + FILETYPE);
	}

	loading_success = loading_success && egg_texture.loadFromFile(ressources_path + TEXTURE_EGG + FILETYPE);

	if (!loading_success || !this.loadMap()) {
		std::cout << "Failed to load game ressources" << std::endl;
	}
	else {
		winner = 0;
		game_window.create(sf::VideoMode(coo.x, coo.y), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);
		for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
			player[i] = Duck(game_window, duck_texture[i][0], duck_texture[i][1], p[i], player_spawn[i], player_initial_dir[i]);
		}

		this.start();
	}
}

bool Game::initMap(){
	std::ifstream map_file(ressources_path + MAPFILE, std::ios::in | std::ios::binary);
	unsigned char x_map_size, y_map_size;
	std::string value;

	if (map_file) {
		std::getline(map_file, x_map_size);
		std::getline(map_file, y_map_size);
		for (unsigned int i = 0 ; i < PLAYER_NUMBER ; ++i) {
			std::getline(map_file, player_spawn[i].x);
			std::getline(map_file, player_spawn[i].y);
			std::getline(map_file, value);
			switch (value){
				case "UP":
					player_initial_dir[i] = UP;
					break;
				case "DOWN":
					player_initial_dir[i] = DOWN;
					break;
				case "LEFT":
					player_initial_dir[i] = LEFT;
					break;
				case "RIGHT":
					player_initial_dir[i] = RIGHT;
					break;
				default:
					std::cout << "Not a valid direction : " << value << std::endl;
					std::cout << "(On player " << i << ")" << std::endl;
			}
		}
		Area map_interpreted[x_map_size][y_map_size];
		for (unsigned int i = 0 ; i < y_map_size ; ++i) {
			std::getline(map_file, value);
			for (unsigned int j = 0 ; j < y_map_size ; ++j) {
				switch(value[j])
				{
					case IDENTIFIER_EMPTY_TILE:
						map_interpreted[j][i] = GRASS;
						break;
					case IDENTIFIER_OBSTACLE:
						map_interpreted[j][i] = ROCK;
						break;
					case IDENTIFIER_WATER_LEFT_DOWN:
						map_interpreted[j][i] = WATER_LD;
						break;
					case IDENTIFIER_WATER_LEFT_RIGHT:
						map_interpreted[j][i] = WATER_LR;
						break;
					case IDENTIFIER_WATER_RIGHT_DOWN:
						map_interpreted[j][i] = WATER_RD;
						break;
					case IDENTIFIER_WATER_UP_DOWN:
						map_interpreted[j][i] = WATER_UD;
						break;
					case IDENTIFIER_WATER_UP_LEFT:
						map_interpreted[j][i] = WATER_UL;
						break;
					case IDENTIFIER_WATER_UP_RIGHT:
						map_interpreted[j][i] = WATER_UR;
						break;
					default:
						std::cout << "Bad map" << std::endl;
						return false;
						break;
				}
			}
		}

		game_map = Map(x_size, y_size, map_interpreted, map_textures);
	}
	else
		return false;
}

void Game::start()
{
	sf::Event event;
	while (game_window.isOpen())
	{
		game_window.clear();
		while (game_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				game_window.close();
		}

		game_window.display();
	}

    return 0;
}
