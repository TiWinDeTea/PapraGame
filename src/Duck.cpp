/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Duck.hpp>

Duck::Duck(sf::Texture duck_textures[4], sf::Texture duckies_textures[4], Coord starting_coordinates, Direction initial_direction, std::vector<sf::Keyboard::Key> control_keys){

	keys = control_keys;
	st_coordinates = starting_coordinates;
	coordinates = starting_coordinates;
	for(unsigned char i = 4 ; i--;){
		duck_sprite[i].setTexture(duck_textures[i]);
	}
	direction = initial_direction;
	for(unsigned char i = 4 ; i--;){
		ducky_sprite[i].setTexture(duckies_textures[i]);
	}
	direction = initial_direction;

	for(unsigned char i = DUCKIES_INITIAL_NUMBER; i--;){
		duckies.push_back(Ducky(starting_coordinates, starting_coordinates, direction));
	}
}

void Duck::damaged(){

	duckies.pop_back();
	coordinates = st_coordinates;
	invulnerability = MOVES_INVULNERABLE;

	for(unsigned char i = static_cast<unsigned char>(duckies.size()); i--;){
		duckies[i].resetCoord();
	}
}

void Duck::powerUp(sf::RenderWindow& window){

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
		default:
			new_dir = LEFT;
			--new_coord.x;
	}
	duckies.push_back(Ducky(new_coord, st_coordinates, new_dir));
	ducky_sprite[duckies.back().direction].setPosition(static_cast<float>(duckies.back().coordinates.x * 32), static_cast<float>(duckies.back().coordinates.y * 32));
	window.draw(ducky_sprite[duckies.back().direction]);
}

void Duck::move(sf::RenderWindow& window, Direction new_direction){
	for(unsigned char i = static_cast<unsigned char>(duckies.size() - 1); i > 0 ; i--){
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
}

unsigned char Duck::size(){

	return static_cast<unsigned char>(duckies.size());
}

void Duck::print(sf::RenderWindow& window, float shift){

	for(unsigned char i = static_cast<unsigned char>(duckies.size()); i--;){
		switch (duckies[i].direction){
			case UP:
				ducky_sprite[duckies[i].direction].setPosition(static_cast<float>(duckies[i].coordinates.x * 32), static_cast<float>(duckies[i].coordinates.y * 32) - shift);
				break;
			case LEFT:
				ducky_sprite[duckies[i].direction].setPosition(static_cast<float>(duckies[i].coordinates.x * 32) - shift, static_cast<float>(duckies[i].coordinates.y * 32));
				break;
			case DOWN:
				ducky_sprite[duckies[i].direction].setPosition(static_cast<float>(duckies[i].coordinates.x * 32), static_cast<float>(duckies[i].coordinates.y * 32) + shift);
				break;
			case RIGHT:
				ducky_sprite[duckies[i].direction].setPosition(static_cast<float>(duckies[i].coordinates.x * 32) + shift, static_cast<float>(duckies[i].coordinates.y * 32));
				break;
			default:
				break;
		}
		window.draw(ducky_sprite[duckies[i].direction]);
	}

	switch (direction){
		case UP:
			duck_sprite[direction].setPosition(static_cast<float>(coordinates.x * 32), static_cast<float>(coordinates.y * 32) - shift);
			break;
		case LEFT:
			duck_sprite[direction].setPosition(static_cast<float>(coordinates.x * 32) - shift, static_cast<float>(coordinates.y * 32));
			break;
		case DOWN:
			duck_sprite[direction].setPosition(static_cast<float>(coordinates.x * 32), static_cast<float>(coordinates.y * 32) + shift);
			break;
		case RIGHT:
			duck_sprite[direction].setPosition(static_cast<float>(coordinates.x * 32) + shift, static_cast<float>(coordinates.y * 32));
			break;
		default:
			break;
	}
	window.draw(duck_sprite[direction]);
}

void Duck::print(sf::RenderWindow& window){
	this->print(window, -32);
}
