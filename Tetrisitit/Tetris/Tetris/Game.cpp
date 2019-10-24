#include "Game.h"
#include "GameData.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include "Vector2D.h"
#include <wchar.h>
#include "Tetromino.h"


Game::Game()
{
	myGameOver = false;
	//Ändrar konsol inställningar
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SetFontSize(gameData::TEXT_WIDTH, gameData::TEXT_HEIGHT);
	SetWindowSize(gameData::SCREEN_WIDTH, gameData::SCREEN_HEIGHT);
	myWindowHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//Skapar en buffer (credit till Viktor Andersson)
	InitializeBuffer();
	//Skapar en board
	Board *board{ new Board{gameData::BOARD_WIDTH, gameData::BOARD_HEIGHT,gameData::BoardMiddleX() ,gameData::BoardMiddleY(), this } };
	myBoard = board;
	NewTetromino();
}

//Kör spelet 
void Game::Run() 
{
	while (!myGameOver) 
	{
		//Används för att räkna ut tiden mellan uppdateringar (deltaTime)
		auto start = std::chrono::system_clock::now();

        /*
		Skapar en temporär kopia av myTetrominos för att kunna köra en
		for-loop där objekt i myTetrominos förstörs.
		*/
		std::vector<Tetromino*> tempMinos = myTetrominos;
		for (int i = 0; i < tempMinos.size(); i++)
		{
			tempMinos[i]->DestroyCheck();
		}

		//Skapar en ny Tetromino om spelet kräver det
		if (myWantNewTetromino) 
		{
			myWantNewTetromino = false;
			Vector2D temp[4];
			std::srand(time(NULL));
			int random = std::rand() % 7;
			ConvertShapeToPosition(shapes[random], temp);
			Tetromino* tetromino{ new Tetromino{ this, std::vector<Vector2D>(temp,temp + sizeof temp / sizeof temp[0]) } };
			random = 0;
			myCurrentTetromino = tetromino;
			myTetrominos.push_back(tetromino);
		}
		else 
		{ 
			//Kollar efter spelarens input
			Input();
			//Uppdaterar alla Tetrominos
			for (int i = 0; i < myTetrominos.size(); i++)
				myTetrominos[i]->Update();

			/*
			Tömmer Buffern, detta göra här istället för före Loopen ovan för att
			p.g.a. Buffern används till kollisioner i Update.
			*/
			ClearBuffer();

			//Ger buffern värden
			for (int i = 0; i < myTetrominos.size(); i++)
				myTetrominos[i]->Draw();

			myBoard->Draw();

			std::string scoreString = "Score: " + std::to_string(myScore);
			for (int i = 0; i < scoreString.length(); i++)
				SetCharacter(Vector2D{ gameData::BoardMiddleX() + i - (gameData::BOARD_WIDTH / 2),
					gameData::BoardMiddleY() - (gameData::BOARD_HEIGHT / 2) - 1 },
					scoreString[i], gameData::Green);

			//Ritar ut buffern
			DrawBuffer();
		}
		//Räknar ut tiden det tog för Run att köra
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = end - start;
		myDeltaTime = (elapsedTime.count());
	}
}

//Förklarar sig själv
bool Game::IsCurrentTetromino(const Tetromino* aTetromino) 
{
	return (myCurrentTetromino == aTetromino);
}

//Tar bort en given Tetromino från listan myTetrominos
void Game::RemoveTetromino(const Tetromino* aTetromino) 
{
	std::vector<Tetromino*>::iterator it = std::find(myTetrominos.begin(), myTetrominos.end(), aTetromino);
	myTetrominos.erase(it);
}

//Timer
bool Game::Timer(const float& someMaxTime, float& someCurrentTime) 
{
	//Plussar på den en tids variabel med tiden mellan uppdateringar
	someCurrentTime += myDeltaTime;
	//Kollar om tids variabeln är mer än en given max tid (returnerar sant i så fall)
	if (someCurrentTime > someMaxTime) 
	{
		someCurrentTime = 0;
		return true;
	}
	return false;
}

//Sätter storleken av konsol fönstret
void Game::SetWindowSize(const int& aWidth, const int& aHeight) 
{
	COORD bufferSize;
	bufferSize.X = aWidth;
	bufferSize.Y = aHeight;

	SMALL_RECT windowRectangle;
	windowRectangle.Top = windowRectangle.Left = 0;
	windowRectangle.Bottom = aHeight - 1;
	windowRectangle.Right = aWidth - 1;

	HANDLE handle{ GetStdHandle(STD_OUTPUT_HANDLE) };
	SetConsoleScreenBufferSize(handle, bufferSize);
	SetConsoleWindowInfo(handle, TRUE, &windowRectangle);
}

//Sätter storleken av texten i konsolen
void Game::SetFontSize(const int& aWidth, const int& aHeight)
{
	CONSOLE_FONT_INFOEX consoleFontInfo;
	consoleFontInfo.cbSize = sizeof(consoleFontInfo);
	consoleFontInfo.dwFontSize.X = aWidth;
	consoleFontInfo.dwFontSize.Y = aHeight;
	consoleFontInfo.FontFamily = FF_DONTCARE;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &consoleFontInfo);
}

//Förklarar sig själv
void Game::GameOver()
{
	myGameOver = true;
}

//Kollar om en position i buffern är upptagen
bool Game::PositionIsTaken(const Vector2D& aPosition) 
{
	CHAR_INFO& character{ myBuffer[aPosition.GetY()][aPosition.GetX()] };
	if (character.Char.AsciiChar != ' ') 
		return true;

	return false;
}

