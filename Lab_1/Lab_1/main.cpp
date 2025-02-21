#include "Game/game.h"
#include <locale>

int main()
{
	setlocale(LC_ALL, "Russian");

	Game game;
	game.init();
	game.run();
}


