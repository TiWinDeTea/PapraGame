/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <NetworkGame.hpp>

GameServer::~GameServer(){
	for (size_t i = clients.size() - 1; i-- ;) {
		delete clients[i];
	}
}

GameServer::GameServer(std::string ressources_path, std::string biome_path_, std::string map_file_name){

	ressources = ressources_path;
	biome_path = biome_path_;
	std::vector< std::vector<Area> > map_as_area;
	unsigned char nbr_player = 0;
	std::string tmpstr;

	std::ifstream map_file(map_file_name, std::ios::in | std::ios::binary);
	if (biome_path == "nope")
		map_file >> biome_path;
	else map_file >> tmpstr;
	map_file >> game_speed >> egg_victory >> map_width >> map_height;

	for (unsigned int i = map_width ; i-- ;) {
		map_as_area.push_back(std::vector<Area>());
	}

	for (unsigned int i = 0 ; i < map_height ; ++i) {
		map_file >> tmpstr;
		for (unsigned int j = 0 ; j < map_width ; ++j) {
			switch(tmpstr[j])
			{
				case IDENTIFIER_EMPTY_TILE:
					map_as_area[j].push_back(EMPTY_TILE);
					break;
				case IDENTIFIER_OBSTACLE:
					map_as_area[j].push_back(OBSTACLE);
					break;
				case IDENTIFIER_WATER_LEFT_DOWN:
					map_as_area[j].push_back(WATER_DL);
					break;
				case IDENTIFIER_WATER_LEFT_RIGHT:
					map_as_area[j].push_back(WATER_LR);
					break;
				case IDENTIFIER_WATER_RIGHT_DOWN:
					map_as_area[j].push_back(WATER_RD);
					break;
				case IDENTIFIER_WATER_UP_DOWN:
					map_as_area[j].push_back(WATER_UD);
					break;
				case IDENTIFIER_WATER_UP_LEFT:
					map_as_area[j].push_back(WATER_LU);
					break;
				case IDENTIFIER_WATER_UP_RIGHT:
					map_as_area[j].push_back(WATER_UR);
					break;
				case IDENTIFIER_WARP:
					map_as_area[j].push_back(WARP);
					break;
				default:
					map_as_area[j].push_back(EMPTY_TILE);
					break;
			}
		}
	}

	map_file >> tmpstr;
	do{
		player_spawn.push_back(Coord());
		player.push_back(Duck());

		map_file >> player_spawn[nbr_player].x;
		--player_spawn[nbr_player].x;
		map_file >> player_spawn[nbr_player].y;
		if (tmpstr == "up")
			player_initial_dir.push_back(UP);
		else if (tmpstr == "down")
			player_initial_dir.push_back(DOWN);
		else if (tmpstr == "left")
			player_initial_dir.push_back(LEFT);
		else if (tmpstr == "right")
			player_initial_dir.push_back(RIGHT);
		else
			player_initial_dir.push_back(RIGHT);
		++nbr_player;
		map_file >> tmpstr;
	}while(tmpstr != "eof");

	game_map = Map(map_width, map_height, map_as_area);

	this->getClients(map_as_area);
}

void GameServer::getClients(std::vector< std::vector<Area> > map_as_area){
	sf::Packet packet;
	packet.clear();
	packet << static_cast<unsigned int>(player.size()) << map_height << map_width << game_speed << ressources << biome_path;

	for(unsigned int i = 0 ; i < player.size() ; ++i){
		packet << player_spawn[i].x << player_spawn[i].y << static_cast<int>(player_initial_dir[i]);
	}

	for(unsigned int i = 0 ; i < map_height ; ++i){
		for (unsigned int j = 0 ; j < map_width ; ++j) {
			packet << static_cast<int>(map_as_area[i][j]);
		}
	}

	sf::UdpSocket udp_client;
	udp_client.bind(PORT);

	sf::TcpListener listener;
	if (listener.listen(PORT) != sf::Socket::Done){
		std::cout << "Cannot host the game" << std::endl;
		return;
	}

	char retrieved_data[25];
	size_t retrieved_data_size;
	unsigned short client_port;
	sf::IpAddress client_ip;
	sf::Socket::Status status;

	std::cout << "Server : starting up" << std::endl << std::endl;

	do{
		status = udp_client.receive(retrieved_data, 25, retrieved_data_size, client_ip, client_port);

		if (status == sf::Socket::Done && retrieved_data_size == 25 && std::string(retrieved_data) == "PapraGame ~ Game Request" && client_port == (PORT + 1)) {

			std::cout << "Request from " << client_ip << ":" << PORT << std::endl;
			sf::TcpSocket* client = new sf::TcpSocket;
			status = udp_client.send("PapraGame ~ Client Accepted", 28, client_ip, client_port);

			if (status == sf::Socket::Done && listener.accept(*client) == sf::Socket::Done) {

				std::cout << "+ Connected" << std::endl;
				clients.push_back(client);
				std::cout << "+ Sending maps infos" << std::endl;
				client->send(packet);
				std::cout << "+ Done" << std::endl << std::endl;
			}
			else{
				std::cout << "- Not Connected" << std::endl;
				delete client;
			}
		}
	}while(true);
}

