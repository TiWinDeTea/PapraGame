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

	this->getMapFile();
	if (biome_path == "none") {
		std::ifstream map_file(path + map_file_name, std::ios::in | std::ios::binary);
		if (map_file){
			map_file >> biome_path;
			biome_path += '/';
		}
		else{
			std::cout << "Failed to open " << path + map_file_name << std::endl;
			return;
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

void Game::getMapFile(){
	DIR* directory = opendir(std::string(path + "maps").c_str());
	struct dirent* redfile = NULL;
	map_file_name = "map";

	if( directory == NULL ){
		std::cout << "Could not open the folder " << path << "maps/" << std::endl;
		std::cout << "Assuming " << path << "map to be the map's file" << std::endl;
	}
	else{
		int choice;
		std::vector<std::string> maps;
		auto isFile = [](std::string const& local_path) -> bool {
			#ifdef OS_WINDOWS
			std::ifstream file (local_path.c_str(), std::ios::in);
			return (file.fail());
			#else
			struct stat s;
			if( stat(local_path.c_str() ,&s ) == 0)
				return !(s.st_mode & S_IFDIR);
			else
				return false;
			#endif
		};
		std::cout << "Found files :" << std::endl;
		while ((redfile = readdir(directory)) != NULL){
			std::string tmp( redfile->d_name );
			if (isFile(path + "maps/" + tmp)) {
				maps.push_back("maps/" + tmp);
				std::cout << '\t' << maps.size() << ". " << maps.back() << std::endl;
			}
		}
		if (maps.size() == 0) {
			std::cout << "No file found !" << std::endl;
			std::cout << "Assuming" << path << "map to be the map's file" << std::endl;
		}
		else{
			std::cout << std::endl << "Your map choice (enter the number) : ";
			std::cin >> choice;
			while(choice <= 0 || choice > static_cast<signed>(maps.size()))
			{
				std::cout << "Invalid input. Please retry : ";
				std::cin >> choice;
			}
			map_file_name = maps[choice - 1];
		}
	}
}

bool Game::loadMap(){
	std::ifstream map_file(path + map_file_name, std::ios::in | std::ios::binary);
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
		game_map.init();
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
	game_map.popEgg(game_window); // Map thuging

	while (game_window.isOpen())
	{
		while (game_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game_window.close();
			else if(event.type == sf::Event::KeyPressed){
				for(unsigned char i = player_number; i--;){
					if(event.key.code == player[i].keys[0] && player[i].getDirection() != DOWN)
					    player_dir[i] = UP;
					else if(event.key.code == player[i].keys[1] && player[i].getDirection() != UP)
					    player_dir[i] = DOWN;
					else if(event.key.code == player[i].keys[2] && player[i].getDirection() != RIGHT)
					    player_dir[i] = LEFT;
					else if(event.key.code == player[i].keys[3] && player[i].getDirection() != LEFT)
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
				player[i].move(player_dir[i], game_map.x_size, game_map.y_size);

				bool damaged(false);
				unsigned int j = player_number;
				while(j > 0 && !damaged){

					--j;
					if(i != j){
						if(player[i].getCoord() == player[j].getCoord()){
							player[i].damaged(player_initial_dir[i]);
							player[j].damaged(player_initial_dir[j]);
							player_dir[i] = player_initial_dir[i];
							player_dir[j] = player_initial_dir[j];
							damaged = true;
						}
					}

					unsigned char k = player[j].size();
					while(k > 0 && !damaged){
						--k;
						if(player[i].getCoord() == player[j].duckies[k].getCoord()){
							player[i].damaged(player_initial_dir[i]);
							player_dir[i] = player_initial_dir[i];
							damaged = true;
						}
					}
				}
				if(game_map.map[player[i].getCoord().x][player[i].getCoord().y] == OBSTACLE){
					player[i].damaged(player_initial_dir[i]);
                    player_dir[i] = player_initial_dir[i];
				}
			}
			for(unsigned char i = player_number; i--;){
				if(player[i].getCoord() == game_map.getEggCoord()){
					player[i].powerUp(game_window);
					game_map.popEgg(game_window);
				}
				player[i].print(game_window);
			}
		}
		else{
			for (unsigned char i = player_number ; i-- ;) {
				if(player[i].isInvulnerable()){
					if(tmp < 8)
						player[i].print(game_window, -static_cast<float>(tmp) * 2);
				}
				else{
					player[i].print(game_window, -static_cast<float>(tmp) * 2);
				}
			}
		}
		game_window.display();
	}
}
