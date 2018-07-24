#pragma once
#include "X\Inc\XEngine.h"

class Resource
{
public:

	Resource(){}

	enum class ResourceType
	{
		Wood,
		Food,
		RawMeat
	};
	
	Resource(ResourceType type, X::Math::Vector2 startPos) : mType(type), mPosition(startPos), mValue(50)
	{
		SetResourceTexture();
	}

	ResourceType mType;

	X::Math::Vector2 mPosition;

	int resourceTextureID;
	int mValue;
	bool isPickedUp = false;

	void SetResourceType(ResourceType type)
	{
		mType = type;
	}

	void SetResourceTexture()
	{
		switch (mType)
		{
		case ResourceType::Wood:
			resourceTextureID = X::LoadTexture("CivImages/Wood.png");
			break;
		case ResourceType::Food:
			resourceTextureID = X::LoadTexture("CivImages/Food.png");
			break;
		case ResourceType::RawMeat:
			resourceTextureID = X::LoadTexture("CivImages/RawMeat.png");
			break;
		}
	}


	void Render()
	{
		X::DrawSprite(resourceTextureID, X::Math::Vector2(mPosition.x*64, mPosition.y * 64 ));
	}

	
};
