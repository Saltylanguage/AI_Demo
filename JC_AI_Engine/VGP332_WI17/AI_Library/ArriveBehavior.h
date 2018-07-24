#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"
#include "Agent.h"

namespace AI_Library
{

	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	class ArriveBehavior : public SteeringBehavior
	{
	public:
		ArriveBehavior() : SteeringBehavior("Arrive")
		{}

		Deceleration deceleration;
		void SetDeceleration(Deceleration d)
		{
			if (d >= 0 && d <= 2)
			{
				deceleration = d;
			}
			else
			{
				deceleration = normal;
			}
		}
		Deceleration GetDeceleration() { return deceleration; }

		X::Math::Vector2 Calculate(Agent& agent) override
		{
		
			X::Math::Vector2 destination = agent.GetDestination();

			float distance = destination.Length();

			if (distance > 2.0f)
			{
				const float decelerationFactor = 0.3f;
				float speed = distance / deceleration * (decelerationFactor);

				speed = X::Math::Min((float)speed, agent.GetMaxSpeed());

				X::Math::Vector2 desiredVelocity = destination * agent.GetMaxSpeed() / distance;

				return desiredVelocity - agent.GetVelocity();
			}
			return X::Math::Vector2::Zero();

		}

	};
}

