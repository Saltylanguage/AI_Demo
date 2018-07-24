#pragma once

#include "AI_Library\AI_Library.h"

class Civ;

//TODO move states into CPP
class SleepState : public AI_Library::State<Civ>
{
public:
	virtual ~SleepState() {}

	void Enter(Civ& agent) override;
	void Update(Civ& agent, float deltaTime) override;
	void Exit(Civ& agent) override;

};