#pragma once
#include "Goal.h"

#include <list>

namespace AI_Library
{

	template <typename AgentType>
	class GoalComposite : public Goal<AgentType>
	{

	public:
		GoalComposite(AgentType& agent);
		virtual ~GoalComposite();

	protected:

		void AddSubGoal(Goal<AgentType>* goal);
		void RemoveAllSubGoals();
		Status ProcessSubGoals;

		std::list<Goal<AgentType>*> mSubGoals;

	};

	/*#include "GoalComposite.inl"*/


	template <typename AgentType>
	GoalComposite<AgentType>::GoalComposite(AgentType& agent)
		: Goal<AgentType>(agent)
	{}
	
	template <typename AgentType>
	GoalComposite<AgentType>::~GoalComposite()
	{
		RemoveAllSubGoals();
	}
	
	template <typename AgentType>
	void GoalComposite<AgentType>::AddSubGoal(Goal<AgentType>* goal)
	{
		mSubGoals.push_front(goal);
	}
	
	template <typename AgentType>
	void GoalComposite<AgentType>::RemoveAllSubGoals()
	{
		for (auto goal : mSubGoals)
		{
			if (goal != nullptr)
			{
				goal->Terminate();
				X::SafeDelete(goal);
			}
		}
		mSubGoals.clear();
	}
	
	//template <typename AgentType>
	//GoalComposite<AgentType>::Status GoalComposite<AgentType>::ProcessSubGoals()
	//{
	//	while (!mSubGoals.empty())
	//	{
	//		Goal<AgentType>* goal = mSubGoals.front();
	//		if (goal->mStatus != Status::Completed &&
	//			goal->mStatus != Status::Failed)
	//		{
	//			break;
	//		}
	//		goal->Terminate();
	//		X::SafeDelete(goal);
	//		mSubGoals.pop_front();
	//	}
	//
	//	if (!mSubGoals.empty())
	//	{
	//		Status status = mSubGoals.front()->Process();
	//		if (status == Status::Completed && mSubGoals.size() > 1)
	//		{
	//			return Status::Active;
	//		}
	//		else
	//		{
	//			return status;
	//		}
	//
	//	}
	//
	//	return Status::Completed;
	//	
	//}
	
}