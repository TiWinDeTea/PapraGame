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

#include <Ducky.hpp>

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
		 * @param duck_textures        Textures of the duck. Order : Up, Down, Left, Right
		 * @param duckies_textures     Textures of the duckies. Order : Up, Down, Left, Right
		 * @param starting_coordinates Coordinates of the ducky spawn
		 * @param initial_direction    Direction of the duck at spawn
		 */
		Duck(sf::Texture duck_textures[4], sf::Texture duckies_textures[4], Coord starting_coordinates, Direction initial_direction, std::vector<sf::Keyboard::Key> control_keys);

		/**
		 * @brief Default constructor
		 */
		Duck(){};

		/**
		 * @brief Default destructor
		 */
		~Duck(){};

		/**
		 * @brief Apply damages to the duck and respawn it
		 * @param initial_dir Initial direction of the duck
		 */
		void damaged(Direction initial_dir);

		/**
		 * @brief Add a ducky to the duck family
		 * @param window Game's window
		 */
		void powerUp(sf::RenderWindow& window);

		/**
		 * @brief Moves the duck and the duckies
		 * @param window  Game's window
		 * @param new_dir New direction
		 */
		void move(Direction new_direction, unsigned int x_map_size, unsigned int y_map_size);

		/**
		 * @brief Return the number of duckies
		 * @return The number of duckies
		 */
		unsigned char size();

		/**
		 * @brief Prints the duck and the duckies
		 * @param window Game's window
		 * @param shift  Duck's shift
		 */
		void print(sf::RenderWindow& window, float shift);

		/**
		 * @brief Prints the duck and its duckies
		 * @param window Game's window
		 */
		void print(sf::RenderWindow& window);

		std::vector<sf::Keyboard::Key> keys; // keys order: up, down, left, right

		std::vector<Ducky> duckies;

	private:

		char invulnerability;
		sf::Sprite duck_sprite[4];
		sf::Sprite ducky_sprite[4];
};

#endif /*DUCK_HPP_INCLUDED*/
