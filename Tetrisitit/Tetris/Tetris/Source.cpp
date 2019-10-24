#include "Game.h"

//WoooaaOOw :OOOOOO
int main() 
{
	Game* myGame{ new Game{} };
	myGame->Run();
	delete myGame;
	return 0;
}