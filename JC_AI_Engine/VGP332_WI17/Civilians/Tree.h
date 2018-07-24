#pragma once
#include "X\Inc\XEngine.h"

class Tree
{
public:

	Tree() {}
	~Tree() {}

	Tree(int texture, int value, X::Math::Vector2 startPos) : textureID(texture), woodCounter(value), mPosition(startPos) {}

	int textureID;
	int woodCounter;
	X::Math::Vector2 mPosition;

	bool isChoppedDown = false;

};