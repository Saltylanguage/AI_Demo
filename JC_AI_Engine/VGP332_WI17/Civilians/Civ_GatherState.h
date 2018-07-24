#pragma once

#include "AI_Library\AI_Library.h"

class Civ;
class GatherState : public AI_Library::State<Civ>
{
public:
	virtual ~GatherState() {}

	void Enter(Civ& agent) override;
	void Update(Civ& agent, float deltaTime) override;
	void Exit(Civ& agent) override;
};