void GameServer::launch(sf::RenderWindow& game_window){
	std::string duck_textures_path[4] = {
		DUCK_PATH + TEXTURE_DUCK_UP,
		DUCK_PATH + TEXTURE_DUCK_DOWN,
		DUCK_PATH + TEXTURE_DUCK_LEFT,
		DUCK_PATH + TEXTURE_DUCK_RIGHT
	};
	std::string ducky_textures_path[4] = {
		DUCK_PATH + TEXTURE_DUCKY_UP,
		DUCK_PATH + TEXTURE_DUCKY_DOWN,
		DUCK_PATH + TEXTURE_DUCKY_LEFT,
		DUCK_PATH + TEXTURE_DUCKY_RIGHT
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
		loading_success = loading_success && map_texture[i].loadFromFile(ressources + map_textures_path[i] + FILETYPE);
	}

	loading_success = loading_success && egg_texture.loadFromFile(ressources + DUCK_PATH + TEXTURE_EGG + FILETYPE);
	loading_success = loading_success && explosion_texture.loadFromFile(ressources + DUCK_PATH + TEXTURE_EXPLOSION + FILETYPE);

	char player_id;
	for (unsigned int i = static_cast<int>(player.size()) ; i-- ;) {
		player_id = static_cast<char>(i + '0');
		for (unsigned int j = 4 ; j-- ;) {
			loading_success = loading_success && duck_texture[i][0][j].loadFromFile(ressources + duck_textures_path[j] + player_id + FILETYPE);
			loading_success = loading_success && duck_texture[i][1][j].loadFromFile(ressources + ducky_textures_path[j] + player_id + FILETYPE);
		}
	}

	sf::Packet packet;
	if (!loading_success) {
		packet << false;
		std::cout << "Failed to load game ressources" << std::endl;
		for (size_t i = clients.size() ; i-- ;){
			clients[i]->send(packet);
		}
	}
	else {
		game_window.create(sf::VideoMode(map_width*32, map_height*32), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);

		for (unsigned int i = static_cast<int>(player.size()) ; i-- ;) {
			std::string tmp("player ");
			tmp.push_back(static_cast<char>(i + '1'));
			player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], this->loadKeys("player 1"));
		}
		packet << true;
		for (size_t i = clients.size() ; i--;){
			clients[i]->send(packet);
		}
		this->start(game_window);
	}
}

void GameServer::start(sf::RenderWindow& game_window){

	std::cout << "Game's starting" << std::endl;

	std::vector<Direction> player_dir = player_initial_dir;

	unsigned char winner(0);
	sf::Packet packet;

	game_map.popEgg();
	game_map.popEgg(); // Map thuging #2

	do{
		packet << false;

		for(unsigned char i((unsigned char)(player.size())); i--;){

			if(game_map.isWarp(player[i].getCoord()))
				player[i].warped(game_map.getWarp(player[i].getCoord()));

			player[i].move(player_dir[i], game_map.x_size, game_map.y_size);

			bool damaged(false);
			unsigned int j = static_cast<unsigned int>(player.size() - 1);

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

			packet << damaged;

			if (!damaged){

				packet << player_dir[i];

				if(player[i].getCoord() == game_map.getEggCoord()){
					player[i].powerUp();

					if (player[i].size() == egg_victory && egg_victory != 0) {
						winner = static_cast<unsigned char>(i + 1);
					}
					game_map.popEgg();

					packet << true << game_map.getEggCoord().x << game_map.getEggCoord().y;
				}
			}
		}

		for (unsigned char i((unsigned char)(clients.size())) ; i-- ;) {
			clients[i]->send(packet);
		}

		for (unsigned char i((unsigned char)(clients.size())) ; i-- ;) {
			packet.clear();
			clients[i]->receive(packet);
			int dir_as_int;
			packet >> dir_as_int;
			player_dir[i] = static_cast<Direction>(dir_as_int);
			if (player_dir[i] == NOPE) {

			}
		}

		sf::sleep(sf::milliseconds(game_speed));

		packet.clear();
	}while(winner == 0);

	packet << true << winner;
	for (size_t i(clients.size() - 1) ; i-- ;) {
		clients[i]->send(packet);
	}
}

