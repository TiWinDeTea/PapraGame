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

/**
 * @class Duck Duck.hpp
 * @brief Defines the Duck Class
 */
class Duck : public Duck{

	public:

		/**
		 * @brief Constructor
		 * @param duck_sprites    Names of the files to use as the duck's sprites
		 * @param duckies_sprites Names of the files to use as the duckies' sprites
		 * @param coord           Initials coordinates of the duck
		 */
		Duck(std::string* duck_sprites, std::string* duckies_sprites, Coord coord);

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
		 * @brief Return the number of duckies
		 * @return The number of duckies
		 */
		unsigned char size();

	private:
		
		std::vector<Ducky> Duckies;
		char invulnerability;
};

#endif /*DUCK_HPP_INCLUDED*/