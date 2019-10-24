#ifndef GAME_HEADER
#define GAME_HEADER
#include "GameData.h"
#include <string>
#include "Board.h"
#include "Vector2D.h"
#include <vector>
#include <Windows.h>

class Tetromino;


class Game
{
public:
	Game();
	~Game() = default;

	void SetCharacter(const Vector2D& aPosition, const wchar_t&	aCharacter, const gameData::Color& aColor);
	bool Timer(const float& someMaxTime, float& someCurrentTime);
	bool PositionIsTaken(const Vector2D& aPosition);
	void NewTetromino();
	Board* GetBoard();
	void DestroyRow(const int& aY);
	void RemoveTetromino(const Tetromino* aTetromino);
	bool IsCurrentTetromino(const Tetromino* aTetromino);
	void GameOver();
	void AddScore(const int& anAmount);
	void Run();
private:	
	int myScore;
	bool myGameOver;
	bool myInputIsBeingPressed;
	bool myJustLanded;
	bool myJustRotated;
	HANDLE myWindowHandle;
	void InitializeBuffer();
	void ClearBuffer();
	void DrawBuffer();
	void SetWindowSize(const int& aWidth, const int& aHeight);
	void SetFontSize(const int& aWidth, const int& aHeight);
	CHAR_INFO myBuffer[gameData::SCREEN_HEIGHT][gameData::SCREEN_WIDTH];
	std::vector<Tetromino*> myTetrominos;
	Tetromino* myCurrentTetromino;
	Board* myBoard;
	float myDeltaTime;
	void Input();
	bool myWantNewTetromino;
	void ConvertShapeToPosition(const int(&aShape)[16], Vector2D(&anOutputArray)[4]);

	//Tetromino former. I efterhand inser jag att detta var ett ganska dumt/lat sätt att skapa dem på men, meh.
	int shapes[7][16]
	{
	{1,1,1,1,
	 0,0,0,0,
	 0,0,0,0,
	 0,0,0,0},

	{1,0,0,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0},

	{0,0,1,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0},

	{1,1,0,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0},

	{0,1,1,0,
	 1,1,0,0,
	 0,0,0,0,
	 0,0,0,0},

	{0,1,0,0,
	 1,1,1,0,
	 0,0,0,0,
	 0,0,0,0},

	{1,1,0,0,
	 0,1,1,0,
	 0,0,0,0,
	 0,0,0,0},
	};
};
#endif 
