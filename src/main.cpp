/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Game.hpp>
#include <Menu.hpp>
#include <string>
#include <X11/Xlib.h>

int main(int argc, char* argv[]);
int main(int argc, char* argv[]){

	XInitThreads();
	if (argc <= 1)  {
		Menu menu;
		menu.mainMenu();
	}
	else{
		if (std::string(argv[1]) == "--version") {
			std::cout << "PapraGame v1.0 -- Temerarium Anatum " << std::endl <<
				"Copyright c 2016 TiWinDeTea (https://github.com/TiWinDeTea). Game under the " <<
				"Mozilla Public License v2.0" << std::endl <<
				std::endl <<
				"License extract : " << std::endl <<
				"Covered Software is provided under this License on an \"as is\"" << std::endl <<
				"basis, without warranty of any kind, either expressed, implied, or" << std::endl <<
				"statutory, including, without limitation, warranties that the" << std::endl <<
				"Covered Software is free of defects, merchantable, fit for a" << std::endl <<
				"particular purpose or non-infringing. The entire risk as to the" << std::endl <<
				"quality and performance of the Covered Software is with You." << std::endl <<
				"Should any Covered Software prove defective in any respect, You" << std::endl <<
				"(not any Contributor) assume the cost of any necessary servicing," << std::endl <<
				"repair, or correction. This disclaimer of warranty constitutes an" << std::endl <<
				"essential part of this License. No use of any Covered Software is" << std::endl <<
				"authorized under this License except under this disclaimer." << std::endl <<
				std::endl;
		}
		else{
			Menu menu;
			menu.setBiome(std::string(argv[1]));
			menu.mainMenu();
		}
	}
	return 0;
}
