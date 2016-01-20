#include <Game.hpp>

void Game::launch(){
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
		for (unsigned int j = PLAYER_NUMBER ; j-- ;) {
			loading_success = loading_success && duck_texture[j][0][i].loadFromFile(path + duck_textures_pah[j] + player + FILETYPE);
			loading_success = loading_success && duck_textures[j][1][i].loadFromFile(path + ducky_textures_path[j] + player + FILETYPE);
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
		
	}
}
