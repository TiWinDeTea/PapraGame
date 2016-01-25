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

void Duck::damaged(Direction initial_dir){

	if(this->size() > 0){

		duckies.pop_back();
		coordinates = st_coordinates;
		invulnerability = MOVES_INVULNERABLE;
		direction = initial_dir;


		for(unsigned char i = 0; i < static_cast<unsigned char>(duckies.size()); ++i){
			duckies[i].resetCoord();
		}
	}
}

void Duck::powerUp(sf::RenderWindow& window){

	if(duckies.size() > 0)
		duckies.push_back(Ducky(duckies.back().coordinates, st_coordinates, duckies.back().direction));
	else
		duckies.push_back(Ducky(coordinates, st_coordinates, direction));
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

	coordinates += new_direction;
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
