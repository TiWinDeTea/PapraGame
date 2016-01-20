/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Duck>

Duck::Duck(sf::Window& game_window, sf::Texture duck_textures[4], sf::Texture duckies_textures[4], Coord starting_coordinates){

	st_coordinates = starting_coordinates;
	coordinates = starting_coordinates;
	for(unsigned char i = 4 ; i--;){
		sprite[i].setTexture(textures[i]);
	}
	direction = UP;
	window = game_window;

	for(unsigned char i = DUCKIES_INITIAL_NUMBER; i--;){
		Duckies.push_back(new Ducky(game_window, duckies_textures[4], starting_coordinates))
	}
}