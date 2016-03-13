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
    selected_biome(0),
    speed(25),
    egg_nbr(10),
    player_nbr(1),
    map_size(10, 10),
    is_visible(true),
    los_is_looping(false),
    los(3),
    areas_map(std::vector< std::vector<Area> >()),
    mouse_position(),
    mouse_prev_position(),
    mouse_press(),
    mouse_release(),
    lButton_pressed(false)
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
void MapEditor::start(sf::RenderWindow& window, std::string const& original_file){

    std::vector<bool> textures_state(this->loadTextures());

    Area area_list[] = AREA_LIST;
    sf::Keyboard::Key key_list[] = KEY_LIST;
    sf::Keyboard::Key secondary_key_list[] = SECONDARY_KEY_LIST;

    if (original_file.size()){
        this->loadMap(original_file);
    }

    if (areas_map.size() == 0){
        ducks_starting_dir[0] = RIGHT;
        ducks_spawn[0].x = 1;
        ducks_spawn[0].y = 1;

        for (unsigned int i(map_size.x) ; i-- ;)
            areas_map.push_back(std::vector<Area>());

        for (unsigned int i(map_size.y) ; i-- ;)
            for (unsigned int j(map_size.x) ; j-- ;)
                areas_map[j].push_back(EMPTY_TILE);
    }

    bool keep_going(true);
    sf::Event event;

    do{

        window.clear();

        while (this->pollEvent(window, event)){
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved){
            }
            else if (event.type == sf::Event::MouseButtonPressed){
            
            }
            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){

                }
                else{
                    for (unsigned char i(SHORTCUTS_QTT) ; i-- ;)
                        if (key_list[i] == event.key.code || secondary_key_list[i] == event.key.code){
                            areas_map[mouse_position.x][mouse_position.y] = area_list[i];
                            break;
                        }
                }
            }
        }

        sf::sleep(REFRESH_RATE);
        this->refreshScreen(window);
        this->highlightTile(mouse_position, window);
        window.display();
    }while(keep_going && window.isOpen());

