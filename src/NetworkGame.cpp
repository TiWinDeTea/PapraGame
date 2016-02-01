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

GameServer::GameServer(std::string ressources, std::string biome_path, std::string map_file_name){

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

	this->getClients(ressources, biome_path, map_as_area);
}

void GameServer::getClients(std::string res, std::string biome_path, std::vector< std::vector<Area> > map_as_area){
	sf::Packet packet;

	packet << res << biome_path << game_speed << map_width << map_height;
	for (unsigned int i = 0 ; i < map_height ; ++i) {
		for (unsigned int j = 0 ; j < map_width ; ++j) {
			packet << map_as_area[i][j];
		}
	}
	packet << static_cast<int>(player.size());

	for (unsigned int i = 0 ; i < player.size() ; ++i) {
		packet << player_spawn[i].x << player_spawn[i].y << player_initial_dir[i];
	}

	sf::TcpListener listener;
	sf::UdpSocket potential_client;
	potential_client.bind(PORT);

	potential_client.setBlocking(false);

	char data[25] = "Pass";
	const char accept[26] = "PapraGame ~ Game accepted";
	size_t volume_received = 0;

	if (listener.listen(PORT) != sf::Socket::Done) {
		std::cout << "Failed to create the server" << std::endl;
		return;
	}

	std::cout << "Press enter to start the game" << std::endl << std::endl;

	do{
		sf::IpAddress client_ip;
		unsigned short client_port;
		potential_client.receive(data, 25, volume_received, client_ip, client_port);

		sf::sleep(sf::milliseconds(30));

		if (volume_received == 25 && std::string(data) == "PapraGame ~ Game request") {
			data[0] = '\0';
			volume_received = 0;
			std::cout << "Request from " << client_ip << ":" << client_port << std::endl;
			sf::sleep(sf::milliseconds(100));
			std::cout << "Sending " << accept << " to " << client_ip << ":" << PORT << std::endl;
			potential_client.send(accept, 26, client_ip, PORT);
			sf::TcpSocket* client = new sf::TcpSocket;
			if (listener.accept(*client) != sf::Socket::Done){
				std::cout << "Connection Failed" << std::endl;
				delete client;
			}
			else{
				std::cout << client_ip << " - connected" << std::endl;
				std::cout << "Sending files" << std::endl;
				clients.push_back(client);
				clients.back()->send(packet);
				std::cout << "Players : " << clients.size() << "/" << player.size() << std::endl << std::endl;
			}
		}
	}while(/*!(instantGetChar() != '\n') && */clients.size() < player.size());
}

char GameServer::instantGetChar(){
#ifdef OS_WINDOWS
		if (_kbhit()) return static_cast<char>(_getch());
		else return 0;
#else
		char key;
		struct termios original_settings, new_settings;
		tcgetattr(0, &original_settings);
		new_settings = original_settings;
		new_settings.c_lflag &= ~ICANON;
		new_settings.c_cc[VMIN] = 1;
		new_settings.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &new_settings);
		key = static_cast<char>(getchar());
		tcsetattr(0, TCSANOW, &original_settings);
		return key;
#endif
}

void GameServer::launch(){
	sf::Packet packet;
	for (size_t i = clients.size() - 1 ; i--;){
		packet.clear();
		packet << static_cast<int>(player_initial_dir[i]);
		clients[i]->send(packet);
	}
	this->start();
}

void GameServer::start(){

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

void GameClient::launch(sf::RenderWindow& game_window){

	sf::UdpSocket broadcast;
	broadcast.setBlocking(false);
	char data[26] = "PapraGame ~ Game request";

	if (broadcast.send(data, 25, sf::IpAddress::Broadcast, PORT) != sf::Socket::Done){

		std::cout << "There was a little problem trying to find the server." << std::endl;
		return;

	} else {

		unsigned short tmpus(PORT);
		size_t tmps_t;
		sf::IpAddress server_ip;
		std::string path;
		sf::UdpSocket fsocket;
		
		fsocket.bind(PORT);

		if (fsocket.receive(data, 26, tmps_t, server_ip, tmpus) != sf::Socket::Done || tmps_t != 26 ||  std::string(data) != "PapraGame ~ Game accepted"){

			std::cout << "Received "<< data << " size : "<< tmps_t<< std::endl;
			std::cout << "There was a problem trying to find the server" << std::endl;
			return;
		}

		if (server.connect(server_ip, PORT) != sf::Socket::Done){

			std::cout << "There was a big problem trying to find the server" << std::endl;
			return;
		}

		std::cout << "Connected to " << server_ip << ":" << PORT << std::endl;

		sf::Packet packet;
		std::vector< std::vector<Area> > map_as_area;
		unsigned char nbr_player;
		std::vector<Coord> player_spawn;

		server.receive(packet);
                         
		packet >> path;

		if (biome_path == "nope"){
			packet >> biome_path;
		} else {
			std::string tmp;
			packet >> tmp;
		}
		packet >> map_width >> map_height;
		biome_path += '/';

		for (unsigned int i = 0 ; i < map_height ; ++i)
			map_as_area.push_back(std::vector<Area>());

		for (unsigned int i = 0 ; i < map_height; ++i) {
			int tmpi;
			for (unsigned int j = 0 ; j < map_width ; ++j) {
				packet >> tmpi;
				map_as_area[i].push_back(static_cast<Area>(tmpi));
			}
		}

		packet >> nbr_player;

		for (unsigned char i = nbr_player; i--;){

			Coord tmpcrd;
			packet >> tmpcrd.x;
			packet >> tmpcrd.y;
			player_spawn.push_back(tmpcrd);

			int tmpi;
			packet >> tmpi;
			player_initial_dir.push_back(static_cast<Direction>(tmpi));
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
				player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], this->loadKeys("player 1"));
			}

			explosion_sprite.setTexture(explosion_texture);

			game_map = Map(map_width, map_height, map_as_area, map_texture, &egg_texture);
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
