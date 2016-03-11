/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef MAPEDITOR_HPP_INCLUDED
#define MAPEDITOR_HPP_INCLUDED

#define N_BIOME_NBR  4
#define N_BIOME_LIST {"ducks", "fonts", "menu", "sounds"}

#include <enum.hpp>
#include <Coord.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>

#include <dirent.h>
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(WIN) || defined(_WIN)
	#include <windows.h>
	#define OS_WINDOWS
#else
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif


/**
 * @file MapEditor.hpp
 * @author Lucas LAZARE
 * @license Mozilla Public License, v. 2.0
 * @brief Define the MapEditor class
 */

/**
 * @class MapEditor MapEditor.hpp
 * @brief Define the Class MapEditor
 */


class MapEditor {

public :

    /**
     * @brief allocs memory for ground_textures and initializes biome_names
     */
    MapEditor();

    /**
     * @brief desalloc memory used by grounds_textures
     */
    ~MapEditor();

    /**
     * @ Starts the editor
     * @param window A window
     */
    void start(sf::RenderWindow& window);


private :

    /**
     * @brief loads sprites
     * @return true on success, false otherwise
     */
    bool loadSprites();

    /**
     * @brief finds textures folder (every folder except those defined by N_BIOME_LIST)
     */
    void findTextures();

    /**
     * @brief poll events from the window
     * @param window A window
     * @param event  Event that will be modified to store the new event, if any
     * @return true if there was an event, false otherwise
     */
    bool pollEvent(sf::RenderWindow& window, sf::Event& event);

    /**
     * @brief refreshes the screen
     * @param window A window
     */
    void refreshScreen(sf::RenderWindow& window);

    /**
     * @brief saves the map
     */
    void saveMap();




    sf::Texture ducks_textures[5][4][2];
    std::vector<sf::Texture**> ground_textures;

    sf::Sprite ducks_sprites[5][4];
    std::vector<sf::Sprite**> ground_sprites;

    std::vector<std::string> biome_names;

    Coord mouse_position;

    unsigned short selected_biome;
    unsigned short speed;
    unsigned char egg_nbr;
    unsigned char player_nbr;

    Direction ducks_starting_dir[5];
    Coord ducks_spawn[5];

    Coord map_size;
    bool is_visible;
    unsigned short los;
    std::vector<std::string> map;
    std::vector< std::vector<Area> > areas_map;

};

#endif /* MAPEDITOR_HPP_INCLUDED */
