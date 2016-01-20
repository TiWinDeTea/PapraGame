/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef DUCK_HPP_INCLUDED
#define DUCK_HPP_INCLUDED

/**
 * @file Duck.hpp
 * @author Maxime PINARD
 * @version 0.0
 * @license Mozilla Public License, v. .2.0
 * @brief Defines the Duck Class
 */

#include <Ducky>

#define DUCKIES_INITIAL_NUMBER 2
#define MOVES_INVULNERABLE 3

/**
 * @class Duck Duck.hpp
 * @brief Defines the Duck Class
 */
class Duck : public Ducky{

	public:

		/**
		 * @brief Duck constructor with path to sprites and starting_coordinates
		 * @param game_window          Window of the game
		 * @param duck_textures        Textures of the duck. Order : Up, Down, Left, Right
		 * @param duckies_textures     Textures of the duckies. Order : Up, Down, Left, Right
		 * @param starting_coordinates Coordinates of the ducky spawn
		 */
		Duck(sf::Window& game_window, sf::Texture duck_textures[4], sf::Texture duckies_textures[4], Coord starting_coordinates);

		/**
		 * @brief Default constructor
		 */
		Duck();

		/**
		 * @brief Default destructor
		 */
		~Duck();

		/**
		 * @brief Apply damages to the duck and respawn it
		 */
		void damaged();

		/**
		 * @brief Add a ducky to the duck familly
		 * @return [description]
		 */
		bool power_up();

		/**
		 * @brief Moves the duck and the duckies
		 * @param new_dir New direction
		 */
		void move();

		/**
		 * @brief Return the number of duckies
		 * @return The number of duckies
		 */
		unsigned char size();

	private:

		std::vector<Ducky> Duckies;
		char invulnerability;
};

#endif /*DUCK_HPP_INCLUDED*/