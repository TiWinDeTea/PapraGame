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
};

GameServer::GameServer(std::string ressources, std::string biome_path, std::string map_file_name){

	unsigned int speed;
	std::vector< std::vector<Area> > map_as_area;
	unsigned char nbr_player = 0;
	std::string tmpstr;

	std::ifstream map_file(ressources + map_file_name, std::ios::in | std::ios::binary);
	if (biome_path == "nope")
		map_file >> biome_path;
	else map_file >> tmpstr;
	map_file >> speed >> egg_victory >> map_width >> map_height;

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

	this->getClients(biome_path, map_as_area);
}

void GameServer::getClients(std::string biome_path, std::vector< std::vector<Area> > map_as_area){
	sf::Packet packet;

	packet << biome_path << game_speed << map_width << map_height;
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

	potential_client.setBlocking(false);
	listener.setBlocking(false);

	bool quitted = false;
	char data[25];
	size_t volume_received = 0;

	if (listener.listen(PORT) != sf::Socket::Done) {
		std::cout << "Failed to create the server" << std::endl;
		return;
	}

	std::cout << "Press enter to start the game" << std::endl << std::endl;

	do{
		sf::TcpSocket* client = new sf::TcpSocket;
		sf::IpAddress client_ip;
		unsigned short client_port;
		potential_client.receive(data, 25, volume_received, client_ip, client_port);
		
		sf::sleep(sf::milliseconds(25));

		if (volume_received == 25 && std::string(data) == "PapraGame ~ Game request") {
			std::cout << "Request from : " << client_ip << std::endl;
			potential_client.send("PapraGame ~ Game accepted", 26, client_ip, client_port);
			sf::sleep(sf::milliseconds(10));
			if (listener.accept(*client) != sf::Socket::Done){
				std::cout << "Connection Failed" << std::endl;
				delete client;
			}
			else{
				std::cout << client_ip << ":" << client_port << " - connected" << std::endl;
				std::cout << "Sending files" << std::endl;
				clients.push_back(client);
				clients.back()->send(packet);
			}
		}

	}while(!quitted && clients.size() < player.size());
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


void GameClient::launch(sf::RenderWindow& game_window){

	sf::UdpSocket broadcast;
	char data[26] = "PapraGame ~ Game request";

	if (broadcast.send(data, 25, sf::IpAddress::Broadcast, PORT) != sf::Socket::Done){

		std::cout << "There was a little problem trying to find the server." << std::endl;
		return;

	} else {

		unsigned short tmpus;
		size_t tmps_t;
		sf::IpAddress server_ip;
		std::string path;

		if (broadcast.receive(data, 26, tmps_t, server_ip, tmpus) != sf::Socket::Done || tmps_t != 26 ||  std::string(data) != "PapraGame ~ Game accepted"){

			std::cout << "There was a problem trying to find the server" << std::endl;
			return;
		}

		if (server.connect(server_ip, PORT) != sf::Socket::Done){

			std::cout << "There was a problem trying to find the server" << std::endl;
			return;
		}

		std::cout << "Connected to " << server_ip << ":" << PORT << std::endl;

		sf::Packet packet;
		std::string biome_path;
		unsigned int speed;
		unsigned char map_height, map_width;
		std::vector< std::vector<Area> > map_as_area;
		unsigned char nbr_player;
		std::vector<Coord> player_spawn;
		std::vector<Direction> player_initial_dir;

		server.receive(packet);
		if (biome_path == "nope"){
			packet >> biome_path;
		} else {
			std::string tmp;
			packet >> tmp;
		}
		packet >> speed >> map_width >> map_height;
		biome_path += '/';

		for (unsigned int i = 0 ; i < map_height; ++i) {
			map_as_area.push_back(std::vector<Area>());
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



}
