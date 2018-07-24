#pragma once
#include "X\Inc\XEngine.h"
#include "AI_Library\AI_Library.h"

class SCV : public AI_Library::Agent
{
public:

	SCV()
		: mSteeringModule(*this)
	{}

	void Initialize();
	void Update(float deltaTime); 
	void Render();
	void Terminate(); //TODO check if i need this

	int scvTextureIds[16];
	float angle;
	int frame;
	int cellSize = 64;

	AI_Library::SteeringModule mSteeringModule;

private:

};

