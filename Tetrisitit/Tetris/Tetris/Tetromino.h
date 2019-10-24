#ifndef TERMINO_HEADER
#define TERMINO_HEADER
#include "Game.h"
#include <string>
#include "Vector2D.h"


class Tetromino
{
public:
	Tetromino(Game* aGame,const std::vector<Vector2D>& aShape);
	~Tetromino();
	const Vector2D& GetPosition();	
	void Draw();
	void Update();
	void SetMovementDirection(const Vector2D& aDirection);
	void Rotate();
	void RemoveBrick(const int& anIndex);
	void DestroyCheck();
	void Land();
	std::vector<Vector2D>& GetShape();
private:
	gameData::Color myColor;
	int Height();
	void Gravity();
	bool DoesntCollide(const Vector2D& aDirection);
	bool myGrounded;
	bool myPreviousGrounded;
	Game* myGame;
	void Move();
	float myGravityTime, myMoveTime;
	float myGravityMaxTime = 0.5f, myMaxMoveTime = 0.15f;
	std::vector<Vector2D> myShape;
	Vector2D myPosition;
	Vector2D myMovementDirection;
};
#endif
