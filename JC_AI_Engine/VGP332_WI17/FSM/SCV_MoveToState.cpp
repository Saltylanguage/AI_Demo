#include "SCV_MoveToState.h"
#include "SCV.h"
#include <cstring>

void MoveToState::Enter(SCV & agent)
{
	
	agent.SCV_Path = agent.mPathfinder.FindPath
		(agent.mPathfinder.searchMode, agent.mPathfinder.mGraph,
			AI_Library::Coord((int)agent.SCV_Position.x / (int)agent.cellSize, (int)agent.SCV_Position.y / (int)agent.cellSize),
			AI_Library::Coord((int)agent.SCV_TargetDestination.x / (int)agent.cellSize, (int)agent.SCV_TargetDestination.y / (int)agent.cellSize));
	agent.initPath = true;
	agent.jobStatus = agent.Incomplete;
}

void MoveToState::Update(SCV & agent, float deltaTime)
{
	char buffer[50];
	char buffer2[50];
	sprintf_s(buffer, "Destination: %f, %f", agent.SCV_NextDestination.x, agent.SCV_NextDestination.y);
	sprintf_s(buffer2, "Position: %f, %f", agent.SCV_Position.x, agent.SCV_Position.y);
	X::DrawScreenText("Move State", 700, 50, 16, X::Math::Vector4::Green());
	X::DrawScreenText(buffer, 700, 100, 16, X::Math::Vector4::Green());
	X::DrawScreenText(buffer2, 700, 200, 16, X::Math::Vector4::Green());

	if (X::Math::Distance(agent.SCV_Position, agent.SCV_NextDestination) > 5.0f)
	{
		agent.MoveTo(deltaTime);
	}
	else
	{
		//agent.SCV_Position = agent.SCV_TargetDestination;
		agent.jobStatus = agent.Complete;
	}

	//TODO find out why its not exiting move state

	if (agent.jobStatus == agent.Complete)
	{
		agent.PickState();
	}
}


void MoveToState::Exit(SCV & agent)
{
	agent.SCV_Path.clear();
	agent.SCV_ClosedList.clear();
	agent.SCV_ActualPath.clear();

	//agent.PickState();

}

//