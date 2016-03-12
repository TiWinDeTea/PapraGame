/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <MapEditor.hpp>

MapEditor::MapEditor():
    ground_textures(std::vector<sf::Texture*>()),
    ground_sprites(std::vector<sf::Sprite*>()),
    biome_names(std::vector<std::string>()),
    selected_biome(),
    speed(),
    egg_nbr(),
    player_nbr(),
    map_size(),
    is_visible(),
    los_is_looping(),
    los(),
    map(std::vector<std::string>()),
    areas_map(std::vector< std::vector<Area> >()),
    mouse_position(),
    mouse_press(),
    mouse_release()
{
    this->findTextures();
    for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ; ) {
        ground_textures.push_back(new sf::Texture[GROUNDS_TILES_NB]);
        ground_sprites.push_back(new sf::Sprite[GROUNDS_TILES_NB]);
    }
}

//////////////////////////////////////////////////////////////////////////////
MapEditor::~MapEditor(){
    for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;){
        delete ground_textures[i];
        delete ground_sprites[i];
    }
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::start(sf::RenderWindow& window, std::string const& original_file){}
// static bool 	sf::Mouse::isButtonPressed (Button button)
// TODO
// -> Map testing


//////////////////////////////////////////////////////////////////////////////
void MapEditor::findTextures(){

    DIR* directory = opendir(RESOURCES_FOLDER);
	struct dirent* redfile = nullptr;

	if( directory != nullptr ){

        std::string n_biome_list[] = N_BIOME_LIST;

		auto isFolder = [](std::string const& local_path) -> bool {
			#ifdef OS_WINDOWS
				std::ifstream file (local_path.c_str(), std::ios::in);
				return file.fail();
			#else
				struct stat s;
				if(stat(local_path.c_str(), &s) == 0)
					return s.st_mode & S_IFDIR;
				else
					return false;
			#endif
		};

        auto notATexture = [&n_biome_list](std::string const& local_path) -> bool {
            for (unsigned short i(N_BIOME_NB) ; i-- ;)
                if(local_path == n_biome_list[i])
                    return true;
            return false;
        };

		while ((redfile = readdir(directory)) != nullptr){
			std::string tmp( redfile->d_name );
			if (isFolder(RESOURCES_FOLDER + tmp) && !notATexture(tmp)) {
				biome_names.push_back(tmp);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::loadTextures(){

    std::string ground_tile_name[] = GROUNDS_TILES_LIST;

    for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;){
        for (unsigned char j (GROUNDS_TILES_NB) ; j-- ;){

            if (!ground_textures[i][j].loadFromFile(biome_names[i] + ground_tile_name[j]))
                return false;
            ground_sprites[i][j].setTexture(ground_textures[i][j]);
        }
    }


    std::string duck_sprite_name[] = DUCKS_SPRITES_LIST;

    for (unsigned char i(PLAYER_NBR_MAX) ; i-- ;){
        for (unsigned char j(4) ; j-- ;){

            if (!ducks_textures[i][j].loadFromFile(RESOURCES_FOLDER + duck_sprite_name[j] + std::to_string(i) + DUCKS_FORMAT))
                return false;
            ducks_sprites[i][j].setTexture(ducks_textures[i][j]);
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::pollEvent(sf::RenderWindow& window, sf::Event& event){

    if(window.pollEvent(event)){
        if(event.type == sf::Event::MouseMoved){
            sf::Vector2i mouse_current_pos = sf::Mouse::getPosition(window);
            if ((unsigned)(mouse_current_pos.x / 32) == mouse_position.x && (unsigned)(mouse_current_pos.y / 32) == mouse_position.y)
                return false;
            else{
                mouse_position.x = mouse_current_pos.x / 32;
                mouse_position.y = mouse_current_pos.y / 32;
                return true;
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed){
            mouse_press.x = event.mouseButton.x / 32;
            mouse_press.y = event.mouseButton.y / 32;
            return true;
        }
        else if (event.type == sf::Event::MouseButtonReleased){
            mouse_release.x = event.mouseButton.x / 32;
            mouse_release.y = event.mouseButton.y / 32;
            return true;
        }
        else
            return true;
    }
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::refreshScreen(sf::RenderWindow& window){

	for(unsigned int x(map_size.x) ; x-- ;){
		for (unsigned int y(map_size.y) ; y-- ;){
            ground_sprites[selected_biome][areas_map[x][y]].setPosition(static_cast<float>(32*x), static_cast<float>(32*y));
            window.draw(ground_sprites[selected_biome][areas_map[x][y]]);
		}
	}

    for(unsigned char i(player_nbr) ; i-- ;){
        ducks_sprites[i][ducks_starting_dir[i]].setPosition(static_cast<float>(32*ducks_spawn[i].x), static_cast<float>(32*ducks_spawn[i].y));
        window.draw(ducks_sprites[i][ducks_starting_dir[i]]);
    }
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::saveMap(std::string const& original_file){
// TODO : GUI

    std::string filename;

    auto selectName = [] () -> std::string {

        auto fileExist = [] (std::string& file) -> bool {
            std::ifstream fs (file, std::ios::in);
            return !fs.fail();
        };

        bool done(false);
        std::string file, ans;
        do{
            std::cout << "Save as: ";
            std::cin >> file;
            file += ".map";
            if (fileExist(file)){
                std::cout << "Overwrite " << file.substr(0, file.size() - 4) << " ? [Y/n]";
                std::cin >> ans;
                if (ans != "n" && ans != "N"){
                    done = true;
                }
            }
        }while(!done);

        return file;
    };

    if (original_file.size()){
        std::cout << "Overwrite " << original_file.substr(0, original_file.size() - 4) << " ? [Y/n]";
        std::cin >> filename;
        if (filename == "n" || filename == "N"){
            filename = selectName();
        }
        else {
            filename = original_file;
        }
    }
    else{
        filename = selectName();
    }

    std::ofstream save (filename, std::ios::out | std::ios::trunc);
    save << biome_names[selected_biome] << std::endl;
    if (is_visible){
        save << "visible" << std::endl;
    }
    else{
        save << "blind " << los;
        if (los_is_looping)
            save << " loop" << std::endl;
        else
            save << " no-loop" << std::endl;
    }

    save << speed << std::endl;
    save << (short)(egg_nbr) << std::endl;
    save << map_size.x << " " << map_size.y << std::endl;

    for (unsigned int i(0) ; i < map_size.y ; --i){
        for (unsigned int j(0) ; j < map_size.x ; j--){
            switch(areas_map[j][i]){
                case OBSTACLE:
                    save << IDENTIFIER_OBSTACLE;
                    break;
                case EMPTY_TILE:
                    save << IDENTIFIER_EMPTY_TILE;
                    break;
                case WATER_UR:
                    save << IDENTIFIER_WATER_UP_RIGHT;
                    break;
                case WATER_RD:
                    save << IDENTIFIER_WATER_RIGHT_DOWN;
                    break;
                case WATER_DL:
                    save << IDENTIFIER_WATER_LEFT_DOWN;
                    break;
                case WATER_LU:
                    save << IDENTIFIER_WATER_UP_LEFT;
                    break;
                case WATER_UD:
                    save << IDENTIFIER_WATER_UP_DOWN;
                    break;
                case WATER_LR:
                    save << IDENTIFIER_WATER_LEFT_RIGHT;
                    break;
                case WARP:
                    save << IDENTIFIER_WARP;
                    break;
                default:
                    std::cout << "Unexpected error when saving the map (found value " << (short)areas_map[j][i] << " in the map)" << std::endl;
                    std::cout << "The save may have failed..." << std::endl;
                    break;
            }
        }
        save << std::endl;
    }

    for (unsigned int i(0) ; i < player_nbr ; --i){
        switch(ducks_starting_dir[i]){
            case UP:
                save << "up ";
                break;
            case DOWN:
                save << "down ";
                break;
            case LEFT:
                save << "left ";
                break;
            case RIGHT:
                save << "right ";
                break;
            case NOPE:
            default:
                std::cout << "Unexpected error when saving the map (found value " << (short)ducks_starting_dir[i] << " in player's " << (short)(i + 1) << " direction" << std::endl;
                std::cout << "The save may have failed..." << std::endl;
                break;
        }
        save << ducks_spawn[i].x << " " << ducks_spawn[i].y << std::endl;
    }
    save << "eof" << std::endl;
}
