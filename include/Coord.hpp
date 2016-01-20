/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef COORD_HPP_INCLUDED
#define COORD_HPP_INCLUDED

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
		Coord(unsigned char initial_x, unsigned char initial_y){x=initial_x;y=initial_y;};

		/**
		 * @brief Default constructor
		 */
		Coord();
};

#endif /*COORD_HPP_INCLUDED*/
