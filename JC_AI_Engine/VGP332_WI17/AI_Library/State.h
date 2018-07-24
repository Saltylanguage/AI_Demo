#pragma once

namespace AI_Library
{

	template <typename AgentType>
	class State
	{
	public:
		virtual ~State(){}
		
		virtual void Enter(AgentType&) = 0;
		virtual void Update(AgentType&, float deltaTime) = 0;
		virtual void Exit(AgentType&) = 0;
	};

}


