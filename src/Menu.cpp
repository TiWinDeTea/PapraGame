 /***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Menu.hpp>

Menu::Menu(){
	window.create(sf::VideoMode(MENU_X_RESOLUTION, MENU_Y_RESOLUTION), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);

	if (!font.loadFromFile("res/fonts/slkscr.ttf"))
		std::cout << "Error loading font" << std::endl;

	res_path = RES_PATH;
	biome_path = BIOME_PATH;
	ducks_path = DUCKS_PATH;
	maps_path = MAPS_PATH;
	menu_path = MENU_PATH;

	title_texture.loadFromFile(res_path + menu_path + TEXTURE_TITLE + TEXTURE_FILETYPE);
	background_texture.loadFromFile(res_path + menu_path + TEXTURE_BACKGROUND + TEXTURE_FILETYPE);
	title_sprite.setTexture(title_texture);
	background_sprite.setTexture(background_texture);
	background_sprite.setOrigin(0, - TITLE_HEIGHT);

	bg_pos = -32;
}

void Menu::setBiome(std::string chosen_biome){
	
	biome_path = chosen_biome.append("/");
}

void Menu::mainMenu(){

	window.clear(sf::Color::White);

	std::vector<sf::RectangleShape> rectangle;
	std::vector<sf::Text> text;

	//creation of rectangles
	for(unsigned char i = 0; i < 3; ++i){
		rectangle.push_back(sf::RectangleShape(sf::Vector2f(MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT)));
		rectangle[i].setOrigin(0,-TITLE_HEIGHT);
		rectangle[i].setFillColor(BUTTON_FILL_COLOR);
		rectangle[i].setPosition(static_cast<float>((MENU_X_RESOLUTION - MAIN_BUTTON_WIDTH) / 2), static_cast<float>(i * (MAIN_BUTTON_HEIGHT + MAIN_BUTTON_SPACE) + MAIN_BUTTON_SPACE));
		rectangle[i].setOutlineThickness(OUTLINE_THICKNESS);
		rectangle[i].setOutlineColor(NORMAL_OUTLINE_COLOR);
	}
	rectangle[0].setOutlineColor(SELECTED_OUTLINE_COLOR);

	//creation of texts
	for(unsigned char i = 0; i < 3; ++i)
		text.push_back(sf::Text());

	text[0].setString(LOCAL_TXT);
	text[1].setString(NETWORK_TXT);
	text[2].setString(EXIT_TXT);

	for(unsigned char i = 0; i < 3; ++i){
		text[i].setFont(font);
		text[i].setCharacterSize(MAIN_FONT_SIZE);
		text[i].setOrigin(0,-TITLE_HEIGHT);
		text[i].setColor(TXT_COLOR);
		text[i].setPosition(static_cast<float>((MENU_X_RESOLUTION) - text[i].getGlobalBounds().width) / 2.0f, static_cast<float>(i * (MAIN_BUTTON_HEIGHT + MAIN_BUTTON_SPACE) + (MAIN_BUTTON_HEIGHT - MAIN_FONT_SIZE) / 2) + MAIN_BUTTON_SPACE - text[i].getGlobalBounds().height / 4.0f);
	}

	char user_choice = 0;
	char old_user_choice = 0;
	char nbr_of_choices = static_cast<char>(rectangle.size());

	background_sprite.setPosition(static_cast<float>(bg_pos), 0);
	title_sprite.setPosition(0,0);
	window.draw(title_sprite);
	window.draw(background_sprite);

	for(unsigned char i = nbr_of_choices; i--;){
		window.draw(rectangle[i]);
		window.draw(text[i]);
	}
	window.display();

	bool done(false);
	sf::Event event;
	while (window.isOpen()){
		sf::sleep(sf::milliseconds(REFRESH_SLOWNESS));
		while (window.pollEvent(event)){

			if(event.type == sf::Event::Closed)
				window.close();
			else if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Key::Up){
					--user_choice;
				}
				else if(event.key.code == sf::Keyboard::Key::Down){
					++user_choice;
				}
				else if(event.key.code == sf::Keyboard::Key::PageUp){
					user_choice = 0;
				}
				else if(event.key.code == sf::Keyboard::Key::PageDown){
					user_choice = static_cast<char>(nbr_of_choices - 1);
				}
				else if(event.key.code == sf::Keyboard::Key::Return){
					done = true;
				}
				else if(event.key.code == sf::Keyboard::Key::Escape){
					user_choice = static_cast<char>(nbr_of_choices - 1);
					done = true;
				}
				if(user_choice >= nbr_of_choices)
					user_choice = static_cast<char>(user_choice - nbr_of_choices);
				if(user_choice < 0)
					user_choice = static_cast<char>(user_choice + nbr_of_choices);
			}
			else if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Button::Left){
					for(unsigned char i = nbr_of_choices; i--;){
						if(rectangle[i].getGlobalBounds().contains(sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))){
							user_choice = i;
							done = true;
						}
					}
				}
			}
			else if(event.type == sf::Event::MouseMoved){
				for(unsigned char i = nbr_of_choices; i--;){
					if(rectangle[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
						user_choice = i;
				}
			}

			if(done){
				Game game(res_path, biome_path, ducks_path);
				std::string tmp;
				switch(user_choice){

					case 0:
						map_path = this->mapMenu();
						game.launch(window, map_path);
						done = false;
						break;

					case 1:
						std::cout << "Server ? [y/n] : ";
						std::cin >> tmp;
						if (tmp == "y") {
							GameServer server("res/", "nope", this->mapMenu());
							sf::Thread server_thread(&GameServer::launch, &server);
							server_thread.launch();
							GameClient client(false);
							client.launch(window);
							server_thread.wait();
						}
						else
						{
							GameClient client(false);
							client.launch(window);

						}
						done = false;
						break;

					case 2:
					default:
						window.close();
						break;
				}
				window.setSize(sf::Vector2u(MENU_X_RESOLUTION, MENU_Y_RESOLUTION));
				window.setView(sf::View(sf::FloatRect(0, 0, MENU_X_RESOLUTION, MENU_Y_RESOLUTION)));
			}
		}

		window.clear();
		window.clear(sf::Color::White);
		rectangle[old_user_choice].setOutlineColor(NORMAL_OUTLINE_COLOR);
		rectangle[user_choice].setOutlineColor(SELECTED_OUTLINE_COLOR);
		window.draw(title_sprite);
		--bg_pos;
		if(bg_pos < -BACKGROUND_LOOP_WIDTH)
			bg_pos = 0;
		background_sprite.setPosition(static_cast<float>(bg_pos), 0);
		window.draw(background_sprite);
		for(unsigned char i = nbr_of_choices; i--;){
			window.draw(rectangle[i]);
			window.draw(text[i]);
		}
		window.display();
		old_user_choice = user_choice;
	}
}

std::string Menu::mapMenu(){

	DIR* directory = opendir(std::string(maps_path).c_str());
	struct dirent* redfile = NULL;
	std::vector<std::string> maps;
	std::vector<sf::RectangleShape> rectangle;
	std::vector<sf::Text> text;

	window.clear(sf::Color::White);

	if( directory != NULL ){

		auto isFile = [](std::string const& local_path) -> bool {
			#ifdef OS_WINDOWS
				std::ifstream file (local_path.c_str(), std::ios::in);
				return !(file.fail());
			#else
				struct stat s;
				if( stat(local_path.c_str() ,&s ) == 0)
					return !(s.st_mode & S_IFDIR);
				else
					return false;
			#endif
		};

		while ((redfile = readdir(directory)) != NULL){
			std::string tmp( redfile->d_name );
			if (isFile(maps_path + "/" + tmp) && tmp.size() > 4 && tmp.substr(tmp.size() - 4) == ".map") {
				tmp.erase(tmp.size() - 4);
				maps.push_back(tmp);
			}
		}
	}

	if (maps.size() == 0) {
		std::cout << "No file found !" << std::endl;
	}
	else{
		
		//creation of rectangles
		for(unsigned char i = 0; i < maps.size(); ++i){
			rectangle.push_back(sf::RectangleShape(sf::Vector2f(MAP_BUTTON_WIDTH, MAP_BUTTON_HEIGHT)));
			rectangle[i].setOrigin(0,-TITLE_HEIGHT);
			rectangle[i].setFillColor(BUTTON_FILL_COLOR);
			rectangle[i].setPosition(static_cast<float>((MENU_X_RESOLUTION - MAP_BUTTON_WIDTH) / 2), static_cast<float>(i * (MAP_BUTTON_HEIGHT + MAP_BUTTON_SPACE) + MAP_BUTTON_SPACE));
			rectangle[i].setOutlineThickness(OUTLINE_THICKNESS);
			rectangle[i].setOutlineColor(NORMAL_OUTLINE_COLOR);
		}
		rectangle[0].setOutlineColor(SELECTED_OUTLINE_COLOR);

		//creation of texts
		for(unsigned char i = 0; i < maps.size(); ++i){
			text.push_back(sf::Text());
			text[i].setString(maps[i]);
			text[i].setFont(font);
			text[i].setCharacterSize(MAP_FONT_SIZE);
			text[i].setOrigin(0,-TITLE_HEIGHT);
			text[i].setColor(TXT_COLOR);
			text[i].setPosition(static_cast<float>((MENU_X_RESOLUTION) - text[i].getGlobalBounds().width) / 2.0f, static_cast<float>(i * (MAP_BUTTON_HEIGHT + MAP_BUTTON_SPACE) + (MAP_BUTTON_HEIGHT - MAP_FONT_SIZE) / 2) + MAP_BUTTON_SPACE - text[i].getGlobalBounds().height / 4.0f);
		}
	}

	char user_choice = 0;
	char old_user_choice = 0;
	char nbr_of_choices = static_cast<char>(maps.size());

	background_sprite.setPosition(static_cast<float>(bg_pos), 0);
	title_sprite.setPosition(0,0);
	window.draw(title_sprite);
	window.draw(background_sprite);

	for(unsigned char i = nbr_of_choices; i--;){
		window.draw(rectangle[i]);
		window.draw(text[i]);
	}
	window.display();

	bool done(false);
	sf::Event event;
	while (!done){
		sf::sleep(sf::milliseconds(REFRESH_SLOWNESS));
		while (window.pollEvent(event)){

			if(event.type == sf::Event::Closed){
				window.close();
				done = true;
			}
			else if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Key::Up){
					--user_choice;
				}
				else if(event.key.code == sf::Keyboard::Key::Down){
					++user_choice;
				}
				else if(event.key.code == sf::Keyboard::Key::PageUp){
					user_choice = 0;
				}
				else if(event.key.code == sf::Keyboard::Key::PageDown){
					user_choice = static_cast<char>(nbr_of_choices - 1);
				}
				else if(event.key.code == sf::Keyboard::Key::Return){
					done = true;
				}
				else if(event.key.code == sf::Keyboard::Key::Escape){
					user_choice = static_cast<char>(nbr_of_choices - 1);
					done = true;
				}
				if(user_choice >= nbr_of_choices)
					user_choice = static_cast<char>(user_choice - nbr_of_choices);
				if(user_choice < 0)
					user_choice = static_cast<char>(user_choice + nbr_of_choices);
			}
			else if(event.type == sf::Event::MouseMoved){
				for(unsigned char i = nbr_of_choices; i--;){
					if(rectangle[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
						user_choice = i;
				}
			}
			else if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Button::Left){
					for(unsigned char i = nbr_of_choices; i--;){
						if(rectangle[i].getGlobalBounds().contains(sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))){
							user_choice = i;
							done = true;
						}
					}
				}
			}
			
		}

		window.clear();
		window.clear(sf::Color::White);
		rectangle[old_user_choice].setOutlineColor(NORMAL_OUTLINE_COLOR);
		rectangle[user_choice].setOutlineColor(SELECTED_OUTLINE_COLOR);
		window.draw(title_sprite);
		--bg_pos;
		if(bg_pos < -BACKGROUND_LOOP_WIDTH)
			bg_pos = 0;
		background_sprite.setPosition(static_cast<float>(bg_pos), 0);
		window.draw(background_sprite);
		for(unsigned char i = nbr_of_choices; i--;){
			window.draw(rectangle[i]);
			window.draw(text[i]);
		}
		window.display();
		old_user_choice = user_choice;
	}
	return (maps_path + maps[user_choice] + ".map");
}