// TODO
// -> Map testing

}


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
std::vector<bool> MapEditor::loadTextures(){

    std::string ground_tile_name[] = GROUNDS_TILES_LIST;
    std::vector<bool> ret;

    for (unsigned char i(static_cast<unsigned char>(biome_names.size() + 1)) ; i-- ;)
        ret.push_back(true);

    for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;){
        for (unsigned char j (GROUNDS_TILES_NB) ; j-- ;){

            if (!ground_textures[i][j].loadFromFile(biome_names[i] + ground_tile_name[j])){
                ret[i] = false;
            }
            ground_sprites[i][j].setTexture(ground_textures[i][j]);
        }
        if (!ret[i]){
            std::cout << biome_names[i] << ": Failed to load biome's textures" << std::endl;
        }
    }


    std::string duck_sprite_name[] = DUCKS_SPRITES_LIST;

    for (unsigned char i(PLAYER_NBR_MAX) ; i-- ;){
        for (unsigned char j(4) ; j-- ;){

            if (!ducks_textures[i][j].loadFromFile(RESOURCES_FOLDER + duck_sprite_name[j] + std::to_string(i) + DUCKS_FORMAT)){
                ret[i] = false;
            }
            ducks_sprites[i][j].setTexture(ducks_textures[i][j]);
        }
        if (!ret[i]){
            std::cout << "Player " << std::to_string(i + 1) << ": Failed to load textures" << std::endl;
        }
    }

    return ret;
}

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::pollEvent(sf::RenderWindow& window, sf::Event& event){

    if(window.pollEvent(event)){
        if(event.type == sf::Event::MouseMoved){
            sf::Vector2i mouse_current_pos = sf::Mouse::getPosition(window);
            if ((unsigned)(mouse_current_pos.x / 32) == mouse_position.x && (unsigned)(mouse_current_pos.y / 32) == mouse_position.y)
                return false;
            else{
                mouse_prev_position = mouse_position;
                mouse_position.x = mouse_current_pos.x / 32;
                mouse_position.y = mouse_current_pos.y / 32;
                return true;
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left)
                lButton_pressed = true;
            mouse_press.x = event.mouseButton.x / 32;
            mouse_press.y = event.mouseButton.y / 32;
            return true;
        }
        else if (event.type == sf::Event::MouseButtonReleased){
            if (event.mouseButton.button == sf::Mouse::Left)
                lButton_pressed = false;
            mouse_release.x = event.mouseButton.x / 32;
            mouse_release.y = event.mouseButton.y / 32;
            return true;
        }
        else if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Key::LShift){
                event.type = sf::Event::MouseButtonPressed;
                event.mouseButton.button = sf::Mouse::Left;
                lButton_pressed = true;
            }
            else if (event.key.code == sf::Keyboard::Key::Up){
                if (mouse_position.y > 0){
                    event.type = sf::Event::MouseMoved;
                    mouse_prev_position = mouse_position;
                    --mouse_position.y;
                }
            }
            else if (event.key.code == sf::Keyboard::Key::Down){
                if (mouse_position.y < map_size.y - 1){
                    event.type = sf::Event::MouseMoved;
                    mouse_prev_position = mouse_position;
                    ++mouse_position.y;
                }
            }
            else if (event.key.code == sf::Keyboard::Key::Left){
                if (mouse_position.x > 0){
                    event.type = sf::Event::MouseMoved;
                    mouse_prev_position = mouse_position;
                    --mouse_position.x;
                }
            }
            else if (event.key.code == sf::Keyboard::Key::Right){
                if (mouse_position.x < map_size.x - 1){
                    event.type = sf::Event::MouseMoved;
                    mouse_prev_position = mouse_position;
                    ++mouse_position.y;
                }
            }
            return true;
        }
        else if (event.type == sf::Event::KeyReleased){
            if (event.key.code == sf::Keyboard::Key::LShift){
                event.type = sf::Event::MouseButtonReleased;
                event.mouseButton.button = sf::Mouse::Left;
                lButton_pressed = false;
            }
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

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::loadMap(std::string const& map_path){

	std::ifstream map_file(map_path, std::ios::in | std::ios::binary);
	unsigned int x_map_size, y_map_size;
    short biome;
	std::string value;

    bool ret(true);

	if (map_file) {

		map_file >> value;
        biome = match(biome_names, value);
        if (biome < 0){
            std::cout << "Bad map: Unknown biome " << value << std::endl;
            selected_biome = 0;
            ret = false;
        }
        else
            selected_biome = (unsigned)biome;


		map_file >> value;
		if (value ==  "blind" || value == "true" || value == "1"){
			is_visible = false;
			map_file >> los;
			map_file >> value;
			if (value == "loop" || value == "true" || value == "1")
				los_is_looping = true;
			else
				los_is_looping = false;
		}
        else
            is_visible = true;

		map_file >> speed;
		map_file >> egg_nbr;
		map_file >> x_map_size;
		map_file >> y_map_size;

        map_size.x = x_map_size;
        map_size.y = y_map_size;

		for(unsigned int i = 0; i < x_map_size; ++i)
            areas_map.push_back(std::vector<Area>());

		for (unsigned int i = 0 ; i < y_map_size ; ++i) {
			map_file >> value;
			for (unsigned int j = 0 ; j < x_map_size ; ++j) {
				switch(value[j])
				{
					case IDENTIFIER_EMPTY_TILE:
						areas_map[j].push_back(EMPTY_TILE);
						break;
					case IDENTIFIER_OBSTACLE:
						areas_map[j].push_back(OBSTACLE);
						break;
					case IDENTIFIER_WATER_LEFT_DOWN:
						areas_map[j].push_back(WATER_DL);
						break;
					case IDENTIFIER_WATER_LEFT_RIGHT:
						areas_map[j].push_back(WATER_LR);
						break;
					case IDENTIFIER_WATER_RIGHT_DOWN:
						areas_map[j].push_back(WATER_RD);
						break;
					case IDENTIFIER_WATER_UP_DOWN:
						areas_map[j].push_back(WATER_UD);
						break;
					case IDENTIFIER_WATER_UP_LEFT:
						areas_map[j].push_back(WATER_LU);
						break;
					case IDENTIFIER_WATER_UP_RIGHT:
						areas_map[j].push_back(WATER_UR);
						break;
					case IDENTIFIER_WARP:
						areas_map[j].push_back(WARP);
						break;
					default:
						std::cout << "Bad map (found character " << value[j] << ")" << std::endl;
                        areas_map[j].push_back(EMPTY_TILE);
                        ret = false;
				}
			}
		}

		map_file >> value;
        player_nbr = 0;
		do{

			map_file >> ducks_spawn[player_nbr].x;
			--ducks_spawn[player_nbr].x;
			map_file >> ducks_spawn[player_nbr].y;
			--ducks_spawn[player_nbr].y;

			if (value == "up")
				ducks_starting_dir[player_nbr] = UP;
			else if (value == "down")
				ducks_starting_dir[player_nbr] = DOWN;
			else if (value == "left")
				ducks_starting_dir[player_nbr] = LEFT;
			else if (value == "right")
				ducks_starting_dir[player_nbr] = RIGHT;
			else{
				std::cout << "Not a valid direction : " << value << std::endl;
				std::cout << "(On player " << player_nbr << ")" << std::endl;
                ducks_starting_dir[player_nbr] = RIGHT;
                ret = false;
			}
			++player_nbr;
			map_file >> value;
		}while (value != "eof");

		return ret;
	}
	else{
        std::cout << "Failed to open the map file" << std::endl;
		return false;
    }
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::highlightTile(Coord tile, sf::RenderWindow& window){

    sf::RectangleShape highlighter(sf::Vector2f(32,32));

    highlighter.setOutlineThickness(HIGHLIGHTER_THICKNESS);
    highlighter.setFillColor(HIGHLIGHTER_INNERCOLOR);
    highlighter.setOutlineColor(HIGHLIGHTER_OUTERCOLOR);
    highlighter.setPosition(sf::Vector2f(static_cast<float>(tile.x), static_cast<float>(tile.y)));

    window.draw(highlighter);
}

//////////////////////////////////////////////////////////////////////////////
short MapEditor::match(std::vector<std::string> biomes, std::string const& str){
    for (short i(static_cast<short>(biomes.size())) ; i-- ;){
        if (biomes[i] == str)
            return i;
    }
    return -1;
}
