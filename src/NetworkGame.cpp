/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

void GameClient::launch(sf::RenderWindow game_window){
	
	sf::UdpSocket broadcast;
	char data[24] = "PapraGame ~ Game request";
	
	if (broadcast.send(data, 24, sf::IpAdress::Broadcast, PORT) != sf::Socket::Done){
		
		std::cout << "There was a little problem trying to find the server." << std::endl;
		return;
		
	} else {
		
		sf::IpAddress server_ip;
		
		if (broadcast.receive(data, 25, server_ip, unsigned short()) != sf::Socket::Done || data != "PapraGame ~ Game accepted"){
			
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
		unsigned char map_height;
		std::vector<std::string> map_as_str;
		unsigned char nbr_player;
		std::vector<Coord> player_spawn;
		
		server.receive(packet);
		if (biome_path == "nope"){
			packet >> biome_path;
		} else {
			packet >> std::string();
		}
		packet >> speed >> map_height;
		biome_path += '/';
		for (unsigned char i = map_height; i-- ;){
			
			std::string tmp;
			packet >> tmp;
			map_as_str.push_back(tmp);
		}
		
		packet >> nbr_player;
		
		for (unsigned char i = nbr_player; i--;){
			
			Coord tmp;
			packet >> tmp;
			player_spawn.push_back(tmp);
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
			winner = 0;
			game_window.create(sf::VideoMode(pxl_length, pxl_height), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);

			for (unsigned char i = player_number ; i-- ;) {
				std::string tmp("player ");
				tmp.push_back(static_cast<char>(i + '1'));
				player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], this->loadKeys(tmp));
			}
		
			explosion_sprite.setTexture(explosion_texture);

			this->start(game_window);
		}			
			
	}
		
}	