/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <MapEditor.hpp>

MapEditor::MapEditor(){}

MapEditor::~MapEditor(){}

void MapEditor::start(sf::RenderWindow& window){}

bool MapEditor::loadSprites(){
    return true;
}

void MapEditor::findTextures(){

    DIR* directory = opendir("res/");
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
            for (unsigned short i(N_BIOME_NBR) ; i-- ;)
                if(local_path == n_biome_list[i])
                    return true;
            return false;
        };

		while ((redfile = readdir(directory)) != nullptr){
			std::string tmp( redfile->d_name );
			if (isFolder("res/" + tmp) && !notATexture(tmp)) {
				biome_names.push_back(tmp);
			}
		}
	}
}

bool MapEditor::pollEvent(sf::RenderWindow& window, sf::Event& event){

    if(window.pollEvent(event)){
        if(event.type == sf::Event::MouseMoved){
            sf::Vector2i mouse_current_pos = sf::Mouse::getPosition(window);
            if( (unsigned)mouse_current_pos.x/32 == mouse_position.x && (unsigned)mouse_current_pos.y/32 == mouse_position.y)
                return false;
            else{
                mouse_position.x = mouse_current_pos.x / 32;
                mouse_position.y = mouse_current_pos.y / 32;
                return true;
            }
        }
        else
            return true;
    }
    else
        return false;
}

void MapEditor::refreshScreen(sf::RenderWindow& window){

	for(unsigned int x(map_size.x) ; x-- ;){
		for (unsigned int y(map_size.y) ; y-- ;){
            ground_sprites[selected_biome][areas_map[x][y]]->setPosition(static_cast<float>(32*x), static_cast<float>(32*y));
            window.draw(*ground_sprites[selected_biome][areas_map[x][y]]);
		}
	}

    for(unsigned char i(player_nbr) ; i-- ;){
        ducks_sprites[i][ducks_starting_dir[i]].setPosition(static_cast<float>(32*ducks_spawn[i].x), static_cast<float>(32*ducks_spawn[i].y));
        window.draw(ducks_sprites[i][ducks_starting_dir[i]]);
    }
}

void MapEditor::saveMap(){

    auto fileExist = [] (std::string& filename) -> bool {
    	std::ifstream file (filename.c_str(), std::ios::in);
	    return !file.fail();
    };
    // Do you really want to override ? blabla
}
