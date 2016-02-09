/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <NetworkGame.hpp>

static void printExplosion(sf::RenderWindow& game_window, Coord coord, sf::Sprite* explosion_sprite);
static sf::Socket::Status receiveWithTimeout(sf::UdpSocket& socket, char* data, size_t max_received_size, size_t& received_size, sf::IpAddress& remote, unsigned short port, sf::Time timeout);

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
	if (biome_path == "none")
		map_file >> biome_path;
	else map_file >> tmpstr;
	map_file >> tmpstr;
	if (tmpstr ==  "blind" || tmpstr == "true" || tmpstr == "1"){
		is_blind = true;
		map_file >> los;
		map_file >> tmpstr;
		if (tmpstr == "loop" || tmpstr == "true" || tmpstr == "1")
			loop = true;
		else
			loop = false;
	}
	else{
		los = 100;
		loop = true;
		is_blind = false;
	}
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
		--player_spawn[nbr_player].y;
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
}

bool GameServer::getClients(sf::RenderWindow& window){
	sf::Packet packet;
	packet.clear();
	packet << static_cast<unsigned int>(player.size()) << is_blind << los << loop << map_height << map_width << game_speed << ressources << biome_path;

	for(unsigned int i = 0 ; i < player.size() ; ++i){
		packet << player_spawn[i].x << player_spawn[i].y << static_cast<int>(player_initial_dir[i]);
	}

	for(unsigned int i = 0 ; i < map_width ; ++i){
		for (unsigned int j = 0 ; j < map_height ; ++j) {
			packet << static_cast<int>(game_map.map[i][j]);
		}
	}

	sf::UdpSocket udp_client;
	udp_client.bind(PORT);

	sf::TcpListener listener;
	if (listener.listen(PORT) != sf::Socket::Done){
		std::cout << "Failed to host the game" << std::endl;
		return false;
	}

	char retrieved_data[25];
	size_t retrieved_data_size;
	unsigned short client_port;
	sf::IpAddress client_ip;
	sf::Socket::Status status;

	std::cout << "Server : starting up" << std::endl;

	size_t tmp_st(player.size() - 1);
	if (tmp_st > 0)
		std::cout << "Waiting for " << tmp_st << " player" << ((tmp_st > 1) ? "s" : "") << std::endl << std::endl;

	udp_client.setBlocking(false);

	sf::Event event;

	while(tmp_st > 0 && window.isOpen()){
		status = udp_client.receive(retrieved_data, 25, retrieved_data_size, client_ip, client_port);

		if (status == sf::Socket::Done && retrieved_data_size == 25 && std::string(retrieved_data) == "PapraGame ~ Game Request" && client_port == (PORT + 1)) {

			std::cout << "Request from " << client_ip << ":" << client_port << std::endl;
			sf::TcpSocket* client = new sf::TcpSocket;
			status = udp_client.send("PapraGame ~ Client Accepted", 28, client_ip, client_port);

			if (listener.accept(*client) == sf::Socket::Done) {

				std::cout << "+ Connected" << std::endl;
				clients.push_back(client);
				std::cout << "+ Sending maps infos" << std::endl;
				client->send(packet);
				std::cout << "+ Done" << std::endl << std::endl;

				--tmp_st;

				if (tmp_st > 0) {
					std::cout << "Waiting for " << tmp_st << " extra player" << ((tmp_st > 1) ? "s" : "") << std::endl;
				}
			}
			else{
				std::cout << "- Connection failed" << std::endl;
				delete client;
			}
		}

		sf::sleep(sf::milliseconds(30));
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
				return false;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return false;
		}
	}
	return true;
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
	biome_path.append("/");
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

	for (unsigned int i = static_cast<int>(player.size()); i--;){
		sf::Texture** texture_array = (sf::Texture**) calloc(2, sizeof(sf::Texture*));
		texture_array[0] = (sf::Texture*) calloc(4, sizeof(sf::Texture));
		texture_array[1] = (sf::Texture*) calloc(4, sizeof(sf::Texture));
		duck_texture.push_back(texture_array);
	}

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
		game_window.setSize(sf::Vector2u(map_width * 32, map_height * 32));
		game_window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(map_width * 32), static_cast<float>(map_height * 32))));

		for (unsigned int i = static_cast<int>(player.size()) ; i-- ;) {
			std::string tmp("player ");
			tmp.push_back(static_cast<char>(i + '1'));
			player[i] = Duck(duck_texture[i][0], duck_texture[i][1], player_spawn[i], player_initial_dir[i], this->loadKeys("player 1"));
		}
		game_map = Map(map_width, map_height, game_map.map, map_texture, &egg_texture);

		for (size_t i = clients.size() ; i--;){
			packet.clear();
			packet << true << static_cast<int>(player_initial_dir[i]);
			clients[i]->send(packet);
		}
		this->start(game_window);
	}
}

