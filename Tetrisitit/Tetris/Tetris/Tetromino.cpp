#include "Tetromino.h"


Tetromino::Tetromino(Game* aGame,const std::vector<Vector2D>& aShape)
{
	myGame = aGame;
	myShape = aShape;
	//S�tter Tetrominons f�rg till en slumpm�ssig
	myColor = gameData::Color(std::rand() % 14 + 1);
	myPosition.SetX(gameData::SCREEN_WIDTH / 2);
	myPosition.SetY(gameData::SPAWN_Y);
}

//F�rklarar sig sj�lv
Tetromino::~Tetromino()
{
	myGame->RemoveTetromino(this);
}

//F�rklarar sig sj�lv
const Vector2D& Tetromino::GetPosition() 
{
	return myPosition;
}

//F�rklarar sig sj�lv
void Tetromino::SetMovementDirection(const Vector2D& aDirection) 
{
	myMovementDirection = aDirection;
}

//Kollar om alla Tetrominons block �r borta, i s� fall tas Tetrominon bort
void Tetromino::DestroyCheck()
{
	if (myShape.size() == 0) 
		delete this;
	
}

//Gravitation
void Tetromino::Gravity() 
{
	//Tajmas av en gravitations timer
	if (myGame->Timer(myGravityMaxTime, myGravityTime)) 
	{
		//Kollar om Tetrominon kommer kollidera ett block ner
		if (DoesntCollide(Vector2D(0, 1)))
			myGrounded = false;
		else
			myGrounded = true;

		//Verkst�ller gravitationen om den inte kolliderar ett block ner
		if (!myGrounded)
			myPosition += Vector2D(0, 1);

		//Kollar Tetrominon blev Grounded den h�r uppdateringen
		if (myGrounded == true && !myPreviousGrounded) 
		{
			//Avslutar spelet om Tetrominon �r grounded p� vid start Y (d�r Tetrominos skapas)
			if (myPosition.GetY() == gameData::SPAWN_Y) 
				myGame->GameOver();

			int height = Height();
		
			bool* rows = new bool[height];

			//Kollar Om n�gon av raderna som tetrominen fyller har blivit full
			for (int j = 0; j < height; j++)
			{
				rows[j] = false;
				for (int i = myGame->GetBoard()->GetStartX() + 1; i < myGame->GetBoard()->GetEndX(); i++)
				{
					if (!myGame->PositionIsTaken(Vector2D{ i,j + myPosition.GetY() })) 
						break;
					
					if (i == myGame->GetBoard()->GetEndX() - 1) 
						rows[j] = true;
				}
			}

			int rowsDestroyed = 0;
			//F�rst�r raderna om n�gon av dem eventuellt har blivit full
			for (int i = 0; i < height; i++)
			{
				if (rows[i] == true)
				{
					myGame->DestroyRow(i + myPosition.GetY());
					rowsDestroyed++;
				}
			}

			//Applicerar en score baserat p� hur m�nga rader som f�rst�redes
			if (rowsDestroyed == 1)
				myGame->AddScore(40);
			if (rowsDestroyed == 2)
				myGame->AddScore(100);
			if (rowsDestroyed == 3)
				myGame->AddScore(300);
			if (rowsDestroyed == 4)
				myGame->AddScore(1200);
		
			//Skapar en ny Tetromino om den nuvarande blir grundad
			//Minskar gravitationstimern (Detta g�r s� att blocken faller snabbare n�r en rad spr�ngs)
			if (myGame->IsCurrentTetromino(this)) 
			{
				myGravityMaxTime = 0.2f;
				myGame->NewTetromino();
			}
		}
		myPreviousGrounded = myGrounded;
	}
}

//F�rklarar sig sj�lv
std::vector<Vector2D>& Tetromino::GetShape() 
{
	return myShape;
}

//Tar bort ett block via dess index i myShape
void Tetromino::RemoveBrick(const int& anIndex) 
{
	myShape.erase(myShape.begin() + (anIndex));
}

//Roterar Tetrominon 90grader
void Tetromino::Rotate() 
{
	int offsetY = 0;
	int offsetX = 0;

	std::vector<Vector2D> shape = myShape;
	for (int i = 0; i < myShape.size(); i++)
	{
		if (myShape[i].GetX() == 1) 
			offsetY++;

		if (myShape[i].GetY() == 1)
			offsetX++;
	}

	//Kollar om Tetrominon kan rotera utan att kollidera med n�got (f�rutom sig sj�lv)
	for (int i = 0; i < myShape.size(); i++)
	{
		bool collidesWithSelf = false;
		shape[i] = Vector2D{ myShape[i].GetY() - (int)(offsetX / 2), -myShape[i].GetX() + (int)(offsetY / 2)};

		for (int j = 0; j < myShape.size(); j++)
			if (shape[i] == myShape[j]) 
				collidesWithSelf = true;

		if (!collidesWithSelf) 
			if (myGame->PositionIsTaken(myPosition + shape[i])) 
				return;
	}
	//Om alla krav �r uppfyllda roterars Tetrominon
	myShape = shape;
}

//Kollar om Tetrominon INTE kolliderar med n�got �t ett h�ll
bool Tetromino::DoesntCollide(const Vector2D& aDirection) 
{

	for (int i = 0; i < myShape.size(); i++)
	{
		Vector2D newPos = myPosition + aDirection + myShape[i];
		bool collidesWithSelf = false;

		for (int j = 0; j < myShape.size(); j++)
			if (newPos == (myPosition + myShape[j])) 
				collidesWithSelf = true;

		//Returnerar falsk om tetrominon kolliderar med annat �n sig sj�lv
		if (!collidesWithSelf) 
			if (myGame->PositionIsTaken(newPos)) 
				return false;
	}
	return true;
}

//Uppdaterings-loopen
void Tetromino::Update() 
{
	Gravity();
	Move();
}

//Returnerar Tetrominons h�jd
int Tetromino::Height() 
{
	int height = 0;
	for (int i = 0; i < myShape.size(); i++)
	{
		myShape[i];
		if (myShape[i].GetY() + 1 > height)
			height = myShape[i].GetY() + 1;
	}
	return height;
}

//F�rklarar sig sj�lv
void Tetromino::Draw() 
{
	for (int i = 0; i < myShape.size(); i++)
		myGame->SetCharacter(myPosition + myShape[i], gameData::BOX, myColor);
}

//Placerar Tetrominon p� marken
void Tetromino::Land() 
{
	while (DoesntCollide(Vector2D{ 0, 1 })) 
		myPosition += Vector2D{ 0, 1 };
}

//Flyttar Tetrominon via spelar-input
void Tetromino::Move() 
{
	if (myGame->Timer(myMaxMoveTime, myMoveTime)) 
	{
		if (DoesntCollide(myMovementDirection)) 
			myPosition += myMovementDirection;
		myMovementDirection = Vector2D{ 0,0 };
	}
}