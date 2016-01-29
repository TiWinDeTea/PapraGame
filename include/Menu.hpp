 /***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

/**
 * @file Menu.hpp
 * @author Lucas LAZARE
 * @version 0.0
 * @license Mozilla Public License, v. 2.0
 * @brief Defines the Menu Class
 */

#include <string>
#include <vector>
#include <iostream>
#include <Coord.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <enum.hpp>
#include <Game.hpp>
#include <dirent.h>
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	#include <windows.h>
	#define OS_WINDOWS
#else
	#include <unistd.h>
	#include <iostream>
	#include <sys/stat.h> 
	#include <sys/types.h>
#endif

#define MENU_X_RESOLUTION             800
#define MENU_Y_RESOLUTION             600

#define TITLE_HEIGHT                  200

#define LOCAL_TXT                     "Local"
#define NETWORK_TXT                   "Network"
#define EXIT_TXT                      "Exit"
#define MAIN_FONT_SIZE                 44
#define MAP_FONT_SIZE                  30

#define MAIN_BUTTON_HEIGHT            50
#define MAIN_BUTTON_WIDTH             400
#define MAIN_BUTTON_SPACE             30
#define MAP_BUTTON_HEIGHT             40
#define MAP_BUTTON_WIDTH              600
#define MAP_BUTTON_SPACE              30
#define SELECTION_OUTLINE_THICKNESS   7

#define BUTTON_FILL_COLOR             sf::Color(100, 250, 50)
#define OUTLINE_COLOR                 sf::Color(250, 150, 100)
#define TXT_COLOR                     sf::Color::Red

#define MAPS_PATH                     "maps/"
#define RES_PATH                      "res/"
#define BIOME_PATH                    "none"
#define DUCKS_PATH                    "ducks/"

/**
 * @class Menu Menu.hpp
 * @brief Defines the Menu Class
 */
class Menu{

	public:

		Menu();

		void setBiome(std::string chosen_biome);

		~Menu(){};

		void mainMenu();

		std::string mapMenu();

	private:

		sf::RenderWindow window;
		sf::Font font;

		std::string res_path, biome_path, ducks_path, maps_path, map_path;
};

#endif /* MENU_HPP_INCLUDED */
