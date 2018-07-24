#pragma once
#include "X\Inc\XEngine.h"
#include "SteeringBehavior.h"
#include "Agent.h"

namespace AI_Library
{

	class WanderBehavior : public SteeringBehavior
	{
	public:
		WanderBehavior() : SteeringBehavior("Wander")
		{}

		X::Math::Vector2 wanderVector = X::Math::Vector2(0, 1);

		float wanderDistance = 128;
		float angleChange = 1;
		float radius = 64;

		X::Math::Vector2 Calculate(Agent& agent) override
		{
			float randX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //- 0.5f;
			float randY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //- 0.5f;

			X::Math::Vector2 randVector = X::Math::Vector2(randX, randY);
			wanderVector += randVector;
			wanderVector.Normalize();

			X::Math::Vector2 displacementVector = wanderVector * radius;
			displacementVector.y += radius;

			X::Math::Matrix3 transformMatrix = agent.LocalToWorldTransform();

			X::Math::Circle circle;
			circle.radius = 64;
			circle.center = agent.GetPosition() + X::Math::Vector2(32, 32);

			X::Math::Vector2 desiredVelocity = (X::Math::TransformCoord(displacementVector, transformMatrix) - (agent.GetPosition() + X::Math::Vector2(32, 32)));
			desiredVelocity.Normalize();
			desiredVelocity *= agent.GetMaxSpeed();

			X::Math::Vector2 circlePos = X::Math::TransformCoord(X::Math::Vector2(0, radius), transformMatrix);

			X::Math::Vector2 lineStart = X::Math::TransformCoord(displacementVector, transformMatrix);
			
			char circleInfo[150];
			sprintf_s(circleInfo, 150, "Circle Center: %f, %f Circle Radius %f", circle.center.x, circle.center.y, circle.radius);
			X::DrawScreenCircle(circlePos, circle.radius, X::Math::Vector4::Red());
			X::DrawScreenLine(lineStart, agent.GetPosition() + X::Math::Vector2(32, 32), X::Math::Vector4::Blue());

			X::DrawScreenText(circleInfo, 700, 200, 16, X::Math::Vector4::Green());

			return desiredVelocity - agent.GetVelocity();
		}

	};
}