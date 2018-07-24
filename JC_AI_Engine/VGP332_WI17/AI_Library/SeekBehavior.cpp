#pragma once
#include "SeekBehavior.h"


X::Math::Vector2 AI_Library::SeekBehavior::Calculate(Agent & agent)
{
	X::Math::Vector2 desiredVelocity = X::Math::Normalize(X::Math::Vector2((agent.mDestination - agent.mPosition).x, (agent.mDestination - agent.mPosition).y));

	return desiredVelocity - X::Math::Vector2(agent.mPosition.x, agent.mPosition.y);
}
           