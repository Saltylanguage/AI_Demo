#include "Civ_BuildState.h"
#include "Civ.h"

void BuildState::Enter(Civ & agent)
{
	agent.mGameWorld->mResources.back().isPickedUp = false;
	agent.jobStatus = agent.Incomplete;
}

void BuildState::Update(Civ & agent, float deltaTime)
{

	X::DrawScreenText("Build State", 700, 50, 16, X::Math::Vector4::Green());

	agent.gatherTimer += deltaTime;

	if (agent.gatherTimer >= 1.0f)
	{
		agent.mGameWorld->mWalls.push_back(Wall(agent.mGameWorld->mWallBuildSites.back() / 64));

		AI_Library::Coord tempCoord = AI_Library::Coord(agent.mGameWorld->mWalls.back().mPosition.x, agent.mGameWorld->mWalls.back().mPosition.y);	
		agent.mGameWorld->mGraph->SetImpassable(tempCoord);

		agent.mGameWorld->mWallBuildSites.pop_back();
		agent.mGameWorld->mResources.pop_back();

		if (!agent.mGameWorld->mResources.empty())
		{
			agent.Civ_TargetDestination = agent.mGameWorld->mResources.back().mPosition;
		}
		agent.jobStatus = agent.Complete;
		agent.gatherTimer = 0;
	}

	if (agent.jobStatus == agent.Complete)
	{
		agent.PickState();
	}

}

void BuildState::Exit(Civ & agent)
{
	agent.Civ_Path.clear();
	agent.Civ_ClosedList.clear();
	agent.Civ_ActualPath.clear();
}



