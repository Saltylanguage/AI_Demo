#pragma once
#include "X\Inc\XEngine.h"
#include "AI_Library\AI_Library.h"



class Animal
{
public:

	Animal() : mStateMachine(*this) {}
	~Animal() {}

	X::Math::Vector2 mPosition;

	int mHealth;

	AI_Library::StateMachine<Animal> mStateMachine;
	AI_Library::Pathfinder mPathfinder;
	

};