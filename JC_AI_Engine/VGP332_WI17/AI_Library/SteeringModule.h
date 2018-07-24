#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"

class Agent;

namespace AI_Library
{

	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent) : mAgent(agent) {}
		~SteeringModule() {};

		void AddBehavior(SteeringBehavior* behavior);
		SteeringBehavior* GetBehavior(const char* name);

		void ToggleBehavior(SteeringBehavior* behavior);

		void Purge();

		X::Math::Vector2 Calculate();
		std::vector<SteeringBehavior*> mBehaviors;

	private:
		Agent& mAgent;

		//SteeringBehavior* mCurrentBehavior;

	};
}