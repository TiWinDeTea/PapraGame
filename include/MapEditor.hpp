/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef MAPEDITOR_HPP_INCLUDED
#define MAPEDITOR_HPP_INCLUDED

#define N_BIOME_LIST         {"ducks", "fonts", "menu", "sounds"}
#define N_BIOME_NB           4
#define GROUNDS_TILES_LIST   {"Obstacle.png", "EmptyTile.png", "WaterUR.png", "WaterRD.png", "WaterDL.png", "WaterLU.png", "WaterUD.png", "WaterLR.png", "Warp.png"}
#define GROUNDS_TILES_NB     9
#define DUCKS_SPRITES_LIST   {"DuckU_", "DuckD_", "DuckL_", "DuckR_"}
#define DUCKS_FORMAT         ".png"
#define RESOURCES_FOLDER     "res/"
#define PLAYER_NBR_MAX       5

#ifndef PAPRAGAME_PATHS_DEFINED
#define PAPRAGAME_PATHS_DEFINED
	#define IDENTIFIER_WATER_UP_DOWN    '|'
	#define IDENTIFIER_WATER_UP_RIGHT   '9'
	#define IDENTIFIER_WATER_UP_LEFT    '7'
	#define IDENTIFIER_WATER_LEFT_RIGHT '-'
	#define IDENTIFIER_WATER_LEFT_DOWN  '1'
	#define IDENTIFIER_WATER_RIGHT_DOWN '3'
	#define IDENTIFIER_EMPTY_TILE       '.'
	#define IDENTIFIER_WARP             'x'
	#define IDENTIFIER_OBSTACLE         'o'
#endif /* PAPRAGAME_PATHS_DEFINED */

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
     * @brief allocs memory for ground_textures, ground_sprites and initializes biome_names
     */
    MapEditor();

    /**
     * @brief desalloc memory used by grounds_textures
     */
    ~MapEditor();

    /**
     * @brief Starts the editor
     * @param window A window
     * @param original_file Edit this map instead of creating a brand new map (empty to start a new map)
     */
    void start(sf::RenderWindow& window, std::string const& original_file);


private :

    /**
     * @brief finds textures folder (every folder except those defined by N_BIOME_LIST)
     */
    void findTextures();

   /**
     * @brief loads textures
     * @return true on success, false otherwise
     */
    bool loadTextures();

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
     * @param original_file The original map (empty for no original map)
     */
    void saveMap(std::string const& original_file);




    sf::Texture ducks_textures[PLAYER_NBR_MAX][4];
    std::vector<sf::Texture*> ground_textures;

    sf::Sprite ducks_sprites[PLAYER_NBR_MAX][4];
    std::vector<sf::Sprite*> ground_sprites;

    std::vector<std::string> biome_names;

    unsigned short selected_biome;
    unsigned short speed;
    unsigned char egg_nbr;
    unsigned char player_nbr;

    Direction ducks_starting_dir[PLAYER_NBR_MAX];
    Coord ducks_spawn[PLAYER_NBR_MAX];

    Coord map_size;
    bool is_visible;
    bool los_is_looping;
    unsigned short los;
    std::vector<std::string> map;
    std::vector< std::vector<Area> > areas_map;

    Coord mouse_position;
    Coord mouse_press;
    Coord mouse_release;
};

#endif /* MAPEDITOR_HPP_INCLUDED */
