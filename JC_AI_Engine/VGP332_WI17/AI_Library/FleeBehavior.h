#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"
#include "Agent.h"



namespace AI_Library
{

	class FleeBehavior : public SteeringBehavior
	{
	public:
		FleeBehavior() : SteeringBehavior("Flee")
		{}

		X::Math::Vector2 Calculate(Agent& agent) override
		{
			//const float panicDistanceSq = 100.0f * 100.0f;
						
			/*if (agent.LengthSq(agent.GetPosition(), agent.GetDestination()) > panicDistanceSq)
			{
				return X::Math::Vector2(0.0f, 0.0f);
			}*/

			X::Math::Vector2 desiredVelocity;

			if (X::Math::Distance(agent.GetPosition(), agent.GetDestination()) > 5.0f)
			{
				desiredVelocity = X::Math::Normalize(agent.GetPosition() - agent.GetDestination()) *agent.GetMaxSpeed();				
			}		
			else
			{
				return X::Math::Vector2(0, 0);
			}

			char desiredVelocityDisplay[50];
			sprintf_s(desiredVelocityDisplay, "Desired Velocity: %f, %f", desiredVelocity.x, desiredVelocity.y);

			X::DrawScreenText(desiredVelocityDisplay, 700, 150, 16, X::Math::Vector4::Green());

			return desiredVelocity - agent.GetVelocity();
		}

	};
}

