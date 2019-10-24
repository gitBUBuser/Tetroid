#include "Board.h"
#include "Game.h"


Board::Board(const int& aWidth, const int& aHeight, const int& middleX, const int& middleY, Game* aGame)
{
	myStartX = middleX - (aWidth / 2);
	myStartY = middleY - (aHeight / 2);

	myEndX = middleX + (aWidth / 2);
	myEndY = middleY + (aHeight / 2);

	myGame = aGame;
}

//Förklarar sig själv
int Board::GetStartX() 
{
	return myStartX;
}

//Förklarar sig själv
int Board::GetEndX() 
{
	return myEndX;
}

//Förklarar sig själv
int Board::GetEndY() 
{
	return myEndY;
}

//Säger till buffern hur den ska rita ut spelplanen
void Board::Draw() 
{
	for (int i = myStartX; i < myEndX; i++)
	{
		myGame->SetCharacter(Vector2D(i, myStartY), gameData::BOX, gameData::Color::White);
		myGame->SetCharacter(Vector2D(i, myEndY), gameData::BOX, gameData::Color::White);
	}
	for (int i = myStartY; i < myEndY; i++)
	{
		myGame->SetCharacter(Vector2D(myStartX, i), gameData::BOX, gameData::Color::White);
		myGame->SetCharacter(Vector2D(myEndX, i), gameData::BOX, gameData::Color::White);
	}
	myGame->SetCharacter(Vector2D(myEndX, myEndY), gameData::BOX, gameData::Color::White);
}
