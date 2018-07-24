#pragma once


class Floor
{
public:

	Floor() {}
	~Floor() {}

	Floor(X::Math::Vector2 startPos) : mPosition(startPos)
	{
		textureID = X::LoadTexture("CivImages/WoodFloor.png");
	}

	int textureID;

	X::Math::Vector2 mPosition;

	void Initialize(X::Math::Vector2 startPos)
	{
				
	}

	void Render()
	{
		X::DrawSprite(textureID, mPosition);
	}


};