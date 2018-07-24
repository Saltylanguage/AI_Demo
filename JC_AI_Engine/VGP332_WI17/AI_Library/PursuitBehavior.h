#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"
#include "Agent.h"

namespace AI_Library
{

	class PursuitBehavior : public SteeringBehavior
	{
	public:
		PursuitBehavior() : SteeringBehavior("Pursuit")
		{}

		X::Math::Vector2 Calculate(Agent& agent) override
		{
			X::Math::Vector2 predictedPosition;

			X::Math::Vector2 desiredVelocity = agent.GetPosition() - agent.GetDestination() *agent.GetMaxSpeed();
			desiredVelocity = X::Math::Normalize(desiredVelocity);

			return desiredVelocity - agent.GetVelocity();
		}

	};
}