std::vector<sf::Keyboard::Key> GameServer::loadKeys(std::string selected_player){
	std::ifstream keys_file("keys.conf", std::ios::in | std::ios::binary);
	std::vector<sf::Keyboard::Key> answer;

	if (keys_file.fail()) {
		std::cout << "Failed to open keys.conf (was looking for " << selected_player << ")" << std::endl;
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
		}
		else{
			for (unsigned char i = 4 ; i-- ;) {
				answer.push_back(keys[i]);
			}
		}
	}
	return answer;
}

void GameClient::launch(sf::RenderWindow& game_window){

	sf::UdpSocket broadcast;
	broadcast.bind(PORT + 1);
	size_t received;
	sf::IpAddress sender;
	unsigned short port;
	char data[26];

	if (broadcast.send("PapraGame ~ Game Request", 25, sf::IpAddress::Broadcast, PORT)!=sf::Socket::Done){
		std::cout << "- Cannot find the server" << std::endl;
		return;
	} else {
		broadcast.setBlocking(true);
		std::cout << "- Can find a server, try to receive data from the server." << std::endl;
		if (broadcast.receive(data, 26, received, sender, port) != sf::Socket::Done && std::string(data) != "PapraGame ~ Game Accepted"){
			std::cout << "- No data received from the server." << std::endl;
			return;
		} else  {
			std::cout << "- Receive data from the server with ip addrress "<< sender << " and with the port " << PORT <<", trying to connect." << std::endl;
			if (server.connect("192.168.43.111", PORT) != sf::Socket::Done){
				std::cout << "- Cannot be connected with the server";
				return;
			} else {

				std::cout << "- Connected" << std::endl;
				unsigned int nbr_player;
				sf::Packet packet;

				server.receive(packet);
				packet >> nbr_player >> map_width >> map_height >> game_speed;

				std::cout << nbr_player <<" - " << map_width << " - " << map_height << std::endl;

				std::string path;
				std::vector<Coord> spawn;
				std::vector<Direction> dir;
				std::vector<std::vector<Area>> tmp_map;
				packet >> path >> biome_path;

				for (unsigned int i = nbr_player; i--;) {
					player.push_back(Duck());
					spawn.push_back(Coord());
					dir.push_back(Direction());
				}
				for (unsigned int i = map_height; i--;){
					tmp_map.push_back(std::vector<Area>());
				}
				for (unsigned int i = 0; i < nbr_player; ++i){
					packet >> spawn[i].x >> spawn[i].y;
					int tmp;
					packet >> tmp;
					dir[i] = static_cast<Direction>(tmp);
				}
				for (unsigned int i = 0; i < map_height; ++i){
					int tmp_tile;
					for( unsigned int j = 0; j < map_width; ++j){
						packet >> tmp_tile;
						tmp_map[i].push_back(static_cast<Area>(tmp_tile));
					}
				}
				std::string duck_textures_path[4] = {
					DUCK_PATH + TEXTURE_DUCK_UP,
					DUCK_PATH + TEXTURE_DUCK_DOWN,
					DUCK_PATH + TEXTURE_DUCK_LEFT,
					DUCK_PATH + TEXTURE_DUCK_RIGHT
				};
				std::string ducky_textures_path[4] = {
					DUCK_PATH + TEXTURE_DUCKY_UP,
					DUCK_PATH + TEXTURE_DUCKY_DOWN,
					DUCK_PATH + TEXTURE_DUCKY_LEFT,
					DUCK_PATH + TEXTURE_DUCKY_RIGHT
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

				loading_success = loading_success && egg_texture.loadFromFile(path + DUCK_PATH + TEXTURE_EGG + FILETYPE);
				loading_success = loading_success && explosion_texture.loadFromFile(path + DUCK_PATH + TEXTURE_EXPLOSION + FILETYPE);

				char player_id;
				for (unsigned char i = nbr_player ; i-- ;) {
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
					game_window.create(sf::VideoMode(map_width*32, map_height*32), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);

					for (unsigned char i = nbr_player ; i-- ;) {
						std::string tmp("player ");
						tmp.push_back(static_cast<char>(i + '1'));
						player[i] = Duck(duck_texture[i][0], duck_texture[i][1], spawn[i], dir[i], this->loadKeys("player 1"));
					}

					explosion_sprite.setTexture(explosion_texture);

					game_map = Map(map_width, map_height, tmp_map, map_texture, &egg_texture);
					direction = NOPE;
					int dir;
					do {
						packet.clear();
						server.receive(packet);
						if((packet >> dir))
							direction = static_cast<Direction>(dir);
					} while(direction == NOPE);

					this->start(game_window);
				}
			}
		}
	}
}

std::vector<sf::Keyboard::Key> GameClient::loadKeys(std::string selected_player){
	std::ifstream keys_file("keys.conf", std::ios::in | std::ios::binary);
	std::vector<sf::Keyboard::Key> answer;

	if (keys_file.fail()) {
		std::cout << "Failed to open keys.conf (was looking for " << selected_player << ")" << std::endl;
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
		}
		else{
			for (unsigned char i = 4 ; i-- ;) {
				answer.push_back(keys[i]);
			}
		}
	}
	return answer;
}

