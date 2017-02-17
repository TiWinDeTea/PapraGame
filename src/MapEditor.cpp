/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <MapEditor.hpp>

#if !defined(WHEELSCROLLEVENT) && !defined(MOUSEWHEELSCROLLDELTA)
#ifdef OLD_SFML_COMPAT
#define WHEELSCROLLEVENT        sf::Event::MouseWheelMoved
#define MOUSEWHEELSCROLLDELTA   mouseWheel.delta
#else
#define WHEELSCROLLEVENT        sf::Event::MouseWheelScrolled
#define MOUSEWHEELSCROLLDELTA   mouseWheelScroll.delta
#endif
#endif


MapEditor::MapEditor( ) :
		ground_textures(std::vector<sf::Texture*>()), ground_sprites(std::vector<sf::Sprite*>()),
		biome_names(std::vector<std::string>()), selected_biome(0), speed(25), egg_nbr(10), player_nbr(1),
		map_size(30, 30), is_visible(true), los_is_looping(false), los(3), areas_map(
		std::vector<std::vector<Area>>()), mouse_position(0, 0), mouse_relative_movmnt(LEFT),
		mouse_relative_movmnt_2(NOPE), mouse_prev_position(0, 1), mouse_prev_relative_movmnt(LEFT),
		mouse_prev_relative_movmnt_2(NOPE), mouse_press(), mouse_release(), lButton_pressed(false) {
	this->findTextures();
	ground_textures.reserve(GROUNDS_TILES_NB);
	ground_sprites.reserve(GROUNDS_TILES_NB);
	for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;) {
		ground_textures.push_back(new sf::Texture[GROUNDS_TILES_NB]);
		ground_sprites.push_back(new sf::Sprite[GROUNDS_TILES_NB]);
	}
}

