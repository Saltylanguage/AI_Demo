#pragma once

#include "AI_Library\AI_Library.h"

class SCV;

//TODO move states into CPP
class BuildState : public AI_Library::State<SCV>
{
public:
	virtual ~BuildState() {}
	
	void Enter(SCV& agent) override;
	void Update(SCV& agent, float deltaTime) override;
	void Exit(SCV& agent) override;

};