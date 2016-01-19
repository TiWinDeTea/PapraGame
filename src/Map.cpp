/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Map>
#include <enum>
#include <Coord>
#include <SFML/Graphics>

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


void printAll (){
    
    Coord tile;
    
    for (tile.y = 0; tile.y < y_size; ++tile.y){	
	for (tile.x = 0; tile.x < x_size; ++tile.x){	    
	    this.print(tile);
	}
    }
}

	    
	    