#pragma once
#include "Agent.h"

class SteeringBehavior
{
public:
	SteeringBehavior(const char* name)
		: mName(name)
	{}
	~SteeringBehavior() {};

	virtual X::Math::Vector2 Calculate(Agent& agent) = 0;

	const char * GetName() const { return mName; }

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }

protected:
	const char* mName;
	float mWeight;
	bool mActive;
};