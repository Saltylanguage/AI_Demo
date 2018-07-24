#pragma once
#include "Goal.h"
#include "GoalComposite.h"
#include "Strategy.h"

namespace AI_Library
{

	template<typename AgentType>
	class DecisionModule
	{
		typedef Goal<AgentType> GoalType;
		typedef Strategy<AgentType> StrategyType;

		DecisionModule(AgentType& agent);
		~DecisionModule();

		void AddStrategy(StrategyType* strategy);
		void Purge();

		void Update();

	private:
		void Arbitrate();

		AgentType& mAgent;
		std::vector<StrategyType*> mStrategies;
		StrategyType* mCurrentStrategy;
		GoalType* mCurrentGoal;
	};


//#include "DecisionModule.inl"

	
	template<typename AgentType>
	DecisionModule<AgentType>::DecisionModule(AgentType& agent)
		:mAgent(agent)
		,mCurrentStrategy(nullptr)
		,mCurrentGoal(nullptr)
	{}
	
	
	template<typename AgentType>
	DecisionModule<AgentType>::~DecisionModule()
	{
		Purge();
	}
	
	template<typename AgentType>
	void DecisionModule<AgentType>::AddStrategy(StrategyType* strategy)
	{
		mStrategies.push_back(strategy);
	}
	
	template<typename AgentType>
	void DecisionModule<AgentType>::Purge()
	{
		for (auto strategy : mStrategies)
		{
			X::SafeDelete(strategy);
		}
		mStrategies.clear();
	
		if (mCUrrentGoal != nullptr)
		{
			mCurrentGoal->Terminate();
			X::SafeDelete(mCurrentGoal);
		}
	}
	
	
	template<typename AgentType>
	void DecisionModule<AgentType>::Update()
	{
		Arbitrate();
	
		if (mCurrent != nullptr)
		{
			mCurrentGoal->Process();
		}
	}
	
	template<typename AgentType>
	void DecisionModule<AgentType>::Arbitrate()
	{
		float mostDesirable = 0.0f;
		StrategyType* bestStrategy = nullptr;
	
		for (auto strategy : mStrategies)
		{
			float desirability = strategy->CalculateDesirability();
			if (desirability > mostDesirable)
			{
				mostDesirable = desirability;
				bestStrategy = strategy;
			}
		}
	
		if (mCurrentStrategy != bestStrategy)
		{
			mCurrentStrategy = bestStrategy;
	
	
			if (mCurrentGoal != nullptr)
			{
				mCurrentGoal->Terminate();
				X::SafeDelete(mCurrentGoal);
			}
	
			if (murrentStrategy != nullptr)
			{
				mCurrentGoal = mCurrentStrategy->CreateGoal();
			}
		}
	
	}

}