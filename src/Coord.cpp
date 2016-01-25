/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Coord.hpp>

Coord::Coord(unsigned int initial_x, unsigned int initial_y){
    
    x = initial_x;
    y = initial_y;
}

bool operator==(const Coord& a, const Coord& b)
{
    return (a.x == b.x && a.y == b.y);
}

bool operator!=(const Coord& a, const Coord& b)
{
    return !(a == b);
}

Coord& Coord::operator=(const Coord& coord){
	x = coord.x;
	y = coord.y;
	return *this;
}

Coord& Coord::operator+=(const Direction& dir){
	*this = *this + dir;
	return *this;
}

Coord& Coord::operator-=(const Direction& dir){
	*this = *this - dir;
	return *this;
}

Coord operator+(const Coord& coord, const Direction& dir)
{
	Coord new_coord = coord;
	switch(dir){
		case UP:
			--new_coord.y;
			break;
		case LEFT:
			--new_coord.x;
			break;
		case DOWN:
			++new_coord.y;
			break;
		case RIGHT:
			++new_coord.x;
			break;
		default:
			break;
	}
	return new_coord;
}

Coord operator-(const Coord& coord, const Direction& dir)
{
	Coord new_coord = coord;
	switch(dir){
		case UP:
			++new_coord.y;
			break;
		case LEFT:
			++new_coord.x;
			break;
		case DOWN:
			--new_coord.y;
			break;
		case RIGHT:
			--new_coord.x;
			break;
		default:
			break;
	}
	return new_coord;
}