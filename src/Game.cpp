/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>

Game::Game(){
	path = "res/";
	biome_path = "none";
	ducks_path = "ducks/";
	player_number = 0;
	is_blind = false;
}

Game::Game(std::string path1, std::string path2, std::string path3){
	path = path1;
	biome_path = path2;
	ducks_path = path3;
	player_number = 0;
	is_blind = false;
}

Game::~Game()
{
	for (unsigned char i = player_number ; i-- ;) {
		free(duck_texture[i][0]);
		free(duck_texture[i][1]);
		free(duck_texture[i]);
	}
}

void Game::launch(sf::RenderWindow& game_window, std::string map_name){

	map_path = map_name;
	if (biome_path == "none") {
		std::ifstream map_file(map_path, std::ios::in | std::ios::binary);
		if (map_file){
			map_file >> biome_path;
			biome_path += '/';
		}
		else{
			std::cout << "Failed to open " << map_path << std::endl;
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
	std::string map_textures_path[9] = {
		biome_path + TEXTURE_OBSTACLE,
		biome_path + TEXTURE_EMPTY_TILE,
		biome_path + TEXTURE_WATER_UP_RIGHT,
		biome_path + TEXTURE_WATER_RIGHT_DOWN,
		biome_path + TEXTURE_WATER_LEFT_DOWN,
		biome_path + TEXTURE_WATER_UP_LEFT,
		biome_path + TEXTURE_WATER_UP_DOWN,
		biome_path + TEXTURE_WATER_LEFT_RIGHT,
		biome_path + TEXTURE_WARP
	};
	bool loading_success = true;
	for (unsigned char i = 9 ; i-- ;) {
		loading_success = loading_success && map_texture[i].loadFromFile(path + map_textures_path[i] + FILETYPE);
	}


	loading_success = loading_success && egg_texture.loadFromFile(path + ducks_path + TEXTURE_EGG + FILETYPE);
	loading_success = loading_success && explosion_texture.loadFromFile(path + ducks_path + TEXTURE_EXPLOSION + FILETYPE);
	loading_success = loading_success && pause_menu_texture.loadFromFile(path + TEXTURE_PAUSE_MENU + FILETYPE);
	loading_success = loading_success && victory_texture.loadFromFile(path + TEXTURE_VICTORY + FILETYPE);
	loading_success = loading_success && this->loadMap();

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
        victory_sprite.setTexture(victory_texture);
		winner = 0;
		game_window.setSize(sf::Vector2u(pxl_length, pxl_height));
		game_window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(pxl_length), static_cast<float>(pxl_height))));

		for (unsigned char i = player_number ; i-- ;) {
			std::string tmp("player ");
			tmp.push_back(static_cast<char>(i + '1'));
			player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], this->loadKeys(tmp));
		}

		explosion_sprite.setTexture(explosion_texture);
		pause_menu_sprite.setTexture(pause_menu_texture);
		pause_menu_sprite.setPosition((static_cast<float>(pxl_length) - pause_menu_sprite.getGlobalBounds().width) / 2.0f, (static_cast<float>(pxl_height) - pause_menu_sprite.getGlobalBounds().height) / 2.0f);

		this->start(game_window);
	}
}

