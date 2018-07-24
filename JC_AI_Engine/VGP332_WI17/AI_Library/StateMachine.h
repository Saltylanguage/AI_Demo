#pragma once
#include "State.h"
#include <vector>
#include "X\Inc\XEngine.h"


namespace AI_Library
{
	template<typename AgentType>
	class StateMachine
	{

	private:
		typedef State<AgentType> StateType;
		AgentType& mAgent;
		StateType* mCurrentState;
		std::vector<StateType*> mStates;

	public:
		StateMachine(AgentType& agent);
		~StateMachine();
			int AddState(StateType* newState);
			void Purge();
			void Update(float deltaTime);
			void ChangeState(uint32_t index);
			void Exit();
	};

		template<typename AgentType>
		StateMachine<AgentType>::StateMachine(AgentType& agent) : mAgent(agent), mCurrentState(nullptr) 
		{
			
		}

		template<typename AgentType>
		StateMachine<AgentType>::~StateMachine() 
		{			
			XASSERT(mStates.empty(), "[StateMachine], Purge must be called before destruction");
			
		}

		template<typename AgentType>
		int StateMachine<AgentType>::AddState(StateType* newState)
		{
			XASSERT(newState != nullptr, "[StateMachine], newState cannot be null");
			mStates.push_back(newState);
			return (int)mStates.size() - 1;			
		}
		template<typename AgentType>
		void StateMachine<AgentType>::Purge()
		{
			for (auto state : mStates)
			{
				X::SafeDelete(state);
			}
			mStates.clear();
		}

		template<typename AgentType>
		void StateMachine<AgentType>::Update(float deltaTime)
		{
			XASSERT(mCurrentState != nullptr, "[StateMachine], mCurrentState is null");					
			mCurrentState->Update(mAgent, deltaTime);			
		}
		
		template<typename AgentType>
		void StateMachine<AgentType>::ChangeState(uint32_t index)
		{			
			XASSERT(index < (uint32_t)mStates.size(), "[StateMachine] Invalid index %d.", index);
			if (mCurrentState)
			{
				mCurrentState->Exit(mAgent);
			}
			mCurrentState = mStates[index];
			mCurrentState->Enter(mAgent);			
		}

		template<typename AgentType>
		void StateMachine<AgentType>::Exit()
		{

		}

}
	


//For Homework:
// Design a demo that uses the state machine/ state classes
// Your Demo Needs to have an agent wtih at least 5 states.
// You will need to create a class taht uses the state machine.
//
//example:
//

// Idle
// MoveToTarget
// Gather
// Build
// 