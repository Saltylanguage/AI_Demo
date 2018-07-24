#pragma once

#include "AI_Library\AI_Library.h"

class SCV;
class GatherState : public AI_Library::State<SCV>
{
public:
	virtual ~GatherState() {}

	void Enter(SCV& agent) override;
	void Update(SCV& agent, float deltaTime) override;
	void Exit(SCV& agent) override;


};