bool Game::loadMap(){
	std::ifstream map_file(map_path, std::ios::in | std::ios::binary);
	unsigned int x_map_size, y_map_size;
	std::string value;

	if (map_file) {
		map_file >> value; // ignoring first line : already interpreted
		map_file >> value;
		if (value ==  "blind" || value == "true" || value == "1"){
			is_blind = true;
			map_file >> los;
			map_file >> value;
			if (value == "loop" || value == "true" || value == "1")
				loop = true;
			else
				loop = false;
		}
		map_file >> game_speed;
		map_file >> egg_victory;
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
					case IDENTIFIER_WARP:
						map_interpreted[j].push_back(WARP);
						break;
					default:
						std::cout << "Bad map (found character " << value[j] << ")" << std::endl;
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

			map_file >> player_spawn[player_number].x;
			--player_spawn[player_number].x;
			map_file >> player_spawn[player_number].y;
			--player_spawn[player_number].y;
			if (value == "up")
				player_initial_dir.push_back(UP);
			else if (value == "down")
				player_initial_dir.push_back(DOWN);
			else if (value == "left")
				player_initial_dir.push_back(LEFT);
			else if (value == "right")
				player_initial_dir.push_back(RIGHT);
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

void Game::start(sf::RenderWindow& game_window)
{
	int tmp(1);
	std::vector<Direction> player_dir;
	for(unsigned char i = 0; i < player_number; ++i)
		player_dir.push_back(player[i].getDirection());

	sf::SoundBuffer new_egg_buffer, damages_buffer, warp_buffer;
	new_egg_buffer.loadFromFile("res/sounds/new_egg.ogg");
	damages_buffer.loadFromFile("res/sounds/damaged.ogg");
	warp_buffer.loadFromFile("res/sounds/warp.ogg");

	sf::Sound egg_sound, damage_sound, warp_sound;
	egg_sound.setBuffer(new_egg_buffer);
	damage_sound.setBuffer(damages_buffer);
	warp_sound.setBuffer(warp_buffer);

	sf::Music game_theme;
	game_theme.openFromFile("res/sounds/game_theme.ogg");
	game_theme.setLoop(true);
	game_theme.play();

	sf::Event event;
	game_map.popEgg();
	game_map.popEgg(); // Map thuging

	std::vector<Coord> explosions_coord;

	while (game_window.isOpen() && winner == 0)
	{
		while (game_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game_window.close();
			else if(event.type == sf::Event::KeyPressed){
				for(unsigned char i = player_number; i--;){
					if(event.key.code == player[i].keys[0] && (player[i].getDirection() != DOWN || player[i].size() == 0))
						player_dir[i] = UP;
					else if(event.key.code == player[i].keys[1] && (player[i].getDirection() != UP || player[i].size() == 0))
						player_dir[i] = DOWN;
					else if(event.key.code == player[i].keys[2] && (player[i].getDirection() != RIGHT || player[i].size() == 0))
						player_dir[i] = LEFT;
					else if(event.key.code == player[i].keys[3] && (player[i].getDirection() != LEFT || player[i].size() == 0))
						player_dir[i] = RIGHT;
				}
				if (event.key.code == sf::Keyboard::Escape) {
					game_theme.pause();
					if (!(this->pauseGame(game_window, true)))
						return;
					game_theme.play();
				}
			}
			else if (event.type == sf::Event::LostFocus)
                if (!(this->pauseGame(game_window, false)))
                    return;
		}
		sf::sleep(sf::milliseconds(game_speed));
		--tmp;

		if (is_blind){
			game_window.clear();
			for (unsigned char i = player_number ; i--;){
				game_map.print(game_window, player[i].getCoord(), los, loop);
				for (unsigned char j = player[i].size() ; j-- ;){
					game_map.print(game_window, player[i].duckies[j].getCoord(), static_cast<unsigned short>(los/2), loop);
				}
			}
			game_map.printEgg(game_window);
		}
		else game_map.print(game_window);

		if(tmp == 0){
			tmp = 16;
			explosions_coord.erase(explosions_coord.begin(), explosions_coord.end());
			for(unsigned char i = player_number; i--;){
				if(game_map.isWarp(player[i].getCoord())){
					warp_sound.play();
					player[i].warped(game_map.getWarp(player[i].getCoord()));
				}
				player[i].move(player_dir[i], game_map.x_size, game_map.y_size);

				bool damaged(false);
				unsigned int j = player_number;
				while(j > 0 && !damaged){

					--j;
					if(i != j){
						if(player[i].getCoord() == player[j].getCoord()){
							explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());
							explosions_coord.push_back(player[j].getCoord() - player[j].getDirection());
							player[i].damaged(player_initial_dir[i]);
							player[j].damaged(player_initial_dir[j]);
							player_dir[i] = player_initial_dir[i];
							player_dir[j] = player_initial_dir[j];
							damaged = true;
							damage_sound.play();
						}
					}

					unsigned char k = player[j].size();
					while(k > 0 && !damaged){
						--k;
						if(player[i].getCoord() == player[j].duckies[k].getCoord()){
							explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());

							if (player[i].size() > 0 && !(player[i].isInvulnerable()) && i != j){
								player[j].powerUp();
								if (player[j].size() == egg_victory && egg_victory != 0)
									winner = static_cast<unsigned char>(j + 1);
							}
							player[i].damaged(player_initial_dir[i]);
							player_dir[i] = player_initial_dir[i];
							damaged = true;
							damage_sound.play();
						}
					}
				}
				if(game_map.map[player[i].getCoord().x][player[i].getCoord().y] == OBSTACLE){
					explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());
					player[i].damaged(player_initial_dir[i]);
					player_dir[i] = player_initial_dir[i];
					damage_sound.play();
				}
			}
			for(unsigned char i = player_number; i--;){
				if(player[i].getCoord() == game_map.getEggCoord()){
					player[i].powerUp();

					if (player[i].size() == egg_victory && egg_victory != 0) {
						winner = static_cast<unsigned char>(i + 1);
					}
					game_map.popEgg();
					egg_sound.play();
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
		for(unsigned int i = static_cast<unsigned int>(explosions_coord.size()); i--;){
			this->printExplosion(game_window, explosions_coord[i]);
		}
		game_window.display();
	}
	game_theme.stop();
	this->printVictory(game_window);
}

std::vector<sf::Keyboard::Key> Game::loadKeys(std::string selected_player){
	std::ifstream keys_file("keys.conf", std::ios::in | std::ios::binary);
	std::vector<sf::Keyboard::Key> answer;

	if (keys_file.fail()) {
		std::cout << "Failed to open" << path << "keys.conf (was looking for " << selected_player << ")" << std::endl;
	}
	else{
		sf::Keyboard::Key keys[4];
		bool key_loading_successful[4] = {false, false, false, false};
		std::string line, argument, value;

		while (std::getline(keys_file, line))
		{
			if (line[0] != '#') {

				std::istringstream parser(line);
				std::getline(parser, argument, ':');
				std::getline(parser, value);
				value.erase(0, 1);
				unsigned char selected;
				if (argument.size() >= selected_player.size() + 1) {
					if (argument.substr(0, selected_player.size()) == selected_player){
						switch(argument.substr(selected_player.size() + 1)[0]){
							case 'u':
								selected = 3;
								break;
							case 'd':
								selected = 2;
								break;
							case 'l':
								selected = 1;
								break;
							case 'r':
								selected = 0;
								break;
							default:
								selected = 4;
								break;
						}
						if (selected != 4) {
							char val = value[0];
							key_loading_successful[selected] = true;
							if (val >= 'a' && val <= 'z') {
								keys[selected] = static_cast<sf::Keyboard::Key>(val - 'a');
							}
							else if (val <= '9' && val >= '0'){
								keys[selected] = static_cast<sf::Keyboard::Key>(val - '0' + 75);
							}
							else{
								switch(val){
									case 'U':
										keys[selected] = sf::Keyboard::Up;
										break;
									case 'D':
										keys[selected] = sf::Keyboard::Down;
										break;
									case 'L':
										keys[selected] = sf::Keyboard::Left;
										break;
									case 'R':
										keys[selected] = sf::Keyboard::Right;
										break;
									default:
										key_loading_successful[selected] = false;
										break;
								}
							}
						}
					}
				}
			}
		}
		if (!key_loading_successful[0] || !key_loading_successful[1] || !key_loading_successful[2] || !key_loading_successful[3]) {
			std::cout << "Failed to load keys for " << selected_player << std::endl;
            for (unsigned char i(4) ; i--;) {
                if (!key_loading_successful[i])
                    answer.push_back(sf::Keyboard::Escape);
                else
                    answer.push_back(keys[i]);
            }
		}
		else{
			for (unsigned char i(4) ; i-- ;) {
				answer.push_back(keys[i]);
			}
		}
	}
	return answer;
}

void Game::printExplosion(sf::RenderWindow& game_window, Coord coord){
	explosion_sprite.setPosition(static_cast<float>(coord.x * 32), static_cast<float>(coord.y * 32));
	game_window.draw(explosion_sprite);
}

bool Game::pauseGame(sf::RenderWindow& game_window, bool player_request){
	sf::Event event;
	if(player_request){
		game_window.draw(pause_menu_sprite);
		game_window.display();
		do{
			if (game_window.waitEvent(event) && sf::Event::Closed == event.type) {
				game_window.close();
                return false;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				return false;
			}
		}while(sf::Event::KeyPressed != event.type || event.key.code != sf::Keyboard::Return);
	}
	else{
		do{
			sf::sleep(sf::milliseconds(20));
			if (game_window.waitEvent(event) && sf::Event::Closed == event.type) {
				game_window.close();
                return false;
			}
		}while(event.type != sf::Event::GainedFocus);
	}
	return true;
}

void Game::printVictory(sf::RenderWindow& game_window){

	sf::Music victory_theme;
	victory_theme.openFromFile("res/sounds/victory_theme.ogg");
	victory_theme.setLoop(false);
	victory_theme.play();

	sf::Sprite winner_sprite[4];
	winner_sprite[0].setTexture(duck_texture[winner - 1][0][2]);
    winner_sprite[1].setTexture(duck_texture[winner - 1][0][1]);
    winner_sprite[2].setTexture(duck_texture[winner - 1][0][3]);
    winner_sprite[3].setTexture(duck_texture[winner - 1][0][0]);

    float y_pos(226.f), pos(336.f);;

    game_window.setSize(sf::Vector2u(800, 600));
	game_window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));

    victory_sprite.setPosition(0,0);
	game_window.draw(victory_sprite);
	game_window.display();

    for (unsigned char i(4); i-- ;){
        winner_sprite[i].setScale(4,4);
        winner_sprite[i].setPosition(pos,y_pos);
    }

	bool end(false);
	sf::Event event;
	sf::Clock elapsed_time;

    unsigned char i(0), j(0);


	while (game_window.isOpen() && !end)
	{
        game_window.clear();
		game_window.draw(victory_sprite);
        game_window.draw(winner_sprite[j]);
		game_window.display();

        i = (unsigned char)((i + 1)%180);
        j = (unsigned char)(i/45);

		while (game_window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				game_window.close();
			else if((event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) && (elapsed_time.getElapsedTime().asSeconds() > 0.5f)){
				end = true;
			}
		}
		sf::sleep(sf::milliseconds(10));
	}
}
