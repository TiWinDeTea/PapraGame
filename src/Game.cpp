/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>

Game::~Game()
{
	for (unsigned char i = player_number ; i-- ;) {
		free(duck_texture[i][0]);
		free(duck_texture[i][1]);
		free(duck_texture[i]);
	}
}

void Game::launch(){

	if (biome_path == "none") {
		std::ifstream map_file(path + MAPFILE, std::ios::in | std::ios::binary);
		if (map_file){
			map_file >> biome_path;
			biome_path += '/';
		}
		map_file.close();
	}

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
	std::string map_textures_path[8] = {
		biome_path + TEXTURE_OBSTACLE,
		biome_path + TEXTURE_EMPTY_TILE,
		biome_path + TEXTURE_WATER_UP_RIGHT,
		biome_path + TEXTURE_WATER_RIGHT_DOWN,
		biome_path + TEXTURE_WATER_LEFT_DOWN,
		biome_path + TEXTURE_WATER_UP_LEFT,
		biome_path + TEXTURE_WATER_UP_DOWN,
		biome_path + TEXTURE_WATER_LEFT_RIGHT
	};
	bool loading_success = true;
	for (unsigned char i = 8 ; i-- ;) {
		loading_success = loading_success && map_texture[i].loadFromFile(path + map_textures_path[i] + FILETYPE);
	}


	loading_success = loading_success && egg_texture.loadFromFile(path + ducks_path + TEXTURE_EGG + FILETYPE);
	loading_success = this->loadMap();

	char player_id;
	for (unsigned char i = player_number ; i-- ;) {
		player_id = static_cast<char>(i + '0');
		for (unsigned int j = 4 ; j-- ;) {
			loading_success = loading_success && duck_texture[i][0][j].loadFromFile(path + duck_textures_path[j] + player_id + FILETYPE);
			loading_success = loading_success && duck_texture[i][1][j].loadFromFile(path + ducky_textures_path[j] + player_id + FILETYPE);
		}
	}

	if (!loading_success) {
		std::cout << "Failed to load game ressources" << std::endl;
	}
	else {
		winner = 0;
		game_window.create(sf::VideoMode(pxl_length, pxl_height), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);

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

		for (unsigned char i = player_number ; i-- ;) {
			player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], player_keys[i]);
		}

		this->start();
	}
}

bool Game::loadMap(){
	std::ifstream map_file(path + MAPFILE, std::ios::in | std::ios::binary);
	unsigned int x_map_size, y_map_size;
	std::string value;

	if (map_file) {
		map_file >> value; // ignoring first line : already interpreted
		map_file >> game_speed;
		map_file >> x_map_size;
		map_file >> y_map_size;
		pxl_height = 32*y_map_size;
		pxl_length = 32*x_map_size;

		std::vector< std::vector<Area> > map_interpreted;
		for(unsigned int i = 0; i < x_map_size; ++i)
			map_interpreted.push_back(std::vector<Area>());

		for (unsigned int i = 0 ; i < y_map_size ; ++i) {
			map_file >> value;
			for (unsigned int j = 0 ; j < x_map_size ; ++j) {
				switch(value[j])
				{
					case IDENTIFIER_EMPTY_TILE:
						map_interpreted[j].push_back(EMPTY_TILE);
						break;
					case IDENTIFIER_OBSTACLE:
						map_interpreted[j].push_back(OBSTACLE);
						break;
					case IDENTIFIER_WATER_LEFT_DOWN:
						map_interpreted[j].push_back(WATER_DL);
						break;
					case IDENTIFIER_WATER_LEFT_RIGHT:
						map_interpreted[j].push_back(WATER_LR);
						break;
					case IDENTIFIER_WATER_RIGHT_DOWN:
						map_interpreted[j].push_back(WATER_RD);
						break;
					case IDENTIFIER_WATER_UP_DOWN:
						map_interpreted[j].push_back(WATER_UD);
						break;
					case IDENTIFIER_WATER_UP_LEFT:
						map_interpreted[j].push_back(WATER_LU);
						break;
					case IDENTIFIER_WATER_UP_RIGHT:
						map_interpreted[j].push_back(WATER_UR);
						break;
					default:
						std::cout << "Bad map" << std::endl;
						return false;
						break;
				}
			}
		}
		map_file >> value;
		do{
			sf::Texture** texture_array = (sf::Texture**) calloc(2, sizeof(sf::Texture*));
			texture_array[0] = (sf::Texture*) calloc(4, sizeof(sf::Texture));
			texture_array[1] = (sf::Texture*) calloc(4, sizeof(sf::Texture));
			duck_texture.push_back(texture_array);
			player.push_back(Duck());
			player_spawn.push_back(Coord());
			player_initial_dir.push_back(Direction());

			map_file >> player_spawn[player_number].x;
			map_file >> player_spawn[player_number].y;
			if (value == "up")
				player_initial_dir[player_number] = UP;
			else if (value == "down")
				player_initial_dir[player_number] = DOWN;
			else if (value == "left")
				player_initial_dir[player_number] = LEFT;
			else if (value == "right")
				player_initial_dir[player_number] = RIGHT;
			else{
				std::cout << "Not a valid direction : " << value << std::endl;
				std::cout << "(On player " << player_number << ")" << std::endl;
				return false;
			}
			++player_number;
			map_file >> value;
		}while (value != "eof");

		game_map = Map(x_map_size, y_map_size, map_interpreted, map_texture, &egg_texture);
		return true;
	}
	else
		return false;
}

void Game::start()
{
	int tmp(1);
	std::vector<Direction> player_dir;
	for(unsigned char i = player_number; i--;)
		player_dir.push_back(player[i].getDirection());

	sf::Event event;
	game_map.popEgg(game_window);

	while (game_window.isOpen())
	{
		while (game_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game_window.close();
			else if(event.type == sf::Event::KeyPressed){
				for(unsigned char i = player_number; i--;){
					if(event.key.code == player[i].keys[0])
						player_dir[i] = UP;
					else if(event.key.code == player[i].keys[1])
						player_dir[i] = DOWN;
					else if(event.key.code == player[i].keys[2])
						player_dir[i] = LEFT;
					else if(event.key.code == player[i].keys[3])
						player_dir[i] = RIGHT;
				}
			}
		}
		sf::sleep(sf::milliseconds(game_speed));
		--tmp;

		game_map.print(game_window);
		if(tmp == 0){
			tmp = 16;
			for(unsigned char i = player_number; i--;){
				player[i].move(game_window, player_dir[i]);

				if(player[i].getCoord() == game_map.getEggCoord()){
					player[i].powerUp(game_window);
					game_map.popEgg(game_window);
				}
				player[i].print(game_window);
			}
		}
		else{
			for (unsigned char i = player_number ; i-- ;) {
				player[i].print(game_window, -static_cast<float>(tmp) * 2);
			}
		}
		game_window.display();
	}
}