void GameServer::start(sf::RenderWindow& game_window){

	sf::SoundBuffer damage_buffer, egg_buffer, warp_buffer;
	damage_buffer.loadFromFile("res/sounds/damaged.ogg");
	egg_buffer.loadFromFile("res/sounds/new_egg.ogg");
	warp_buffer.loadFromFile("res/sounds/warp.ogg");

	sf::Sound damage_sound, egg_sound, warp_sound;
	damage_sound.setBuffer(damage_buffer);
	egg_sound.setBuffer(egg_buffer);
	warp_sound.setBuffer(warp_buffer);

	sf::Music game_theme;
	game_theme.openFromFile("res/sounds/game_theme.ogg");
	game_theme.setLoop(true);
	game_theme.play();

	sf::Sprite explosion_sprite;
	explosion_sprite.setTexture(explosion_texture);

	std::cout << "Game's starting" << std::endl;
	std::vector<Direction> player_dir;
	std::vector<Coord> explosions_coord;

	for (size_t i = 0 ; i < player_initial_dir.size() ; ++i) {
		player_dir.push_back(player_initial_dir[i]);
	}

	unsigned char winner(0);
	sf::Packet packet;

	game_map.init();

	game_map.popEgg();
	game_map.popEgg(); // Map thuging #2

	packet << game_map.getEggCoord().x << game_map.getEggCoord().y;
	for (size_t i = clients.size() ; i-- ;) {
		clients[i]->send(packet);
	}
	packet.clear();

	Direction self_current_dir;

	do{
		packet << false;

		for(unsigned char i((unsigned char)(player.size())); i--;){

			if(game_map.isWarp(player[i].getCoord())){
				player[i].warped(game_map.getWarp(player[i].getCoord()));
				warp_sound.play();
			}

			player[i].move(player_dir[i], game_map.x_size, game_map.y_size);

			bool damaged(false);
			unsigned int j = static_cast<unsigned int>(player.size());
			explosions_coord.erase(explosions_coord.begin(), explosions_coord.end());
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
						packet << true << static_cast<int>(65000);
					}
				}

				unsigned char k = player[j].size();
				while(k > 0 && !damaged){
					--k;
					if(player[i].getCoord() == player[j].duckies[k].getCoord()){
						packet << true;
						explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());
						if (player[i].size() > 0 && !(player[i].isInvulnerable())){
							player[j].powerUp();
							packet << j;
							if (player[j].size() == egg_victory && egg_victory != 0)
									winner = static_cast<unsigned char>(j + 1);
						}
						else
							packet << 65000;
						player[i].damaged(player_initial_dir[i]);
						player_dir[i] = player_initial_dir[i];
						damaged = true;
					}
				}
			}
			if(game_map.map[player[i].getCoord().x][player[i].getCoord().y] == OBSTACLE){
				explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());
				player[i].damaged(player_initial_dir[i]);
				player_dir[i] = player_initial_dir[i];
				damaged = true;
				packet << true << static_cast<int>(65000);
			}

			if (!damaged){

				packet << false << player_dir[i];

				if(player[i].getCoord() == game_map.getEggCoord()){
					egg_sound.play();
					player[i].powerUp();

					if (player[i].size() == egg_victory && egg_victory != 0) {
						winner = static_cast<unsigned char>(i + 1);
					}
					game_map.popEgg();

					packet << true << game_map.getEggCoord().x << game_map.getEggCoord().y;
				}
				else{
					packet << false;
				}
			}
			else{
					damage_sound.play();
			}
		}

		for (unsigned char i((unsigned char)(clients.size())) ; i-- ;) {
			clients[i]->send(packet);
		}

		self_current_dir = player_dir[player.size()-1];

		for (unsigned char w = 16 ; --w ;) {

			sf::Event event;
			game_window.clear();

			while (game_window.pollEvent(event)){
				if(event.type == sf::Event::Closed){
					game_window.close();
					return;
				}
				else if(event.type == sf::Event::KeyPressed){
					if(event.key.code == player[player.size()-1].keys[0] && (self_current_dir != DOWN || player[player.size()-1].size() == 0))
						player_dir[player.size()-1] = UP;

					else if(event.key.code == player[player.size()-1].keys[1] && (self_current_dir != UP || player[player.size()-1].size() == 0))
						player_dir[player.size()-1] = DOWN;

					else if(event.key.code == player[player.size()-1].keys[2] && (self_current_dir != RIGHT || player[player.size()-1].size() == 0))
						player_dir[player.size()-1] = LEFT;

					else if(event.key.code == player[player.size()-1].keys[3] && (self_current_dir != LEFT || player[player.size()-1].size() == 0))
						player_dir[player.size()-1] = RIGHT;
				}
			}


			if (is_blind){
				game_window.clear();
				for (unsigned char i = player.size() ; i--;){
					game_map.print(game_window, player[i].getCoord(), los, loop);
					for (unsigned char j = player[i].size() ; j-- ;){
						game_map.print(game_window, player[i].duckies[j].getCoord(), static_cast<unsigned short>(los/2), loop);
					}
				}
				game_map.printEgg(game_window);
			}
			else game_map.print(game_window);

			for (unsigned int i = static_cast<unsigned int>(player.size()) ; i-- ;) {
				if(player[i].isInvulnerable()){
					if(w < 8)
						player[i].print(game_window, -static_cast<float>(w) * 2);
				}
				else{
					player[i].print(game_window, -static_cast<float>(w) * 2);
				}
			}

			for(unsigned int i = static_cast<unsigned int>(explosions_coord.size()); i--;)
				printExplosion(game_window, explosions_coord[i], &explosion_sprite);
			game_window.display();
			sf::sleep(sf::milliseconds(game_speed));
		}

		for (unsigned char i((unsigned char)(clients.size())) ; i-- ;) {
			packet.clear();
			clients[i]->receive(packet);
			int dir_as_int;
			packet >> dir_as_int;
			if (dir_as_int != NOPE && ((player_dir[i] + dir_as_int != 1 && player_dir[i] + dir_as_int != 5) || player[i].size() == 0)) {
				player_dir[i] = static_cast<Direction>(dir_as_int);
			}
		}

		packet.clear();
	}while(winner == 0);

	sf::Packet packet_for_the_winner;

	packet << true << false;
	packet_for_the_winner << true << true;

	for (size_t i(clients.size()) ; i-- ;) {
		if ( i+1 != winner)
			clients[i]->send(packet);
		else
			clients[i]->send(packet_for_the_winner);
	}

	for (size_t i = duck_texture.size() ; i-- ;) {
		free(duck_texture[i][0]);
		free(duck_texture[i][1]);
		free(duck_texture[i]);
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

GameClient::~GameClient(){

	for (size_t i = player.size() ; i-- ;) {
		free(duck_texture[i][0]);
		free(duck_texture[i][1]);
		free(duck_texture[i]);
	}
}

void GameClient::launch(sf::RenderWindow& game_window){

	sf::UdpSocket broadcast;
	broadcast.bind(PORT + 1);
	size_t received;
	sf::IpAddress sender;
	unsigned short port(0);
	char data[28];

	if (broadcast.send("PapraGame ~ Game Request", 25, sf::IpAddress::Broadcast, PORT) != sf::Socket::Done){
		std::cout << "- Broadcast failure" << std::endl;
		return;
	} else {
		bool server_found(false);
		unsigned char client_loopout(0);
		do{
			sf::Event event;
			while(game_window.pollEvent(event)){
				if (event.type == sf::Event::Closed){
						game_window.close();
						return;
				}
				else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
					return;
			}
			if(receiveWithTimeout(broadcast, data, 28, received, sender, port, CLIENT_CONNECTION_REFRESH_RATE) == sf::Socket::Done){
				if (received != 28 || std::string(data) != "PapraGame ~ Client Accepted"){
					++client_loopout;
					std::cout << "- Bad server : " << sender << std::endl;
				}
				else
					server_found = true;
			}
			else{
				++client_loopout;
				if (client_loopout > CLIENT_CONNECTION_ATTEMPT_NBR){
					std::cout << "- No server found" << std::endl;
					return;
				}
				else
					 broadcast.send("PapraGame ~ Game Request", 25, sf::IpAddress::Broadcast, PORT);
			}
		}while(!server_found);

		std::cout << "- Server found : " << sender << std::endl;

		if (server.connect(sender, PORT, CLIENT_CONNECTION_TIMEOUT) != sf::Socket::Done){
			std::cout << "- Connection Failed" << std::endl;
			return;
		} else {

			std::cout << "+ Connected" << std::endl;
			unsigned int nbr_player;
			sf::Packet packet;

			server.receive(packet);
			packet >> nbr_player >> is_blind >> los >> loop >> map_width >> map_height >> game_speed;

			std::string path;
			std::vector<Coord> spawn;

			std::vector<std::vector<Area> > tmp_map;
			packet >> path >> biome_path;
			biome_path.append("/");

			for (unsigned int i = nbr_player; i--;) {
				player.push_back(Duck());
				spawn.push_back(Coord());
				player_initial_dir.push_back(Direction());
				sf::Texture** texture_array = (sf::Texture**) calloc (2, sizeof(sf::Texture*));
				texture_array[0] = (sf::Texture*) calloc (4, sizeof(sf::Texture));
				texture_array[1] = (sf::Texture*) calloc (4, sizeof(sf::Texture));
				duck_texture.push_back(texture_array);
			}
			for (unsigned int i = map_height; i--;){
				tmp_map.push_back(std::vector<Area>());
			}
			for (unsigned int i = 0; i < nbr_player; ++i){
				packet >> spawn[i].x >> spawn[i].y;
				int tmp;
				packet >> tmp;
				player_initial_dir[i] = static_cast<Direction>(tmp);
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
			std::cout << std::endl;
			bool loading_success = true;
			for (unsigned char i = 9 ; i-- ;) {
				loading_success = loading_success && map_texture[i].loadFromFile(path + map_textures_path[i] + FILETYPE);
			}

			loading_success = loading_success && egg_texture.loadFromFile(path + DUCK_PATH + TEXTURE_EGG + FILETYPE);
			loading_success = loading_success && explosion_texture.loadFromFile(path + DUCK_PATH + TEXTURE_EXPLOSION + FILETYPE);

			char player_id;
			for (unsigned int i = nbr_player ; i-- ;) {
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
				game_window.setSize(sf::Vector2u(map_width*32, map_height*32));
				game_window.setView(sf::View(sf::FloatRect(0,0,static_cast<float>(map_width*32), static_cast<float>(map_height*32))));
				for (unsigned int i = nbr_player ; i-- ;) {
					player[i] = Duck(duck_texture[i][0], duck_texture[i][1], spawn[i], player_initial_dir[i], this->loadKeys("player 1"));
				}
				explosion_sprite.setTexture(explosion_texture);
				game_map = Map(map_width, map_height, tmp_map, map_texture, &egg_texture);

				packet.clear();
				bool starting, done(false);
				sf::Event event;
				server.setBlocking(false);

				do{
					while(game_window.pollEvent(event)){
						if (event.type == sf::Event::Closed){
								game_window.close();
								done = true;
						}
					}
					server.receive(packet);
					if((packet >> starting)){
						if (starting) {
							int tmp;
							packet >> tmp;
							direction = static_cast<Direction>(tmp);
							server.setBlocking(true);
							this->start(game_window);
							done = true;
						} else {
							std::cout << "Connection closed by server." << std::endl;
							done = true;
						}
					}
				}while(!done);
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

	/* TODO : victory // defeat screen */
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

	packet.clear();
	server.receive(packet);
	packet >> egg_x >> egg_y;
	game_map.popEgg(Coord(egg_x, egg_y));

	std::vector<Coord> explosions_coord;
	game_window.clear();

	sf::SoundBuffer damage_buffer, egg_buffer, warp_buffer;
	damage_buffer.loadFromFile("res/sounds/damaged.ogg");
	egg_buffer.loadFromFile("res/sounds/new_egg.ogg");
	warp_buffer.loadFromFile("res/sounds/warp.ogg");

	sf::Sound damage_sound, egg_sound, warp_sound;
	damage_sound.setBuffer(damage_buffer);
	egg_sound.setBuffer(egg_buffer);
	warp_sound.setBuffer(warp_buffer);

	sf::Music game_theme;
	game_theme.openFromFile("res/sounds/game_theme.ogg");
	game_theme.setLoop(true);
	game_theme.play();

	do {
		explosions_coord.erase(explosions_coord.begin(), explosions_coord.end());

		packet.clear();
		server.receive(packet);
		packet >> ended;
		if (ended) {
			packet >> winner;
		} else {
			int ducky_stolen;
			for (unsigned int i = static_cast<int>(player.size()); i--;){
				packet >> damaged;
				if (!damaged){
					Direction tmp_dir;
					packet >> tmpint >> power_up;
					tmp_dir = static_cast<Direction>(tmpint);
					player[i].move(tmp_dir,map_width, map_height);
					if (game_map.isWarp(player[i].getCoord())){
						player[i].warped(game_map.getWarp(player[i].getCoord()));
						warp_sound.play();
					}
					if (power_up){
						packet >> egg_x >> egg_y;
						game_map.popEgg(Coord(egg_x,egg_y));
						player[i].powerUp();
						egg_sound.play();
					}
				} else {
					packet >> ducky_stolen;
					if (ducky_stolen != 65000)
						player[ducky_stolen].powerUp();
					explosions_coord.push_back(player[i].getCoord() - player[i].getDirection());
					player[i].damaged(player_initial_dir[i]);
					damage_sound.play();
				}
			}
			packet.clear();

			for (unsigned char w = 17 ; --w ;) {

				game_window.clear();

				while (game_window.pollEvent(event)){
					if(event.type == sf::Event::Closed){
						game_window.close();
						packet << static_cast<int>(NOPE);
						server.send(packet);
						return;
					}
					else if(event.type == sf::Event::KeyPressed){
						if(event.key.code == player[0].keys[0])
							direction = UP;
						else if(event.key.code == player[0].keys[1])
							direction = DOWN;
						else if(event.key.code == player[0].keys[2])
							direction = LEFT;
						else if(event.key.code == player[0].keys[3])
							direction = RIGHT;
					}
				}


				if (is_blind){
					game_window.clear();
					for (unsigned char i = player.size(); i--;){
						game_map.print(game_window, player[i].getCoord(), los, loop);
						for (unsigned char j = player[i].size() ; j-- ;){
							game_map.print(game_window, player[i].duckies[j].getCoord(), static_cast<unsigned short>(los/2), loop);
						}
					}
					game_map.printEgg(game_window);
				}
				else game_map.print(game_window);

				for (unsigned int i = static_cast<unsigned int>(player.size()) ; i-- ;) {
					if(player[i].isInvulnerable()){
						if(w < 8)
							player[i].print(game_window, -static_cast<float>(w) * 2);
					}
					else{
						player[i].print(game_window, -static_cast<float>(w) * 2);
					}
				}

				for(unsigned int i = static_cast<unsigned int>(explosions_coord.size()); i--;)
					printExplosion(game_window, explosions_coord[i], &explosion_sprite);

				game_window.display();
				sf::sleep(sf::milliseconds(game_speed));
			}

			for(unsigned int i = static_cast<unsigned int>(explosions_coord.size()); i--;)
				printExplosion(game_window, explosions_coord[i], &explosion_sprite);

			packet << static_cast<int>(direction);
			server.send(packet);

			if (is_blind){
				game_window.clear();
				for (unsigned char i = player.size() ; i--;){
					game_map.print(game_window, player[i].getCoord(), los, loop);
					for (unsigned char j = player[i].size() ; j-- ;){
						game_map.print(game_window, player[i].duckies[j].getCoord(), static_cast<unsigned short>(los/2), loop);
					}
				}
				game_map.printEgg(game_window);
			}
			else game_map.print(game_window);
			for (unsigned int i = static_cast<int>(player.size()); i--;){
				player[i].print(game_window, 0);
			}
			game_window.display();
		}
	} while (!ended && game_window.isOpen());
	return;
}

static void printExplosion(sf::RenderWindow& game_window, Coord coord, sf::Sprite* explosion_sprite){
	explosion_sprite->setPosition(static_cast<float>(coord.x * 32), static_cast<float>(coord.y * 32));
	game_window.draw(*explosion_sprite);
}

static sf::Socket::Status receiveWithTimeout(sf::UdpSocket& socket, char* data, size_t max_received_size, size_t& received_size, sf::IpAddress& remote, unsigned short port, sf::Time timeout){
    sf::SocketSelector selector;
    selector.add(socket);
    if (selector.wait(timeout))
        return socket.receive(data, max_received_size, received_size, remote, port);
    else
        return sf::Socket::NotReady;
}
