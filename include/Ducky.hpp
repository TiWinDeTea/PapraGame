/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef DUCKY_HPP_INCLUDED
#define DUCKY_HPP_INCLUDED

/**
 * @file Ducky.hpp
 * @author Lucas LAZARE
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 * @brief Defines the Ducky Class
 */

#include <vector>
#include <Coord.hpp>
#include <SFML/Graphics.hpp>
#include <enum.hpp>
#include "Coord.hpp"

/**
 * @class Ducky Ducky.hpp
 * @brief Defines the Ducky Class
 */
class Ducky {

	friend class Duck;

public:

	/**
	 * @brief Ducky destructor
	 */
	~Ducky( ) { };

	/**
	 * @brief Default Ducky constructor
	 */
	Ducky( ) { };

	/**
	 * @brief Ducky constructor with path to sprites and starting_coordinates
	 * @param starting_coordinates Coordinates of the ducky spawn
	 */
	Ducky(Coord starting_coordinates);

	/**
	 * @brief Ducky constructor with path to sprites
	 * @param act_coordinates      Coordinates of the ducky
	 * @param starting_coordinates Coordinates of the ducky spawn
	 * @param dir                  Direction of the ducky.
	 */
	Ducky(Coord act_coordinates, Coord starting_coordinates, Direction dir);

	/**
	 * @brief Moves the ducky
	 * @param new_dir    New direction
	 * @param new_coords New coordinate
	 */
	void move(Direction new_dir, Coord new_coords);

	/**
	 * @brief Moves the ducky
	 * @param new_dir New direction
	 */
	void move(Direction new_dir, unsigned int x_map_size, unsigned int y_map_size);

	/**
	 * @brief Resets the Ducky's coordinate to their original ones
	 */
	void resetCoord( ) { coordinates = st_coordinates; }

	/**
	 * @brief Coordinates getter
	 * @return Ducky's coordinates
	 */
	Coord getCoord( ) { return coordinates; }

	/**
	 * @brief Direction getter
	 * @return Ducky's direction
	 */
	Direction getDirection( ) { return direction; }

private:

	Direction direction;
	Coord coordinates, st_coordinates;
};

#endif /* DUCKY_HPP_INCLUDED */
