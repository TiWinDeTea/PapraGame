/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef MAPEDITOR_HPP_INCLUDED
#define MAPEDITOR_HPP_INCLUDED

#define N_BIOME_LIST            {"ducks", "fonts", "menu", "sounds", "default"}
#define N_BIOME_NB              5
#define GROUNDS_TILES_LIST      {"Obstacle.png", "EmptyTile.png", "WaterUR.png", "WaterRD.png", "WaterDL.png", "WaterLU.png", "WaterUD.png", "WaterLR.png", "Warp.png"}
#define GROUNDS_TILES_NB        9
#define DUCKS_SPRITES_LIST      {"DuckU_", "DuckD_", "DuckL_", "DuckR_"}
#define DUCKS_FORMAT            ".png"
#define RESOURCES_FOLDER        "res/"
#define PLAYER_NBR_MAX          5
#define HIGHLIGHTER_INNERCOLOR  sf::Color(255,255,255,150)
#define HIGHLIGHTER_OUTERCOLOR1 sf::Color(175,175,175,255)
#define HIGHLIGHTER_OUTERCOLOR2 sf::Color(255,125,125,255)
#define HIGHLIGHTER_THICKNESS   2

#define AREA_LIST               {OBSTACLE, EMPTY_TILE, WATER_UR, WATER_RD, WATER_DL, WATER_LU, WATER_UD, WATER_LR, WARP}
#define KEY_LIST                {sf::Keyboard::Key::O, sf::Keyboard::BackSpace, sf::Keyboard::Key::Num9, sf::Keyboard::Key::Num3, \
                                sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num7, sf::Keyboard::Key::Add, sf::Keyboard::Key::Subtract, sf::Keyboard::Key::W}
#define SECONDARY_KEY_LIST      {sf::Keyboard::Key::O, sf::Keyboard::E, sf::Keyboard::Key::Numpad9, sf::Keyboard::Key::Numpad3, \
                            sf::Keyboard::Key::Numpad1, sf::Keyboard::Key::Numpad7, sf::Keyboard::Key::R, sf::Keyboard::Key::Subtract, sf::Keyboard::Key::X}
#define AREAS_QTT               9

#define CONTINUOUS_TOGGLE       sf::Keyboard::Key::C
#define DUCK_TOGGLE             sf::Keyboard::Key::D

#define REFRESH_RATE_MS         50

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

enum Mode : unsigned char {
    Normal,
    Continuous,
    Duck
};

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
     */
    void loadTextures();

    /**
     * @brief poll events from the window and pre-treats them
     * @param window A window
     * @param event  Event that will be modified to store the new event, if any
     * @param ignore This parameter is set to true if the event should be ignored
     * @return true if there was an event, false otherwise
     */
    bool pollEvent(sf::RenderWindow& window, sf::Event& event, bool& ignore);

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

    /**
     * @brief loads the map
     * @param map_path Path to the map
     * @return         True on successful loading, false otherwise
     */
    bool loadMap(std::string const& map_path);

    /**
     * @brief highlights a tile and prints an area
     * @param tile   Coordinates of the tile to highlight
     * @param area   Index of the area to print in the sprite vector
     * @param color  Color of the outer line
     * @param window A window
     */
    void highlightTile(Coord tile, unsigned char area, sf::RenderWindow& window, sf::Color color);

    /**
     * @brief returns the subscript where str can be find
     * @param biomes Array to scan
     * @param str    String to find
     * @return       The subscript where str is, or -1 if not found
     */
   short match(std::vector<std::string> biomes, std::string const& str);

   /**
     * @brief returns the subscript where expr can be find
     * @param area_nbr Lenght of the array
     * @param area     Array to scan
     * @param expr     Area to find
     * @return         The subscript where str is, or -1 if not found
     */
   unsigned char match(unsigned char area_nbr, Area area[], Area expr);

   /**
    * @brief Links rivers together
    * @param departure_area Area where the cursor was just before
    * @param arrival_area   Area where the cursor is
    */
   void riverLinkIt(Area& departure_area, Area& arrival_area);




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
    std::vector< std::vector<Area> > areas_map;

    Coord mouse_position;
    Direction mouse_relative_movmnt;
    Direction mouse_relative_movmnt_2;
    Coord mouse_prev_position;
    Direction mouse_prev_relative_movmnt;
    Direction mouse_prev_relative_movmnt_2;
    Coord mouse_press;
    Coord mouse_release;
    bool lButton_pressed;
};

#endif /* MAPEDITOR_HPP_INCLUDED */
