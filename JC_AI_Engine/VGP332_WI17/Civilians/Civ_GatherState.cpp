#include "Civ_GatherState.h"
#include "Civ.h"


void GatherState::Enter(Civ& agent)
{
	agent.jobStatus = agent.Incomplete;
}

void GatherState::Update(Civ& agent, float deltaTime)
{

	X::DrawScreenText("Gather State", 700, 50, 16, X::Math::Vector4::Green());

	agent.gatherTimer += deltaTime;

	if (agent.gatherTimer >= 0.02f)
	{
		agent.mWoodCounter++;
		agent.mGameWorld->mTrees.back().woodCounter--;

		if (agent.mGameWorld->mTrees.back().woodCounter <= 0)
		{
			agent.mGameWorld->mResources.push_back(Resource(Resource::ResourceType::Wood, agent.Civ_Position));
			agent.mGameWorld->mTrees.back().isChoppedDown = true;
		}

		agent.gatherTimer = 0;
	}

	if (agent.mWoodCounter >= 10)
	{
		agent.jobStatus = agent.Complete;
	}


	char woodCounterDisplay[50];
	sprintf_s(woodCounterDisplay, 50, "Wood Left: %d", agent.mGameWorld->mTrees.back().woodCounter);
	X::DrawScreenText(woodCounterDisplay, 700, 350, 16, X::Math::Vector4::Green());

	if (agent.jobStatus == agent.Complete || agent.jobStatus == agent.Cancelled)
	{
		//agent.mGameWorld->mResources.back().isPickedUp = true;
		//agent.Civ_TargetDestination = agent.mGameWorld->stockpileTiles.back() * 64;
		//agent.ChangeState(agent.Move);

		agent.PickState();
	}

}

void GatherState::Exit(Civ& agent)
{


	
}