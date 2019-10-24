#ifndef VECTOR2D_HEADER
#define VECTOR2D_HEADER


class Vector2D
{
public:
	Vector2D(const int& anX,const int& anY);
	Vector2D();
	~Vector2D() = default;
	const int& GetX() const { return myX; }
	const int& GetY() const { return myY; }
	
	const void SetX(const int& anX);
	const void SetY(const int& anY);

	Vector2D operator+(const Vector2D& aVector) const;
	Vector2D operator-(const Vector2D& aVector) const;
	Vector2D& operator = (const Vector2D& aVector);
	Vector2D& operator += (const Vector2D& aVector);
	bool operator== (const Vector2D& aVector) const;
private:
	int myX;
	int myY;
};

#endif