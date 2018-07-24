#pragma once
#include "X\Inc\XEngine.h"

class Bed
{
public:

	Bed(){}
	~Bed(){}

	Bed(X::Math::Vector2 startPos) :mPosition(startPos)
	{
		textureID = X::LoadTexture("CivImages/Bed.png");
	}

	int textureID;
	X::Math::Vector2 mPosition;

	bool isOccupied;

	void Initialize()
	{
		isOccupied = false;
	}

};