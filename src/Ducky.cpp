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

void Ducky::move(Direction new_dir){
	direction = new_dir;
	switch(direction){
		case UP:
			--coordinates.y;
			break;
		case DOWN:
			++coordinates.y;
			break;
		case LEFT:
			--coordinates.x;
			break;
		case RIGHT:
			++coordinates.x;
			break;
		default:
			break;
	}
}
