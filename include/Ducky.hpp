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
 * @license Mozilla Public License, v. .2.0
 * @brief Defines the Ducky Class
 */

#include <vector>
#include <Coord>
#include <SFML/Graphics>
#include <string>
#include <enum>

/**
 * @class Ducky Ducky.hpp
 * @brief Defines the Ducky Class
 */
class Ducky{

public:

	/**
	 * @brief Ducky destructor
	 * 	just closes the textures
	 */
	~Ducky();
	/**
	 * @brief Default Ducky constructor
	 */
	Ducky(){};
	/**
	 * @brief Ducky constructor with path to sprites and starting_coordinates
	 * @param sprites_path         Path to the sprites. Order : Up, Down, Left, Right
	 * @param starting_coordinates Coordinates of the ducky departure
	 */
	Ducky(string sprites_path[4], Coord starting_coordinates);

	/**
	 * @brief Prints the ducky
	 */
	void print();

	/**
	 * @brief Moves the ducky
	 */
	void move();

	/**
	 * @brief Sets the Ducky's direction
	 * @param new_dir New direction
	 */
	void setDirection(Direction new_dir);

	/**
	 * @brief Resets the Ducky's coordinate to their original ones
	 */
	void resetCoord(){ coordinates = st_coordinates; }

	/**
	 * @brief Coordinates getter
	 * @return Ducky's coordinates
	 */
	Coord getCoord(){ return Coordinates; }

private:

	Coord coordinates;
	Direction direction;
	sf::Texture sprites[4];
};

#endif /* DUCKY_HPP_INCLUDED */
