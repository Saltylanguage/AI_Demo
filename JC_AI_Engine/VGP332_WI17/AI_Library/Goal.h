#pragma once

namespace AI_Library
{


	template <typename AgentType>
	class  Goal
	{
	public:

		enum class Status
		{
			Inactive, 
			Active,
			Completed,
			Failed
		};

		Goal(AgentType& agent) :
			mAgent(agent)
		 ,  mStatus(Status::Inactive)
		{}
		virtual ~Goal() {};

		// Function to activate this action
		virtual void Activate() = 0;

		// Function to update this action
		virtual Status Process() = 0;

		virtual void Teminate() = 0;

	protected:
		void ActivateIfInactive()
		{
			if (mStatus == Status::Inactive)
			{
				Activate();
			}
		}

		AgentType& mAgent;
		Status mStatus;

	};

//#include "Goal.inl"



}