//////////////////////////////////////////////////////////////////////////////
MapEditor::~MapEditor( ) {
	for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;) {
		delete[] ground_textures[i];
		delete[] ground_sprites[i];
	}
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::start(sf::RenderWindow& window, std::string const& original_file) {

	this->loadTextures();

	Area area_list[] = AREA_LIST;
	sf::Keyboard::Key key_list[] = KEY_LIST;
	sf::Keyboard::Key secondary_key_list[] = SECONDARY_KEY_LIST;

	if (original_file.size()) {
		this->loadMap(original_file);
	}

	if (areas_map.size() == 0) {
		ducks_starting_dir[0] = RIGHT;
		ducks_spawn[0].x = 1;
		ducks_spawn[0].y = 1;

		for (unsigned int i(map_size.x) ; i-- ;) {
			areas_map.push_back(std::vector<Area>());
		}

		for (unsigned int i(map_size.y) ; i-- ;) {
			for (unsigned int j(map_size.x) ; j-- ;) {
				areas_map[j].push_back(EMPTY_TILE);
			}
		}
	}

	window.setSize(sf::Vector2u(map_size.x * 32, map_size.y * 32));
	window.setView(
			sf::View(sf::FloatRect(0, 0, static_cast<float>(map_size.x * 32), static_cast<float>(map_size.y * 32))));

	bool keep_going(true);
	Mode mode(Mode::Normal);
	unsigned char selection(0);
	sf::Event event;

	auto isRiver = [](Area area) -> bool {
		switch (area) {
			case OBSTACLE:
			case EMPTY_TILE:
			case WARP:
				return false;
			case WATER_DL:
			case WATER_LR:
			case WATER_LU:
			case WATER_RD:
			case WATER_UD:
			case WATER_UR:
			default:
				return true;
		}
	};

	bool ignore;

	do {

		window.clear();

		while (this->pollEvent(window, event, ignore)) {
			if (!ignore) {

				if (event.type == sf::Event::Closed) {
					window.close();
					return;
				} else if (event.type == sf::Event::MouseMoved) {
					if (mode == Mode::Continuous || lButton_pressed) {
						if (isRiver(area_list[selection])) {

							riverLinkIt(areas_map[mouse_prev_position.x][mouse_prev_position.y],
							            areas_map[mouse_position.x][mouse_position.y]);
						} else {
							areas_map[mouse_position.x][mouse_position.y] = area_list[selection];
						}
					}

				} else if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						areas_map[mouse_position.x][mouse_position.y] = area_list[selection];
					} else if (event.mouseButton.button == sf::Mouse::Right) {
						selection = match(AREAS_QTT, area_list, areas_map[mouse_position.x][mouse_position.y]);
					}
					//TODO : crossed rivers
				} else if (event.type == sf::Event::KeyPressed) {

					if (event.key.code == sf::Keyboard::Escape) {
						return;
						//TODO : menu (biomes, test, help, ...)
					} else if (event.key.code == CONTINUOUS_TOGGLE) {
						if (mode == Mode::Continuous) {
							mode = Mode::Normal;
						} else {
							mode = Mode::Continuous;
							areas_map[mouse_position.x][mouse_position.y] = area_list[selection];
						}
					} else if (event.key.code == DUCK_TOGGLE) {
						if (mode == Mode::Duck) {
							mode = Mode::Normal;
						} else {
							std::cout << "Duck mode" << std::endl;
							mode = Mode::Duck;
							if (player_nbr < PLAYER_NBR_MAX) {
								// todo : Duck mode does not work (bad index ?)
								selection = static_cast<unsigned char>(player_nbr + 1);
							} else {
								selection = PLAYER_NBR_MAX;
							}
						}
					} else {
						for (unsigned char i(AREAS_QTT) ; i-- ;) {
							if (key_list[i] == event.key.code || secondary_key_list[i] == event.key.code) {
								selection = i;
								break;
							}
						}
						if (mode == Mode::Continuous) {
							areas_map[mouse_position.x][mouse_position.y] = area_list[selection];
						}
					}
				}
			}
		}




		//Display

		sf::sleep(sf::milliseconds(REFRESH_RATE_MS / 10));
		/*
		if (window.getSize().x%32 || window.getSize().y%32){

			if(window.getSize().x > map_size.x * 32)
				map_size.x = window.getSize().x / 32 + 1;
			else{
				map_size.x = window.getSize().x / 32;
				if (!map_size.x)
					map_size.x = 1;
			}
			if(window.getSize().y > map_size.y * 32)
				map_size.y = window.getSize().y / 32 + 1;
			else{
				map_size.y = window.getSize().y / 32;
				if(!map_size.y)
					map_size.y = 1;
			}
			window.setSize(sf::Vector2u(map_size.x * 32, map_size.y * 32));
			window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(map_size.x * 32), static_cast<float>(map_size.y * 32))));
		}
		// TODO : resize areas_map
		*/
		this->refreshScreen(window);
		this->highlightTile(mouse_position, mode == Mode::Continuous
		                                    ? (unsigned char) 255
		                                    : selection, window, mode == Mode::Continuous
		                                                         ? HIGHLIGHTER_OUTERCOLOR2
		                                                         : HIGHLIGHTER_OUTERCOLOR1
		);
		window.display();


	} while (keep_going && window.isOpen());

// TODO
// -> Map testing

}


