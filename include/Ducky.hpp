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
#include <SFML/Graphics>
#include <enum>

/**
 * @class Ducky Ducky.hpp
 * @brief Defines the Ducky Class
 */
class Ducky{

	public:

		/**
		 * @brief Ducky destructor
		 */
		~Ducky(){};

		/**
		 * @brief Default Ducky constructor
		 */
		Ducky(){};

		/**
		 * @brief Ducky constructor with path to sprites and starting_coordinates
		 * @param game_window          Window of the game
		 * @param textures             Textures of the ducky. Order : Up, Down, Left, Right
		 * @param starting_coordinates Coordinates of the ducky spawn
		 */
		Ducky(sf::Window& game_window, sf::Texture textures[4], Coord starting_coordinates);

		/**
		 * @brief Ducky constructor with path to sprites
		 * @param game_window          Window of the game
		 * @param textures             Textures of the ducky. Order : Up, Down, Left, Right
		 * @param act_coordinates      Coordinates of the ducky
		 * @param starting_coordinates Coordinates of the ducky spawn
		 * @param dir                  Direction of the ducky.
		 */
		Ducky(sf::Window& game_window, sf::Texture textures[4], Coord act_coordinates, Coord starting_coordinates, Direction dir);

		/**
		 * @brief Prints the ducky
		 */
		void print();

		/**
		 * @brief Moves the ducky
		 * @param new_dir New direction
		 */
		void move(Direction new_dir);

		/**
		 * @brief Resets the Ducky's coordinate to their original ones
		 */
		void resetCoord(){ coordinates = st_coordinates; }

		/**
		 * @brief Coordinates getter
		 * @return Ducky's coordinates
		 */
		Coord getCoord(){ return Coordinates; }

	protected:
		
		Coord coordinates, st_coordinates;
		Direction direction;

	private:

		sf::Sprite sprite[4];
		sf::Window window;
};

#endif /* DUCKY_HPP_INCLUDED */
