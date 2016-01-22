#include <Game.hpp>

int main(int argc, char* argv[]);
int main(int argc, char* argv[]){
	if (argc <= 1)  {
		Game game;
		game.launch();
	}
	else{
		Game game("res/", std::string(argv[1]) + '/', "ducks/");
		game.launch();
	}
	return 0;
}
