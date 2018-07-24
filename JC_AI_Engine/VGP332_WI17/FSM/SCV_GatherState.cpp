#include "SCV_GatherState.h"
#include "SCV.h"


void GatherState::Enter(SCV& agent)
{
	agent.jobStatus = agent.Incomplete;
}

void GatherState::Update(SCV& agent, float deltaTime)
{

	X::DrawScreenText("Gather State", 700, 50, 16, X::Math::Vector4::Green());

	agent.gatherTimer += deltaTime;

	if (agent.gatherTimer >= 0.2f)
	{
		agent.mMineralCounter++;
		agent.gatherTimer = 0;
	}


	if (agent.mMineralCounter >= 10)
	{
		agent.jobStatus = agent.Complete;
	}


	if (agent.jobStatus == agent.Complete || agent.jobStatus == agent.Cancelled)
	{
		agent.PickState();
	}

}

void GatherState::Exit(SCV& agent)
{

}