void GameClient::start(sf::RenderWindow& game_window){
	sf::Packet packet;
	sf::Event event;
	bool ended, damaged, power_up;
	unsigned char winner;
	int tmpint, egg_x, egg_y;
	game_window.setSize(sf::Vector2u(map_width*32, map_height*32));
	game_window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(map_width*32), static_cast<float>(map_height*32))));
	game_window.requestFocus();
	do {
		packet.clear();
		server.receive(packet);
		packet >> ended;
		if (ended) {
			packet >> winner;
		} else {
			for (unsigned int i = static_cast<int>(player.size()) - 1; i--;){
				packet >> damaged;
				if (!damaged){
					packet >> tmpint >> power_up;
					direction = static_cast<Direction>(tmpint);
					player[i].move(direction,map_width, map_height);
					if (power_up){
						packet >> egg_x >> egg_y;
						game_map.popEgg(Coord(egg_x,egg_y));
						player[i].powerUp();
					}
				} else {
					player[i].damaged(player_initial_dir[i]);
				}
			}
			packet.clear();
			while (game_window.pollEvent(event)){
				if(event.type == sf::Event::Closed){
					game_window.close();
					packet << static_cast<int>(NOPE);
					server.send(packet);
				}
				else if(event.type == sf::Event::KeyPressed){
					if(event.key.code == player[0].keys[0] && (player[0].getDirection() != DOWN || player[0].size() == 0))
						direction = UP;
					else if(event.key.code == player[0].keys[1] && (player[0].getDirection() != UP || player[0].size() == 0))
						direction = DOWN;
					else if(event.key.code == player[0].keys[2] && (player[0].getDirection() != RIGHT || player[0].size() == 0))
						direction = LEFT;
					else if(event.key.code == player[0].keys[3] && (player[0].getDirection() != LEFT || player[0].size() == 0))
						direction = RIGHT;
				}
			}
			if (game_window.isOpen()){
				packet << static_cast<int>(direction);
				server.send(packet);
				game_map.print(game_window);
				for (unsigned int i = static_cast<int>(player.size()) -1; i--;){
					if (game_map.isWarp(player[i].getCoord())){
						player[i].warped(game_map.getWarp(player[i].getCoord()));
					} else {
						player[i].move(direction, map_width, map_height);
					}
					player[i].print(game_window);
				}
			}
		}
	} while (!ended);
}
