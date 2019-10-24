#include "Vector2D.h"


Vector2D::Vector2D(const int& anX, const int& anY)
{
	myX = anX;
	myY = anY;
}

Vector2D::Vector2D()
{
	myX = 0;
	myY = 0;
}

//Förklarar sig själv
const void Vector2D::SetX(const int& anX) 
{
	myX = anX;
}

//Förklarar sig själv
const void Vector2D::SetY(const int& anY) 
{
	myY = anY;
}

// + operator
Vector2D Vector2D::operator+(const Vector2D& aVector) const 
{
	return { myX + aVector.myX, myY + aVector.myY };
}

// - operator
Vector2D Vector2D::operator-(const Vector2D& aVector) const
{
	return { myX - aVector.myX, myY - aVector.myY };
}

// = operator
Vector2D& Vector2D::operator=(const Vector2D& aVector) 
{
	myX = aVector.myX;
	myY = aVector.myY;
	return *this;
}

// += operator
Vector2D& Vector2D::operator+=(const Vector2D& aVector) 
{
	myX = aVector.myX + myX;
	myY = aVector.myY + myY;
	return *this;
}

// == operator
bool Vector2D::operator==(const Vector2D& aVector) const 
{
	return(myX == aVector.myX && myY == aVector.myY);
}