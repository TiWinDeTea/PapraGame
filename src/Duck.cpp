/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Duck.hpp>

Duck::Duck(sf::Window& game_window, sf::Texture duck_textures[4], sf::Texture duckies_textures[4], Coord starting_coordinates, Direction initial_direction){

	st_coordinates = starting_coordinates;
	coordinates = starting_coordinates;
	for(unsigned char i = 4 ; i--;){
		sprite[i].setTexture(textures[i]);
	}
	direction = initial_direction;
	window = game_window;

	for(unsigned char i = DUCKIES_INITIAL_NUMBER; i--;){
		Duckies.push_back(new Ducky(game_window, duckies_textures[4], starting_coordinates))
	}
}

void Duck::damaged(){

	duckies.pop_back();
	coordinates = st_coordinates;
	invulnerability = MOVES_INVULNERABLE;

	for(unsigned char i = duckies.size(); i--;){
		Duckies.resetCoord();
	}
}

void Duck::power_up(){

	Coord new_coord = duckies.back().coordinates;
	Direction new_dir;
	switch(duckies.back().direction){
		case UP:
			new_dir = DOWN;
			++new_coord.y;
			break;
		case LEFT:
			new_dir = RIGHT;
			++new_coord.x;
			break;
		case DOWN:
			new_dir = UP;
			--new_coord.y;
			break;
		case RIGHT:
			new_dir = LEFT;
			--new_coord.x;
			break;
		default:
			break;
	}
	duckies.push_back(new Ducky(game_window, duckies_textures[4], new_coord, starting_coordinates, new_dir));
	duckies.back().print();
}

void Duck::move(new_direction){

	for(unsigned char i = duckies.size(); i > 0; --i){
		if(duckies[i].coordinates != duckies[i - 1].coordinates)
			duckies[i].move(duckies[i - 1].direction);
	}
	if(duckies.front().coordinates != coordinates){
		duckies.front().move(direction);
	}

	switch(new_direction){
		case UP:
			--coordinates.y;
			break;
		case LEFT:
			--coordinates.x;
			break;
		case DOWN:
			++coordinates.y;
			break;
		case RIGHT:
			++coordinates.x;
			break;
		default:
			break;
	}
	direction = new_direction;
	this.print();
}

unsigned char Duck::size(){

	return static_cast<unsigned char>(duckies.size());
}

void Duck::print(){

	for(unsigned char i = duckies.size(); i--;){
		ducky_sprite[duckies[i].direction].setPosition(duckies[i].coordinates.x * 32, duckies[i].coordinates.y * 32);
		window.draw(ducky_sprite[duckies[i].direction]);
	}

	duck_sprite[direction].setPosition(coordinates.x * 32, coordinates.y * 32);
	window.draw(duck_sprite[direction]);
}