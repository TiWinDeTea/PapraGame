/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef COORD_HPP_INCLUDED
#define COORD_HPP_INCLUDED

#include <enum.hpp>

 /**
 * @file Coord.hpp
 * @author Maxime PINARD
 * @version 0.0
 * @license Mozilla Public License, v. .2.0
 * @brief Defines the Coord Class
 */

/**
 * @class Coord Coord.hpp
 * @brief Defines the Coord Class
 */
class Coord{

	public:
		unsigned int x;
		unsigned int y;

		/**
		 * @brief Constructor with coordinates initialisation
		 */
		
                Coord(unsigned int initial_x, unsigned int initial_y);

		/**
		 * @brief Default constructor
		 */
		Coord(){};
};

/**
 * @brief == operator for Coord
 */
bool operator==(const Coord& a, const Coord& b);

/**
 * @brief != operator for Coord
 */
bool operator!=(const Coord& a, const Coord& b);

/**
 * @brief + operator for Coord and Direction
 */
Coord operator+(const Coord& coo, const Direction& dir);

#endif /*COORD_HPP_INCLUDED*/
