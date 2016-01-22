/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Map.hpp>

/*Map::Map : function to create with parametre the map for the game.
 * All the parametre must be different of undefined.
 * x : the lenght of the map.
 * y : the width of the map.
 * texture : array of texture wich contain all the texture needed to print the map.
 * egg_sprite : texture of the egg.
 * return the map created.
 */

Map::Map(const unsigned int x, const unsigned int y, std::vector< std::vector<Area> > smap, sf::Texture texture[NB_TEXTURE], sf::Texture* egg_texture) : x_size(x), y_size(y)
{ 
    for (unsigned int i = 0; i < x; i++){
        map.push_back(std::vector<Area>());
    	for (unsigned int j = 0; j < y; j ++){
                map[i].push_back(smap[j][i]);
    	}
    }

    for (unsigned int i = 0; i < NB_TEXTURE; i++){
	sprites[i].setTexture(texture[i]);
    }
    egg_sprite.setTexture(*egg_texture);
}

/*printAll : procedure to print all the map.
 */
void Map::print (sf::RenderWindow& window){
    
    Coord tile;
    
	for (tile.y = 0; tile.y < y_size; ++tile.y){	
		for (tile.x = 0; tile.x < x_size; ++tile.x){	    
			this->print(tile, window);
		}
	}
	this->popEgg(coordinate_egg, window);
}

/*print : print a tile of the map.
 * tile : coordinate of the tile we want to print.
 */

void Map::print (Coord tile, sf::RenderWindow& window){

    sprites[map[tile.x][tile.y]].setPosition(static_cast<float>(tile.x*RESOLUTION_X_IMAGE), static_cast<float>(tile.y*RESOLUTION_Y_IMAGE));
    window.draw(sprites[map[tile.x][tile.y]]);

}

/*popEgg : change the coordinate of the egg and print it.
 * egg_coord : new coordinate of the egg.
 */

void Map::popEgg (Coord egg_coord, sf::RenderWindow& window){

    coordinate_egg = egg_coord;
    egg_sprite.setPosition(static_cast<float>(coordinate_egg.x * RESOLUTION_X_IMAGE),static_cast<float>( coordinate_egg.y * RESOLUTION_Y_IMAGE));
    window.draw(egg_sprite);

}


Map& Map::operator=(const Map& mymap){
	x_size = mymap.x_size;
	y_size = mymap.y_size;
	for (unsigned int i = NB_TEXTURE ; i-- ;) {
		sprites[i] = mymap.sprites[i];
	}
	egg_sprite = mymap.egg_sprite;
	map = mymap.map;
	return *this;
}
