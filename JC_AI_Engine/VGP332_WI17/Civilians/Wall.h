#pragma once
#include "X\Inc\XEngine.h"

class Wall
{
public:
	
	Wall() {}
	~Wall() {}

	Wall(X::Math::Vector2 startPos) : mPosition(startPos)
	{
		textureID = X::LoadTexture("CivImages/WoodWall.png");
	}

	int textureID;

	X::Math::Vector2 mPosition;

	void Initialize(X::Math::Vector2 startPos)
	{
	
		mPosition = startPos;
	}

	void Render()
	{
		X::DrawSprite(textureID, mPosition);
	}


};
