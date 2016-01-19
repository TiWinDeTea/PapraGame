/*******************************************************************************************
 * This file is created to implement the class Duck for our game called PapraGame.         *
 * This file will allow us to have all the method to control the duck for the game.        *
 * This file are under licence Mozilla Public License Version 2.0                          *
 * Author : Barbier Julien, julien.barbier@utbm.fr                                         *	
 * Date of creation : 01/19/2016                                                           *
 * *****************************************************************************************/
 
 #ifndef MAP_HPP_INCLUDED
 #define MAP_HPP_INCLUDED
 
 #include <Coord>
 #include <SFML/Graphics>
 #include <enum>
 
 #define NB_TEXTURE 8
 

 class Map {
	 
	 public : 
		 
		/**
		 * @brief Destroy the class Map
		 */
		 
		~Map();		
	 
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
		
		Map(unsigned char x, unsigned char y, Area smap[x][y], const String& texture[NB_TEXTURE], const String& egg_texture); 
			
		/**
		 * @brief Change the coordinates of the egg and print him.
		 * @param egg_coord The coordinate of the egg to initialize coordinate_egg(Map) and 
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
		sf::Texture sprites[NB_TEXTURE];
		Coord coordinate_egg;
		sf::Texture egg_sprite;
		Area map [] [];
 };
 
 #endif /* MAPP_HPP_INCLUDED*/
 
 
