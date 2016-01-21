/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <Coord.hpp>
#include <SFML/Graphics.hpp>
#include <enum.hpp>

#define NB_TEXTURE 8

/**
 * @file Map.hh
 * @author Julien BARBIER
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 */

class Map {

	public :

		/**
		 * @brief Destroy the class Map
		 */
		~Map(){delete map;}

		/**
		 * @brief Creation of Map by default.
		 */
		Map () {};

		/**
		 * @brief Creation of map with all the parametre needed.
		 * @param x The lenght of the map
		 * @param y The width of the map
		 * @param smap[x][y] The map himself with the lenght and the width.
		 * @param texture[NB_TEXTURE] Array which will contain the texture for this game.
		 * @param egg_texture Contain the texture of the egg.
		 */
		Map(unsigned char x, unsigned char y, Area smap[x][y], sf::Texture texture[NB_TEXTURE], sf::Texture egg_texture);

		/**
		 * @brief Change the coordinates of the egg and print him.
		 * @param egg_coord The coordinate of the egg
		 */
		void popEgg (Coord egg_coord);

		/**
		 * @brief Print a send case in the screen.
		 * @param coord The coordinate of the case we want to convert.
		 */

		void print(Coord coord);

		/**
		 * @brief printAll Print all the map.
		 */
		void printAll();

	private :

		unsigned char x_size;
		unsigned char y_size;
		sf::Sprite sprites[NB_TEXTURE];
		Coord coordinate_egg;
		sf::Sprite egg_sprite;
		Area** map;
};

#endif /* MAPP_HPP_INCLUDED */
