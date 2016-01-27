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

Map::Map(const unsigned int x, const unsigned int y, std::vector< std::vector<Area> > smap, sf::Texture texture[NB_TEXTURE], sf::Texture* egg_texture, sf::Texture* explosion_texture) : x_size(x), y_size(y)
{
	srand(static_cast<unsigned int>(time(NULL)));
	for (unsigned int i = 0; i < x; i++){
		map.push_back(std::vector<Area>());
		for (unsigned int j = 0; j < y; j++){
			map[i].push_back(smap[i][j]);
		}
	}

	for (unsigned int i = 0; i < NB_TEXTURE; i++){
		sprites[i].setTexture(texture[i]);
	}
	egg_sprite.setTexture(*egg_texture);
	explosion_sprite.setTexture(*explosion_texture);
}

/*printAll : procedure to print all the map.
 */
void Map::print (sf::RenderWindow& window){

	Coord tile;

	for (tile.x = 0; tile.x < x_size; ++tile.x){
		for (tile.y = 0; tile.y < y_size; ++tile.y){
			this->print(tile, window);
		}
	}
	egg_sprite.setPosition(static_cast<float>(coordinate_egg.x * RESOLUTION_X_IMAGE),static_cast<float>( coordinate_egg.y * RESOLUTION_Y_IMAGE));
	window.draw(egg_sprite);
}

/*print : print a tile of the map.
 * tile : coordinate of the tile we want to print.
 */

void Map::print (Coord tile, sf::RenderWindow& window){

	sprites[map[tile.x][tile.y]].setPosition(static_cast<float>(tile.x*RESOLUTION_X_IMAGE), static_cast<float>(tile.y*RESOLUTION_Y_IMAGE));
	window.draw(sprites[map[tile.x][tile.y]]);

}

void Map::init(){

	for (unsigned int i = x_size ; i-- ;){
		for (unsigned int j = y_size ; j-- ;){
			if (map[i][j] != OBSTACLE){
				free_tile.push_back(Coord(i, j));
				if (map[i][j] == WARP){
					warp.push_back(Coord(i, j));
				}
			}
		}
	}
	if (free_tile.size() == 0)
		std::cout << "Bad map : Full of obstacles" << std::endl;
	else if (warp.size() == 1)
		std::cout << "Bad map : you should have no warp at all or at least 2" << std::endl;
}

void Map::popEgg (sf::RenderWindow& window){

	if (free_tile.size() != 0)
		coordinate_egg = free_tile[rand()%free_tile.size()];
	else{
		coordinate_egg = Coord(0,0);
	}
	egg_sprite.setPosition(static_cast<float>(coordinate_egg.x * RESOLUTION_X_IMAGE),static_cast<float>( coordinate_egg.y * RESOLUTION_Y_IMAGE));
	window.draw(egg_sprite);

}

bool Map::isWarp(Coord tile){

	for (unsigned int i = static_cast<unsigned int>(warp.size()); i-- ;)
		if (warp[i] == tile)
			return true;
	return false;
}

Coord Map::getWarp(Coord current_tile){
	if (warp.size() <= 1) {
		return current_tile;
	}

	std::vector<Coord> possible_warp;
	for (unsigned int i = static_cast<unsigned int>(warp.size()) ; i-- ;)
		if (warp[i] != current_tile)
			possible_warp.push_back(warp[i]);
	return possible_warp[rand()%possible_warp.size()];
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

void Map::printExplosion(sf::RenderWindow& window, Coord coord){

	explosion_sprite.setPosition(static_cast<float>(10 * 32), static_cast<float>(10 * 32));
	window.draw(explosion_sprite);
}