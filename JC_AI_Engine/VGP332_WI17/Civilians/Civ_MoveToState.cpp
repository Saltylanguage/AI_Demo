#include "Civ_MoveToState.h"
#include "Civ.h"
#include <cstring>

void MoveToState::Enter(Civ & agent)
{	
	agent.Civ_Path = agent.mPathfinder.FindPath
		(agent.mPathfinder.searchMode, agent.mPathfinder.mGraph,
			AI_Library::Coord((int)agent.Civ_Position.x / (int)agent.cellSize, (int)agent.Civ_Position.y / (int)agent.cellSize),
			AI_Library::Coord((int)agent.Civ_TargetDestination.x / (int)agent.cellSize, (int)agent.Civ_TargetDestination.y / (int)agent.cellSize));
	agent.initPath = true;
	agent.jobStatus = agent.Incomplete;
}

void MoveToState::Update(Civ & agent, float deltaTime)
{
	
	char buffer2[100];
	char buffer3[100];

	
	sprintf_s(buffer2, 100, "Position: %f, %f", agent.Civ_Position.x, agent.Civ_Position.y);
	sprintf_s(buffer3, 100, "Target Destination: %f, %f", agent.Civ_TargetDestination.x, agent.Civ_TargetDestination.y);

	X::DrawScreenText("Move State", 700, 50, 16, X::Math::Vector4::Green());
	
	
	X::DrawScreenText(buffer2, 700, 200, 16, X::Math::Vector4::Green());
	X::DrawScreenText(buffer3, 700, 250, 16, X::Math::Vector4::Green());

	if (X::Math::Distance(agent.Civ_Position, agent.Civ_TargetDestination) > 10.0f)
	{
		agent.MoveTo(deltaTime);
	}
	else
	{
		agent.jobStatus = agent.Complete;
	}

	//TODO find out why its not exiting move state

	if (agent.jobStatus == agent.Complete)
	{
		agent.PickState();
	}
}


void MoveToState::Exit(Civ & agent)
{
	agent.Civ_Path.clear();
	agent.Civ_ClosedList.clear();
	agent.Civ_ActualPath.clear();

	if (!agent.mGameWorld->mResources.empty())
	{
		if (agent.mGameWorld->mResources.back().isPickedUp)
		{
			agent.mGameWorld->mResources.back().isPickedUp = false;
			agent.mGameWorld->woodCounter += agent.mGameWorld->mResources.back().mValue;
		}
	}

}

//