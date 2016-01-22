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
#include <vector>

#define NB_TEXTURE 8
#define RESOLUTION_X_IMAGE 32
#define RESOLUTION_Y_IMAGE 32

/**
 * @file Map.hpp
 * @author Julien BARBIER
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 * @brief Define the Map class
 */

/**
 * @class Map Map.hpp
 * @brief Define the Class Map
 */


class Map {

	public :
		/**
		 * @brief Destroy the class Map
		 */
		~Map(){};

		/**
		 * @brief Creation of Map by default.
		 */
		Map (){};

		/**
		 * @brief Creation of map with all the parametre needed.
		 * @param x The lenght of the map
		 * @param y The width of the map
		 * @param smap[x][y] The map himself with the lenght and the width.
		 * @param texture[NB_TEXTURE] Array which will contain the texture for this game.
		 * @param egg_texture Contain the texture of the egg.
		 */
		Map(const unsigned int x, const unsigned int y, std::vector< std::vector<Area> > smap, sf::Texture texture[NB_TEXTURE], sf::Texture* egg_texture);

		/**
		 * @brief Change the coordinates of the egg and print him.
		 * @param egg_coord The coordinate of the egg
		 * @param window The window for sfml and to print the map
		 */
		void popEgg (Coord egg_coord, sf::RenderWindow& window);

		/**
		 * @brief Print a send case in the screen.
		 * @param coord The coordinate of the case we want to convert.
		 * @param window the window for sfml and to print the map.
		 */

		void print(Coord coord, sf::RenderWindow& window);

		/**
		 * @brief printAll Print all the map.
		 * @param window the window for sfml and to print the map
		 */
		void print(sf::RenderWindow& window);
		
		Map& operator=(const Map& mymap);

	private :

		unsigned int x_size;
		unsigned int y_size;
		sf::Sprite sprites[NB_TEXTURE];
		Coord coordinate_egg;
		sf::Sprite egg_sprite;
		std::vector< std::vector<Area> > map;
};

#endif /* MAPP_HPP_INCLUDED */