//Förklarar sig själv
Board* Game::GetBoard() 
{
	return myBoard;
}

//Tar bort en horisontell rad vid den givna vertikala positionen
void Game::DestroyRow(const int& aY) 
{
	for (int i = 0; i < myTetrominos.size(); i++)
	{
		std::vector<int> temp;

		for (int j = 0; j < myTetrominos[i]->GetShape().size(); j++)
			if ((myTetrominos[i]->GetShape()[j] + myTetrominos[i]->GetPosition()).GetY() == aY)			
				temp.push_back(j);
		
		for (int j = 0; j < temp.size(); j++)
			myTetrominos[i]->RemoveBrick(temp[j] - j);
	}
}

//Förklarar sig själv
void Game::AddScore(const int& anAmount) 
{
	myScore += anAmount;
}

void Game::Input() {

	//Mellanslag (placerar Tetrominon på marken)
	if (GetKeyState(0x20) & 0x8000) 
	{
		if (!myJustLanded)
		{
			myJustLanded = true;
			myCurrentTetromino->Land();
		}
		return;
	}
	else 
		myJustLanded = false;

	//Upp (roterar Tetrominon)
	if (GetKeyState(0x26) & 0x8000) 
	{
		if (!myJustRotated) 
		{
			myJustRotated = true;
			myCurrentTetromino->Rotate();
		}
		return;
	}
	else 
		myJustRotated = false;

	/*
	Ingen av Inputerna (förutom Landd() och Rotate()) flyttar direkt på
	Tetrominon, utan säger hällre till Tetrominon att spelaren vill flytta i en riktning
	som ges via input. Därefter flyttas Tetrominon i sin Move() metod som är tidsbaserad.
	*/

	//Vänster
	if (GetKeyState(0x25) & 0x8000) 
	{
		myCurrentTetromino->SetMovementDirection(Vector2D{ -1,0 });
		return;
	}
	//Höger
	if (GetKeyState(0x27) & 0x8000) 
	{
		myCurrentTetromino->SetMovementDirection(Vector2D{ 1,0 });
		return;
	}

	//Ner
	if (GetKeyState(0x28) & 0x8000) 
	{
		myCurrentTetromino->SetMovementDirection(Vector2D{ 0,1 });
		return;
	}
}

//Berättar för spelet att det krävs en ny Tetromino
void Game::NewTetromino() 
{
	myWantNewTetromino = true;
}

/*
Konverterar en 4x4 form som ges av en boolesk (endast 1:or & 0:or) int array
till en Vector2D array med 4 element.
*/
void Game::ConvertShapeToPosition(const int(&aShape)[16], Vector2D (&anOutputArray)[4]) 
{
	Vector2D index;
	int times = 0;
	for (int i = 0; i < 16; i++)
	{
		if (aShape[i] == 1) 
		{		
			anOutputArray[times] = Vector2D(index.GetX(), index.GetY());
			times++;
		}

		if (index.GetX() == 3) 
		{
			index += Vector2D(0, 1);
			index.SetX(0);
		}
		else
			index += Vector2D(1, 0);
	}
}

////////// De följande metoder är buffer metoder som bygger på Viktor Anderssons kod

//Skapar en buffer (credit till Viktor Andersson)
void Game::InitializeBuffer()
{
	COORD bufferSize{ gameData::SCREEN_WIDTH, gameData::SCREEN_HEIGHT };
	COORD bufferCoordinates{ 0, 0 };
	SMALL_RECT consoleRegion{ 0, 0, gameData::SCREEN_WIDTH - 1, gameData::SCREEN_HEIGHT - 1 };
	ReadConsoleOutput(myWindowHandle, reinterpret_cast<CHAR_INFO*>(myBuffer), bufferSize, bufferCoordinates, &consoleRegion);
}

//Rensar buffern (credit till Viktor Andersson)
void Game::ClearBuffer()
{
	for (int y{ 0 }; y < gameData::SCREEN_HEIGHT; ++y)
	{
		for (int x{ 0 }; x < gameData::SCREEN_WIDTH; ++x)
		{
			myBuffer[y][x].Attributes = 0;
			myBuffer[y][x].Char.AsciiChar = ' ';
		}
	}
}

//Ritar ut buffern (credit till Viktor Andersson)
void Game::DrawBuffer() {
	COORD bufferSize{ gameData::SCREEN_WIDTH, gameData::SCREEN_HEIGHT };
	COORD bufferCoordinates{ 0, 0 };
	SMALL_RECT consoleRegion{ 0, 0, gameData::SCREEN_WIDTH - 1, gameData::SCREEN_HEIGHT - 1 };
	WriteConsoleOutputW(myWindowHandle, reinterpret_cast<CHAR_INFO*>(myBuffer), bufferSize, bufferCoordinates, &consoleRegion);
}

//Sätter värdet av en position i buffern (credit till Viktor Andersson)
void Game::SetCharacter(const Vector2D& aPosition, const wchar_t& aCharacter, const gameData::Color& aColor)
{
	int x{ aPosition.GetX() }, y{ aPosition.GetY() };
	if (x < 0 || x >= gameData::SCREEN_WIDTH || y < 0 || y >= gameData::SCREEN_HEIGHT)
	{
		return;
	}
	CHAR_INFO& character{ myBuffer[y][x] };
	character.Char.UnicodeChar = aCharacter;
	character.Attributes = static_cast<int>(aColor);
}


