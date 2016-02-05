 /***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Menu.hpp>

static const struct {
  int           width;
  int           height;
  int           bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char pixel_data[16 * 16 * 4 + 1];
} sfml_icon = {
  16, 16, 4,
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\377\26k\0\377\26k\0\377\26k\0\377\0\0\0\377\377"
  "\377\377\377\31)x\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0"
  "\377\26k\0\377\26k\0\377\26k\0\377\0\0\0\377\305\202\1\377\305\202\1\377"
  "\371\244\0\377\371\244\0\377\371\244\0\377\0\0\0\0\0\0\0\377\26k\0\377\26"
  "k\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377\0\0\0"
  "\377\371\244\0\377\371\244\0\377\371\244\0\377\371\244\0\377\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\377\0\0\0\377\26k\0\377\26k\0\377\26k\0\377\26k\0\377"
  "\26k\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\377"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\0\0\0\377\377\377\377\377\0\0\0"
  "\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\377\377"
  "\377\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0\0\377\377\377"
  "\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\377\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377"
  "\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377"
  "\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\377\377\377\377\377"
  "\377\377\377\377\377\377\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0",
};

Menu::Menu(){
	title_theme.openFromFile("res/sounds/title_theme.ogg");
	title_theme.setLoop(true);
	title_theme.play();
	window.create(sf::VideoMode(MENU_X_RESOLUTION, MENU_Y_RESOLUTION), "PapraGame ~ A game with Ducks !", sf::Style::Titlebar | sf::Style::Close);
	window.setIcon( sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data );

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

	background_sprite.setPosition(static_cast<float>(bg_pos), -64);
	window.draw(background_sprite);
	title_sprite.setPosition(0,0);
	window.draw(title_sprite);

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

					case 0:{
						map_path = this->mapMenu();
						if(map_path.size() > 0){

							sf::Text countdown;
							countdown.setFont(font);
							countdown.setCharacterSize(100);
							countdown.setColor(TXT_COLOR);
							char i = '3';
							countdown.setString(std::string() + i);
							countdown.setPosition(static_cast<float>((MENU_X_RESOLUTION) - countdown.getGlobalBounds().width) / 2.0f, static_cast<float>(MENU_Y_RESOLUTION - countdown.getGlobalBounds().height) / 2.0f);
							sf::Clock elapsed_time;
							while(i > '0'){
								--bg_pos;
								if(bg_pos < -BACKGROUND_LOOP_WIDTH)
									bg_pos = 0;
								background_sprite.setPosition(static_cast<float>(bg_pos), -64);
								window.draw(background_sprite);
								window.draw(title_sprite);
								window.draw(countdown);
								window.display();
								sf::sleep(sf::milliseconds(REFRESH_SLOWNESS));
								if(elapsed_time.getElapsedTime().asSeconds() > 1.0f){
									--i;
									countdown.setString(std::string() + i);
									countdown.setPosition(static_cast<float>((MENU_X_RESOLUTION) - countdown.getGlobalBounds().width) / 2.0f, static_cast<float>(MENU_Y_RESOLUTION - countdown.getGlobalBounds().height) / 2.0f);
									elapsed_time.restart();
								}
							}
							title_theme.stop();
							game.launch(window, map_path);
							title_theme.play();
						}
						done = false;
						break;
					}
					case 1:{
						bool network_done(false);
						while(!network_done){
							switch(this->networkMenu()){
								case 0:{
									map_path = this->mapMenu();
									if(map_path.size() > 0){
										GameServer server("res/", biome_path, map_path);
										if (server.getClients(window)){
												title_theme.stop();
												server.launch(window);
												title_theme.play();
										}
										network_done = true;
									}
									break;
								}
								case 1:{
									GameClient client(biome_path, false);
									title_theme.stop();
									client.launch(window);
									title_theme.play();
									network_done = true;
									break;
								}
								default:{
									network_done = true;
									break;
								}
							}
						}
						done = false;
						break;
					}
					case 2:{
					default:
						window.close();
						break;
					}
				}
				window.setSize(sf::Vector2u(MENU_X_RESOLUTION, MENU_Y_RESOLUTION));
				window.setView(sf::View(sf::FloatRect(0, 0, MENU_X_RESOLUTION, MENU_Y_RESOLUTION)));
			}
		}

		window.clear();
		rectangle[old_user_choice].setOutlineColor(NORMAL_OUTLINE_COLOR);
		rectangle[user_choice].setOutlineColor(SELECTED_OUTLINE_COLOR);
		--bg_pos;
		if(bg_pos < -BACKGROUND_LOOP_WIDTH)
			bg_pos = 0;
		background_sprite.setPosition(static_cast<float>(bg_pos), -64);
		window.draw(background_sprite);
		window.draw(title_sprite);
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
	std::vector<sf::Vector2f> rect_coord;
	std::vector<sf::Vector2f> text_coord;

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
		//creation of rectangle
		rectangle.push_back(sf::RectangleShape(sf::Vector2f(MAP_BUTTON_WIDTH, MAP_BUTTON_HEIGHT)));
		rectangle[0].setOrigin(0,-TITLE_HEIGHT);
		rectangle[0].setFillColor(BUTTON_FILL_COLOR);
		rect_coord.push_back(sf::Vector2f(static_cast<float>((MENU_X_RESOLUTION - MAP_BUTTON_WIDTH) / 2), MAP_BUTTON_SPACE));
		rectangle[0].setPosition(rect_coord[0]);
		rectangle[0].setOutlineThickness(OUTLINE_THICKNESS);
		rectangle[0].setOutlineColor(SELECTED_OUTLINE_COLOR);

		//creation of text
		text.push_back(sf::Text());
		text[0].setString(NO_MAPS_TXT);
		text[0].setFont(font);
		text[0].setCharacterSize(MAP_FONT_SIZE);
		text[0].setOrigin(0,-TITLE_HEIGHT);
		text[0].setColor(TXT_COLOR);
		text_coord.push_back(sf::Vector2f(static_cast<float>((MENU_X_RESOLUTION) - text[0].getGlobalBounds().width) / 2.0f, static_cast<float>((MAP_BUTTON_HEIGHT - MAP_FONT_SIZE) / 2) + MAP_BUTTON_SPACE - text[0].getGlobalBounds().height / 4.0f));
		text[0].setPosition(text_coord[0]);
	}
	else{

		//creation of rectangles
		for(unsigned char i = 0; i < maps.size() + 1; ++i){ //+1 for the back button
			rectangle.push_back(sf::RectangleShape(sf::Vector2f(MAP_BUTTON_WIDTH, MAP_BUTTON_HEIGHT)));
			rectangle[i].setOrigin(0,-TITLE_HEIGHT);
			rectangle[i].setFillColor(BUTTON_FILL_COLOR);
			rect_coord.push_back(sf::Vector2f(static_cast<float>((MENU_X_RESOLUTION - MAP_BUTTON_WIDTH) / 2), static_cast<float>(i * (MAP_BUTTON_HEIGHT + MAP_BUTTON_SPACE) + MAP_BUTTON_SPACE)));
			rectangle[i].setPosition(rect_coord[i]);
			rectangle[i].setOutlineThickness(OUTLINE_THICKNESS);
			rectangle[i].setOutlineColor(NORMAL_OUTLINE_COLOR);
		}
		rectangle[0].setOutlineColor(SELECTED_OUTLINE_COLOR);

		//creation of texts
		for(unsigned char i = 0; i < maps.size(); ++i){
			text.push_back(sf::Text());
			text[i].setString(maps[i]);
		}
		text.push_back(sf::Text());
		text[maps.size()].setString(BACK_TXT);
		for(unsigned char i = 0; i < maps.size() + 1; ++i){ //+1 for the back button
			text[i].setFont(font);
			text[i].setCharacterSize(MAP_FONT_SIZE);
			text[i].setOrigin(0,-TITLE_HEIGHT);
			text[i].setColor(TXT_COLOR);
			text_coord.push_back(sf::Vector2f(static_cast<float>((MENU_X_RESOLUTION) - text[i].getGlobalBounds().width) / 2.0f, static_cast<float>(i * (MAP_BUTTON_HEIGHT + MAP_BUTTON_SPACE) + (MAP_BUTTON_HEIGHT - MAP_FONT_SIZE) / 2) + MAP_BUTTON_SPACE - text[i].getGlobalBounds().height / 4.0f));
			text[i].setPosition(text_coord[i]);
		}
	}

	char user_choice = 0;
	char old_user_choice = 0;
	char nbr_of_choices = static_cast<char>(maps.size() + 1);
	int scrolling = 0;

	background_sprite.setPosition(static_cast<float>(bg_pos), -64);
	window.draw(background_sprite);
	title_sprite.setPosition(0,0);
	window.draw(title_sprite);

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

				while(rectangle[user_choice].getPosition().y > (MENU_Y_RESOLUTION - TITLE_HEIGHT - MAP_BUTTON_HEIGHT - MAP_BUTTON_SPACE)){
					scrolling += (MAP_BUTTON_SPACE + MAP_BUTTON_HEIGHT);
					for(unsigned char i = nbr_of_choices; i--;){
						rectangle[i].setPosition(rect_coord[i].x, rect_coord[i].y - static_cast<float>(scrolling));
						text[i].setPosition(text_coord[i].x, text_coord[i].y - static_cast<float>(scrolling));
					}
				}
				while(rectangle[user_choice].getPosition().y < MAP_BUTTON_SPACE){
					scrolling -= (MAP_BUTTON_SPACE + MAP_BUTTON_HEIGHT);
					for(unsigned char i = nbr_of_choices; i--;){
						rectangle[i].setPosition(rect_coord[i].x, rect_coord[i].y - static_cast<float>(scrolling));
						text[i].setPosition(text_coord[i].x, text_coord[i].y - static_cast<float>(scrolling));
					}
				}
			}
			else if(event.type == sf::Event::MouseMoved){
				if(sf::Mouse::getPosition(window).y > MINIMAL_Y_MOUSE){
					for(unsigned char i = nbr_of_choices; i--;){
						if(((sf::Mouse::getPosition(window).y - TITLE_HEIGHT) > rectangle[i].getPosition().y) && ((sf::Mouse::getPosition(window).y - TITLE_HEIGHT) < (rectangle[i].getPosition().y + MAP_BUTTON_HEIGHT)))
							user_choice = i;
					}
				}
			}
			else if(event.type == sf::Event::MouseButtonPressed){
				if(sf::Mouse::getPosition(window).y > MINIMAL_Y_MOUSE){
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
			else if(event.type == sf::Event::MouseWheelScrolled){
				if(event.mouseWheelScroll.delta > 0 && (rectangle[0].getPosition().y < MAP_BUTTON_SPACE)){
					scrolling -= (MAP_BUTTON_SPACE + MAP_BUTTON_HEIGHT);
					for(unsigned char i = nbr_of_choices; i--;){
						rectangle[i].setPosition(rect_coord[i].x, rect_coord[i].y - static_cast<float>(scrolling));
						text[i].setPosition(text_coord[i].x, text_coord[i].y - static_cast<float>(scrolling));
					}
				}
				else if(event.mouseWheelScroll.delta < 0 && (rectangle[nbr_of_choices - 1].getPosition().y > (MENU_Y_RESOLUTION - TITLE_HEIGHT - MAP_BUTTON_SPACE))){
					scrolling += (MAP_BUTTON_SPACE + MAP_BUTTON_HEIGHT);
					for(unsigned char i = nbr_of_choices; i--;){
						rectangle[i].setPosition(rect_coord[i].x, rect_coord[i].y - static_cast<float>(scrolling));
						text[i].setPosition(text_coord[i].x, text_coord[i].y - static_cast<float>(scrolling));
					}
				}
				while(rectangle[user_choice].getPosition().y < 0)
					++user_choice;
				while(rectangle[user_choice].getPosition().y > (MENU_Y_RESOLUTION - TITLE_HEIGHT - MAP_BUTTON_HEIGHT))
					--user_choice;
				for(unsigned char i = nbr_of_choices; i--;){
					if(((sf::Mouse::getPosition(window).y - TITLE_HEIGHT) > rectangle[i].getPosition().y) && ((sf::Mouse::getPosition(window).y - TITLE_HEIGHT) < (rectangle[i].getPosition().y + MAP_BUTTON_HEIGHT)))
						user_choice = i;
				}
			}
		}

		window.clear();
		rectangle[old_user_choice].setOutlineColor(NORMAL_OUTLINE_COLOR);
		rectangle[user_choice].setOutlineColor(SELECTED_OUTLINE_COLOR);
		--bg_pos;
		if(bg_pos < -BACKGROUND_LOOP_WIDTH)
			bg_pos = 0;
		background_sprite.setPosition(static_cast<float>(bg_pos), -64);
		window.draw(background_sprite);
		for(unsigned char i = nbr_of_choices; i--;){
			window.draw(rectangle[i]);
			window.draw(text[i]);
		}
		window.draw(title_sprite);
		window.display();
		old_user_choice = user_choice;
	}
	if(user_choice == (nbr_of_choices - 1))
		return std::string("");
	else
		return (maps_path + maps[user_choice] + ".map");
}

char Menu::networkMenu(){

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

	text[0].setString(SERVER_TXT);
	text[1].setString(CLIENT_TXT);
	text[2].setString(BACK_TXT);

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

	window.draw(background_sprite);
	window.draw(title_sprite);

	for(unsigned char i = nbr_of_choices; i--;){
		window.draw(rectangle[i]);
		window.draw(text[i]);
	}
	window.display();

	bool done(false);
	sf::Event event;
	while (window.isOpen() && !done){
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
		}

		window.clear();
		rectangle[old_user_choice].setOutlineColor(NORMAL_OUTLINE_COLOR);
		rectangle[user_choice].setOutlineColor(SELECTED_OUTLINE_COLOR);
		--bg_pos;
		if(bg_pos < -BACKGROUND_LOOP_WIDTH)
			bg_pos = 0;
		background_sprite.setPosition(static_cast<float>(bg_pos), -64);
		window.draw(background_sprite);
		window.draw(title_sprite);
		for(unsigned char i = nbr_of_choices; i--;){
			window.draw(rectangle[i]);
			window.draw(text[i]);
		}
		window.display();
		old_user_choice = user_choice;
	}
	return user_choice;
}
