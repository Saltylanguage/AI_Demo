#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"
#include "Agent.h"

namespace AI_Library
{

	class SeekBehavior : public SteeringBehavior
	{
	public:
		SeekBehavior() : SteeringBehavior("Seek")
		{}

		X::Math::Vector2 Calculate(Agent& agent) override
		{

			X::Math::Vector2 desiredVelocity(0.0f, 0.0f);
			//X::Math::Vector2 destination = agent.GetDestination();

			//desiredVelocity = X::Math::Normalize(destination - agent.GetPosition()) * agent.GetMaxSpeed();

			if (X::Math::DistanceSqr(agent.GetPosition(), agent.GetDestination()) > 2.0f)
			{
				X::Math::Vector2 destination = agent.GetDestination();
				desiredVelocity = X::Math::Normalize(destination - agent.GetPosition()) * agent.GetMaxSpeed();
			}
			else
			{
				return X::Math::Vector2(0.0f, 0.0f);
			}
		
			char desiredVelocityDisplay[50];
			sprintf_s(desiredVelocityDisplay, "Desired Velocity: %f, %f", desiredVelocity.x, desiredVelocity.y);

			X::DrawScreenText(desiredVelocityDisplay, 700, 150, 16, X::Math::Vector4::Green());

			return desiredVelocity - agent.GetVelocity();

		}

	};
}

