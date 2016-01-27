/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef NETWORKGAME_HPP_INCLUDED
#define NETWORKGAME_HPP_INCLUDED

#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class NetworkGame{

public:

	NetworkGame(Game& loaded_game){ game = loaded_game; isServer = true;}
	NetworkGame(){bool isServer = false;}
	void launch();

private:
	bool isServer;
	Game game;
};

#endif /* NETWORKGAME_HPP_INCLUDED */
