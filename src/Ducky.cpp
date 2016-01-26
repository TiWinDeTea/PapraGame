/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Ducky.hpp>

Ducky::Ducky(Coord starting_coordinates){
	st_coordinates = starting_coordinates;
	coordinates = starting_coordinates;
	direction = LEFT;
}

Ducky::Ducky(Coord act_coordinates, Coord starting_coordinates, Direction dir){
	st_coordinates = starting_coordinates;
	coordinates = act_coordinates;
	direction = dir;
}

void Ducky::move(Direction new_dir, unsigned int x_map_size, unsigned int y_map_size){
	direction = new_dir;
	switch(direction){
		case UP:
			if(coordinates.y == 0)
				coordinates.y = y_map_size - 1;
			else
				--coordinates.y;
			break;
		case LEFT:
			if(coordinates.x == 0)
				coordinates.x = x_map_size - 1;
			else
				--coordinates.x;
			break;
		case DOWN:
			if(coordinates.y == y_map_size - 1)
				coordinates.y = 0;
			else
				++coordinates.y;
			break;
		case RIGHT:
			if(coordinates.x == x_map_size - 1)
				coordinates.x = 0;
			else
				++coordinates.x;
			break;
		default:
			break;
	}
}
