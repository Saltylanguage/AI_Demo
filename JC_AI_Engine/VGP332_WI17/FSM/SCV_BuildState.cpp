#include "SCV_BuildState.h"
#include "SCV.h"

void BuildState::Enter(SCV & agent)
{
    agent.jobStatus = agent.Incomplete;

	//BuildingSite = FindBuildingSite();
}

void BuildState::Update(SCV & agent, float deltaTime)
{

    X::DrawScreenText("Build State", 700, 50, 16, X::Math::Vector4::Green());
	
	agent.gatherTimer += deltaTime;

	if (agent.gatherTimer >= 0.5f)
	{
		if (agent.refineryFrame < 5)
		{
			
			agent.refineryFrame++;
		}
		agent.gatherTimer = 0;
	}

	if (agent.refineryFrame >= 4)
	{
		agent.mMineralCounter = 0;
		agent.jobStatus = agent.Complete;
	}

	if (agent.jobStatus == agent.Complete)
	{
		agent.PickState();
	}

}

void BuildState::Exit(SCV & agent)
{
	agent.SCV_Path.clear();
	agent.SCV_ClosedList.clear();
	agent.SCV_ActualPath.clear();
}



