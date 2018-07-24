#pragma once
#include "AI_Library\AI_Library.h"

class SCV;
class IdleState : public AI_Library::State<SCV>
{
public:
	virtual ~IdleState() {}
	
	
	void Enter(SCV& agent) override;
	void Update(SCV& agent, float deltaTime) override;
	void Exit(SCV& agent) override;



 };