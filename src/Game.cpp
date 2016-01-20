#include <Game.hpp>

void Game::launch(){
	Coord p[PLAYER_NUMBER];
	p[0].x = 1;
	p[0].y = 1;
	p[1].x = 5;
	p[1].y = 5;
	Area map_tiles[20][20];
	// ~ ?
	for (unsigned int i = 20 ; i-- ; ) {
		for (unsigned int j = 20 ; j--; ) {
			map_tiles[i][j] = ROCK;
		}
	}


	std::string duck_textures_path[4] = {TEXTURE_DUCK_UP, TEXTURE_DUCK_DOWN, TEXTURE_DUCK_LEFT, TEXTURE_DUCK_RIGHT};
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
	char player;

	for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
		player = i + '0';
		for (unsigned int j = 4 ; j-- ;) {
			loading_success = loading_success && duck_texture[i][0][j].loadFromFile(path + duck_textures_pah[j] + player + FILETYPE);
			loading_success = loading_success && duck_textures[i][1][j].loadFromFile(path + ducky_textures_path[j] + player + FILETYPE);
		}
	}
	for (unsigned char i = 8 ; i-- ;) {
		loading_success = loading_success && map_textures[i].loadFromFile(path + map_textures_path[i] + FILETYPE);
	}

	loading_success = loading_success && egg_texture.loadFromFile(path + TEXTURE_EGG + FILETYPE);

	if (!loading_success) {
		std::cout << "Failed to load game textures" << std::endl;
	}
	else {
		winner = 0;
		game_window.create(sf::VideoMode(coo.x, coo.y), "Ducks !", sf::Style::Titlebar | sf::Style::Close);
		for (unsigned int i = PLAYER_NUMBER ; i-- ;) {
			player[i] = Duck(game_window, duck_texture[i][0], duck_texture[i][1], p[i]);
		}
		game_map = Map(20, 20, map_tiles, map_textures, egg_texture);

		this.start();

	}
}