//////////////////////////////////////////////////////////////////////////////
void MapEditor::findTextures( ) {

	DIR* directory = opendir(RESOURCES_FOLDER);
	struct dirent* redfile = nullptr;

	if (directory != nullptr) {

		std::string n_biome_list[] = N_BIOME_LIST;

		auto isFolder = [](std::string const& local_path) -> bool {
#ifdef OS_WINDOWS
			std::ifstream file (local_path.c_str(), std::ios::in);
			return file.fail();
#else
			struct stat s;
			if (stat(local_path.c_str(), &s) == 0) {
				return (bool) (s.st_mode & S_IFDIR);
			} else {
				return false;
			}
#endif
		};

		auto notATexture = [&n_biome_list](std::string const& local_path) -> bool {
			for (unsigned short i(N_BIOME_NB) ; i-- ;) {
				if (local_path == n_biome_list[i]) {
					return true;
				}
			}
			return false;
		};

		while ((redfile = readdir(directory)) != nullptr) {
			std::string tmp(redfile->d_name);
			if (isFolder(RESOURCES_FOLDER + tmp) && !notATexture(tmp) && tmp[0] != '.') {
				biome_names.push_back(tmp);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::loadTextures( ) {

	std::string ground_tile_name[] = GROUNDS_TILES_LIST;
	std::vector<bool> ret;

	for (unsigned char i(static_cast<unsigned char>(biome_names.size() + 1)) ; i-- ;) {
		ret.push_back(true);
	}

	for (unsigned char i(static_cast<unsigned char>(biome_names.size())) ; i-- ;) {
		for (unsigned char j(GROUNDS_TILES_NB) ; j-- ;) {

			if (!ground_textures[i][j].loadFromFile(RESOURCES_FOLDER + biome_names[i] + "/" + ground_tile_name[j])) {
				ret[i] = false;
			}
			ground_sprites[i][j].setTexture(ground_textures[i][j]);
		}
		if (!ret[i]) {
			std::cout << RESOURCES_FOLDER + biome_names[i] + "/" << ": Failed to load biome's textures" << std::endl;
		}
	}


	std::string duck_sprite_name[] = DUCKS_SPRITES_LIST;

	for (unsigned char i(PLAYER_NBR_MAX) ; i-- ;) {
		for (unsigned char j(4) ; j-- ;) {

			if (!ducks_textures[i][j].loadFromFile(
					std::string(RESOURCES_FOLDER) + "ducks/" + duck_sprite_name[j] + std::to_string(i) +
					DUCKS_FORMAT)) {
				ret[i] = false;
			}
			ducks_sprites[i][j].setTexture(ducks_textures[i][j]);
		}
		if (!ret[i]) {
			std::cout << "Player " << std::to_string(i + 1) << ": Failed to load textures" << std::endl;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::pollEvent(sf::RenderWindow& window, sf::Event& event, bool& ignore) {

	ignore = false;
	if (window.pollEvent(event)) {
		if (event.type == sf::Event::MouseMoved) {
			sf::Vector2i mouse_current_pos = sf::Mouse::getPosition(window);
			if ((unsigned) (mouse_current_pos.x / 32) == mouse_position.x &&
			    (unsigned) (mouse_current_pos.y / 32) == mouse_position.y) {
				    ignore = true;
			} else {
				mouse_prev_position = mouse_position;
				mouse_prev_relative_movmnt = mouse_relative_movmnt;


				mouse_prev_relative_movmnt = mouse_relative_movmnt;
				mouse_prev_relative_movmnt_2 = mouse_relative_movmnt_2;

				mouse_position.x = static_cast<unsigned int>(mouse_current_pos.x / 32);
				mouse_position.y = static_cast<unsigned int>(mouse_current_pos.y / 32);

				if (mouse_position.x == mouse_prev_position.x) {
					if (mouse_position.y < mouse_prev_position.y) {
						mouse_relative_movmnt = UP;
						mouse_relative_movmnt_2 = NOPE;
					} else if (mouse_position.y > mouse_prev_position.y) {
						mouse_relative_movmnt = DOWN;
						mouse_relative_movmnt_2 = NOPE;
					} else {
						mouse_relative_movmnt = NOPE;
					}
				} else if (mouse_position.y == mouse_prev_position.y) {
					if (mouse_position.x < mouse_prev_position.x) {
						mouse_relative_movmnt = LEFT;
						mouse_relative_movmnt_2 = NOPE;
					} else if (mouse_position.x > mouse_prev_position.x) {
						mouse_relative_movmnt = RIGHT;
						mouse_relative_movmnt_2 = NOPE;
					} else {
						mouse_relative_movmnt = NOPE;
					}
				} else {
					if (mouse_position.y > mouse_prev_position.y) {
						if (mouse_position.x < mouse_prev_position.x) {
							mouse_relative_movmnt = DOWN;
							mouse_relative_movmnt_2 = LEFT;
						} else if (mouse_position.x > mouse_prev_position.x) {
							mouse_relative_movmnt = DOWN;
							mouse_relative_movmnt_2 = RIGHT;
						} else {
							mouse_relative_movmnt = NOPE;
						}
					} else if (mouse_position.y < mouse_prev_position.y) {
						if (mouse_position.x < mouse_prev_position.x) {
							mouse_relative_movmnt = UP;
							mouse_relative_movmnt_2 = LEFT;
						} else if (mouse_position.x > mouse_prev_position.x) {
							mouse_relative_movmnt = UP;
							mouse_relative_movmnt = RIGHT;
						} else {
							mouse_relative_movmnt = NOPE;
						}
					} else {
						mouse_relative_movmnt = NOPE;
					}
				}

				if (mouse_position.x > map_size.x - 1) {
					mouse_position.x = map_size.x - 1;
				}
				if (mouse_position.y > map_size.y - 1) {
					mouse_position.y = map_size.y - 1;
				}

			}
		} else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				lButton_pressed = true;
			}
			mouse_press.x = static_cast<unsigned int>(event.mouseButton.x / 32);
			mouse_press.y = static_cast<unsigned int>(event.mouseButton.y / 32);
		} else if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				lButton_pressed = false;
			}
			mouse_release.x = static_cast<unsigned int>(event.mouseButton.x / 32);
			mouse_release.y = static_cast<unsigned int>(event.mouseButton.y / 32);
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Key::Return) {
				event.type = sf::Event::MouseButtonPressed;
				event.mouseButton.button = sf::Mouse::Left;
				lButton_pressed = true;
			} else if (event.key.code == sf::Keyboard::Key::Up) {
				mouse_prev_relative_movmnt = mouse_relative_movmnt;
				mouse_prev_relative_movmnt_2 = mouse_relative_movmnt_2;
				mouse_relative_movmnt_2 = NOPE;
				mouse_relative_movmnt = UP;
				event.type = sf::Event::MouseMoved;
				mouse_prev_position = mouse_position;
				if (mouse_position.y > 0) {
					--mouse_position.y;
				} else {
					mouse_position.y = map_size.y - 1;
				}
			} else if (event.key.code == sf::Keyboard::Key::Down) {
				mouse_prev_relative_movmnt = mouse_relative_movmnt;
				mouse_prev_relative_movmnt_2 = mouse_relative_movmnt_2;
				mouse_relative_movmnt_2 = NOPE;
				mouse_relative_movmnt = DOWN;
				event.type = sf::Event::MouseMoved;
				mouse_prev_position = mouse_position;
				if (mouse_position.y < map_size.y - 1) {
					++mouse_position.y;
				} else {
					mouse_position.y = 0;
				}
			} else if (event.key.code == sf::Keyboard::Key::Left) {
				mouse_prev_relative_movmnt = mouse_relative_movmnt;
				mouse_prev_relative_movmnt_2 = mouse_relative_movmnt_2;
				mouse_relative_movmnt_2 = NOPE;
				mouse_relative_movmnt = LEFT;
				event.type = sf::Event::MouseMoved;
				mouse_prev_position = mouse_position;
				if (mouse_position.x > 0) {
					--mouse_position.x;
				} else {
					mouse_position.x = map_size.x - 1;
				}
			} else if (event.key.code == sf::Keyboard::Key::Right) {
				mouse_prev_relative_movmnt = mouse_relative_movmnt;
				mouse_prev_relative_movmnt_2 = mouse_relative_movmnt_2;
				mouse_relative_movmnt_2 = NOPE;
				mouse_relative_movmnt = RIGHT;
				event.type = sf::Event::MouseMoved;
				mouse_prev_position = mouse_position;
				if (mouse_position.x < map_size.x - 1) {
					++mouse_position.x;
				} else {
					mouse_position.x = 0;
				}
			} else if (event.key.code == sf::Keyboard::Key::Escape) {
				this->saveMap("maps/test.map");
			}
		} else if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::Key::Return) {
				event.type = sf::Event::MouseButtonReleased;
				event.mouseButton.button = sf::Mouse::Left;
				lButton_pressed = false;
			}
		}
		return true;
	} else {
		sf::sleep(sf::milliseconds(9 * REFRESH_RATE_MS / 10));
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::refreshScreen(sf::RenderWindow& window) {

	for (unsigned int x(map_size.x) ; x-- ;) {
		for (unsigned int y(map_size.y) ; y-- ;) {
			if (areas_map[x][y] != EMPTY_TILE) {
				ground_sprites[selected_biome][EMPTY_TILE].setPosition(static_cast<float>(32 * x),
				                                                       static_cast<float>(32 * y));
				window.draw(ground_sprites[selected_biome][EMPTY_TILE]);
			}
			ground_sprites[selected_biome][areas_map[x][y]].setPosition(static_cast<float>(32 * x),
			                                                            static_cast<float>(32 * y));
			window.draw(ground_sprites[selected_biome][areas_map[x][y]]);
		}
	}

	for (unsigned char i(player_nbr) ; i-- ;) {
		ducks_sprites[i][ducks_starting_dir[i]].setPosition(static_cast<float>(32 * ducks_spawn[i].x),
		                                                    static_cast<float>(32 * ducks_spawn[i].y));
		window.draw(ducks_sprites[i][ducks_starting_dir[i]]);
	}
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::saveMap(std::string const& original_file) {
// TODO : GUI

	std::string filename;

	auto selectName = []( ) -> std::string {

		auto fileExist = [](std::string& file) -> bool {
			std::ifstream fs(file, std::ios::in);
			return !fs.fail();
		};

		bool done(false);
		std::string file, ans;
		do {
			std::cout << "Save as: ";
			std::cin >> file;
			file += ".map";
			if (fileExist(file)) {
				std::cout << "Overwrite " << file.substr(0, file.size() - 4) << " ? [Y/n]";
				std::cin >> ans;
				if (ans != "n" && ans != "N") {
					done = true;
				}
			} else {
				done = true;
			}
		} while (!done);

		return file;
	};

	if (original_file.size()) {
		std::cout << "Overwrite " << original_file.substr(0, original_file.size() - 4) << " ? [Y/n]";
		std::cin >> filename;
		if (filename == "n" || filename == "N") {
			filename = selectName();
		} else {
			filename = original_file;
		}
	} else {
		filename = selectName();
	}

	std::ofstream save(filename, std::ios::out | std::ios::trunc);
	save << biome_names[selected_biome] << std::endl;
	if (is_visible) {
		save << "visible" << std::endl;
	} else {
		save << "blind " << los;
		if (los_is_looping) {
			save << " loop" << std::endl;
		} else {
			save << " no-loop" << std::endl;
		}
	}

	save << speed << std::endl;
	save << (short) (egg_nbr) << std::endl;
	save << map_size.x << " " << map_size.y << std::endl;

	for (unsigned int i(0) ; i < map_size.y ; ++i) {
		for (unsigned int j(0) ; j < map_size.x ; ++j) {
			switch (areas_map[j][i]) {
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
					std::cout << "Unexpected error when saving the map (found value " << (short) areas_map[j][i]
					          << " in the map)" << std::endl;
					std::cout << "The save may have failed..." << std::endl;
					break;
			}
		}
		save << std::endl;
	}

	for (unsigned int i(0) ; i < player_nbr ; --i) {
		switch (ducks_starting_dir[i]) {
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
				std::cout << "Unexpected error when saving the map (found value " << (short) ducks_starting_dir[i]
				          << " in player's "
				          << (short) (i + 1) << " direction" << std::endl;
				std::cout << "The save may have failed..." << std::endl;
				break;
		}
		save << ducks_spawn[i].x << " " << ducks_spawn[i].y << std::endl;
	}
	save << "eof" << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
bool MapEditor::loadMap(std::string const& map_path) {

	std::ifstream map_file("maps/" + map_path, std::ios::in | std::ios::binary);
	unsigned int x_map_size, y_map_size;
	short biome;
	std::string value;

	bool ret(true);

	if (map_file) {

		map_file >> value;
		value.append("/");
		value = RESOURCES_FOLDER + value;
		biome = match(biome_names, value);
		if (biome < 0) {
			std::cout << "Bad map: Unknown biome " << value << std::endl;
			selected_biome = 0;
			ret = false;
		} else {
			selected_biome = static_cast<unsigned short>(biome);
		}

		map_file >> value;
		if (value == "blind" || value == "true" || value == "1") {
			is_visible = false;
			map_file >> los;
			map_file >> value;
			los_is_looping = value == "loop" || value == "true" || value == "1";
		} else {
			is_visible = true;
		}

		map_file >> speed;
		map_file >> x_map_size;
		egg_nbr = static_cast<unsigned char>(x_map_size);
		map_file >> x_map_size;
		map_file >> y_map_size;

		map_size.x = x_map_size;
		map_size.y = y_map_size;

		for (unsigned int i = 0 ; i < x_map_size ; ++i) {
			areas_map.push_back(std::vector<Area>());
		}

		for (unsigned int i = 0 ; i < y_map_size ; ++i) {
			map_file >> value;
			for (unsigned int j = 0 ; j < x_map_size ; ++j) {
				switch (value[j]) {
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
		do {

			map_file >> ducks_spawn[player_nbr].x;
			--ducks_spawn[player_nbr].x;
			map_file >> ducks_spawn[player_nbr].y;
			--ducks_spawn[player_nbr].y;

			if (value == "up") {
				ducks_starting_dir[player_nbr] = UP;
			} else if (value == "down") {
				ducks_starting_dir[player_nbr] = DOWN;
			} else if (value == "left") {
				ducks_starting_dir[player_nbr] = LEFT;
			} else if (value == "right") {
				ducks_starting_dir[player_nbr] = RIGHT;
			} else {
				std::cout << "Not a valid direction : " << value << std::endl;
				std::cout << "(On player " << std::to_string(player_nbr + 1) << ")" << std::endl;
				ducks_starting_dir[player_nbr] = RIGHT;
				ret = false;
			}
			++player_nbr;
			map_file >> value;
		} while (value != "eof" && player_nbr < PLAYER_NBR_MAX);

		return ret;
	} else {
		std::cout << "Failed to open the map file" << std::endl;
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
void MapEditor::highlightTile(Coord tile, unsigned char area, sf::RenderWindow& window, sf::Color color) {

	if (area != 255) {
		ground_sprites[selected_biome][area].setPosition(
				sf::Vector2f(static_cast<float>(tile.x * 32), static_cast<float>(tile.y * 32)));
		ground_sprites[selected_biome][area].setColor(HIGHLIGHTER_INNERCOLOR);
		window.draw(ground_sprites[selected_biome][area]);
		ground_sprites[selected_biome][area].setColor(sf::Color(255, 255, 255, 255));
	}

	sf::RectangleShape highlighter(sf::Vector2f(32, 32));
	highlighter.setOutlineThickness(HIGHLIGHTER_THICKNESS);
	highlighter.setFillColor(sf::Color(0, 0, 0, 0));
	highlighter.setOutlineColor(color);
	highlighter.setPosition(sf::Vector2f(static_cast<float>(tile.x * 32), static_cast<float>(tile.y * 32)));
	window.draw(highlighter);
}

//////////////////////////////////////////////////////////////////////////////
short MapEditor::match(std::vector<std::string> biomes, std::string const& str) {
	for (short i(static_cast<short>(biomes.size())) ; i-- ;) {
		if (biomes[i] == str) {
			return i;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
unsigned char MapEditor::match(unsigned char area_nbr, Area area[], Area expr) {
	for (unsigned char i(area_nbr) ; i-- ;) {
		if (area[i] == expr) {
			return i;
		}
	}
	return 0;
}

void MapEditor::riverLinkIt(Area& departure_area, Area& arrival_area) {
	switch (mouse_relative_movmnt) {
		case UP:
			arrival_area = WATER_UD;
			switch (mouse_prev_relative_movmnt) {
				case UP:
					departure_area = WATER_UD;
					break;
				case LEFT:
					departure_area = WATER_UR;
					break;
				case DOWN:
					departure_area = WATER_UD;
					break;
				case RIGHT:
					departure_area = WATER_LU;
					break;
				case NOPE:
				default:
					break;
			}
			break;
		case LEFT:
			arrival_area = WATER_LR;
			switch (mouse_prev_relative_movmnt) {
				case UP:
					departure_area = WATER_DL;
					break;
				case LEFT:
					departure_area = WATER_LR;
					break;
				case DOWN:
					departure_area = WATER_LU;
					break;
				case RIGHT:
					departure_area = WATER_LR;
					break;
				case NOPE:
				default:
					departure_area = EMPTY_TILE;
					break;
			}
			break;
		case DOWN:
			arrival_area = WATER_UD;
			switch (mouse_prev_relative_movmnt) {
				case UP:
					departure_area = WATER_UD;
					break;
				case LEFT:
					departure_area = WATER_RD;
					break;
				case DOWN:
					departure_area = WATER_UD;
					break;
				case RIGHT:
					departure_area = WATER_DL;
					break;
				case NOPE:
				default:
					departure_area = EMPTY_TILE;
					break;
			}
			break;
		case RIGHT:
			arrival_area = WATER_LR;
			switch (mouse_prev_relative_movmnt) {
				case UP:
					departure_area = WATER_RD;
					break;
				case LEFT:
					break;
				case DOWN:
					departure_area = WATER_UR;
					break;
				case RIGHT:
					departure_area = WATER_LR;
					break;
				case NOPE:
				default:
					departure_area = EMPTY_TILE;
					break;
			}
			break;
		case NOPE:
		default:
			arrival_area = EMPTY_TILE;
			break;
	}
}
