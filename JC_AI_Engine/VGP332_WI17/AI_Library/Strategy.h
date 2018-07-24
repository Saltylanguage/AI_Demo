#pragma once
#include "Goal.h"

namespace AI_Library
{

	template<typename AgentType>
	class Strategy
	{
	public:



		Strategy(AgentType& agent) : mAgent(agent) {}
		virtual ~Strategy() {}

		virtual float CalculateDesirability() = 0;
		virtual Goal<AgentType>* CreateGoal() = 0;

	protected:
		AgentType& mAGent;

	};

}