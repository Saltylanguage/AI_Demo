//
//template<typename AgentType>
//DecisionModule<AgentType>::DecisionModule(AgentType& agent)
//	:mAgent(agent)
//	,mCurrentStrategy(nullptr)
//	,mCurrentGoal(nullptr)
//{}
//
//
//template<typename AgentType>
//DecisionModule<AgentType>::~DecisionModule()
//{
//	Purge();
//}
//
//template<typename AgentType>
//void DecisionModule<AgentType>::AddStategy(StrategyType* strategy)
//{
//	mStrategies.push_back(strategy);
//}
//
//template<typename AgentType>
//void DecisionModule<AgentType>::Purge()
//{
//	for (auto strategy : mStrategies)
//	{
//		X::SafeDelete(strategy);
//	}
//	mStrategies.clear();
//
//	if (mCUrrentGoal != nullptr)
//	{
//		mCurrentGoal->Terminate();
//		X::SafeDelete(mCurrentGoal);
//	}
//}
//
//
//template<typename AgentType>
//void DecisionModule<AgentType>::Update()
//{
//	Arbitrate();
//
//	if (mCurrent != nullptr)
//	{
//		mCurrentGoal->Process();
//	}
//}
//
//template<typename AgentType>
//void DecisionModule<AgentType>::Arbitrate()
//{
//	float mostDesirable = 0.0f;
//	StrategyType* bestStrategy = nullptr;
//
//	for (auto strategy : mStrategies)
//	{
//		float desirability = strategy->CalculateDesirability();
//		if (desirability > mostDesirable)
//		{
//			mostDesirable = desirability;
//			bestStrategy = strategy;
//		}
//	}
//
//	if (mCurrentStrategy != bestStrategy)
//	{
//		mCurrentStrategy = bestStrategy;
//
//
//		if (mCurrentGoal != nullptr)
//		{
//			mCurrentGoal->Terminate();
//			X::SafeDelete(mCurrentGoal);
//		}
//
//		if (murrentStrategy != nullptr)
//		{
//			mCurrentGoal = mCurrentStrategy->CreateGoal();
//		}
//	}
//
//}