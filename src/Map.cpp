/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Map>
#include <enum>
#include <Coord>
#include <SFML/Graphics>

#define RESOLUTION_X_IMAGE 32
#define RESOLUTION_Y_IMAGE 32

/*Map::Map : function to create with parametre the map for the game.
 * All the parametre must be different of undefined.
 * x : the lenght of the map.
 * y : the width of the map.
 * texture : array of texture wich contain all the texture needed to print the map.
 * egg_sprite : texture of the egg.
 * return the map created.
 */

Map::Map(unsigned char x, unsigned char y, Area smap[x][y], sf::Texture texture[NB_TEXTURE], sf::Texture egg_texture){
    
    x_size = x;
    y_size = y;
    
    map = new Area[x][y];
    
    for (unsigned int i = 0; i < x; i++){	
	for (unsigned int j = 0; j < y; j ++){	    
	    map[i][j] = smap[i][j];
	}
    }
    
    for (unsigned int i = 0; i < NB_TEXTURE; i++){	
	sprites[i].setTexture(texture[i]); /* Pray for working */
    }
    
    egg_sprite = egg_texture;
}

/*printAll : procedure to print all the map.
 */

void Map::printAll (){
    
    Coord tile;
    
    for (tile.y = 0; tile.y < y_size; ++tile.y){	
	for (tile.x = 0; tile.x < x_size; ++tile.x){	    
	    this.print(tile);
	}
    }
}

/*print : print a tile of the map.
 * tile : coordinate of the tile we want to print.
 */

void Map::print (Coord tile){
    
    map[tile.x][tile.y].setPosition(tile.x*RESOLUTION_X_IMAGE, tile.y*RESOLUTION_Y_IMAGE);
    windows.draw(map[tile.x][tile.y]);
    
}

/*popEgg : change the coordinate of the egg and print it.
 * egg_coord : new coordinate of the egg.
 */

void Map::popEgg (Coord egg_coord){
    
    coordinate_egg = egg_coord;
    
    egg_sprite.setPosition(coordinate_egg.x*RESOLUTION_X_IMAGE, coordinate_egg.y*RESOLUTION_Y_IMAGE);
    windows.draw(egg_sprite);
    
}


	    