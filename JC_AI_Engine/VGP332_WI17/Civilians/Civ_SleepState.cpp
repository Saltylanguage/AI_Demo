#include "Civ_SleepState.h"
#include "Civ.h"


void SleepState::Enter(Civ& agent)
{
	/*agent.mGameWorld->createFloor = true;*/
	agent.mGameWorld->isAsleep = true;
	agent.Civ_Position = agent.mGameWorld->mBeds.back().mPosition + X::Math::Vector2(16,26);
	agent.Civ_TargetDestination = X::Math::Vector2(0, 1000);
	agent.jobStatus = agent.Incomplete;
}

void SleepState::Update(Civ& agent, float deltaTime)
{

	X::DrawScreenText("Sleep State", 700, 50, 16, X::Math::Vector4::Green());

	if (agent.jobStatus == agent.Complete || agent.jobStatus == agent.Cancelled)
	{
		agent.PickState();
	}

}

void SleepState::Exit(Civ& agent)
{

}