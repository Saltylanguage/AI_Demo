#include "Civ_HaulState.h"
#include "Civ.h"


void HaulState::Enter(Civ& agent)
{
	agent.mGameWorld->mResources.back().isPickedUp = true;
	agent.Civ_TargetDestination = agent.mGameWorld->stockpileTiles.back() * 64;
	
	agent.jobStatus = agent.Complete;
}

void HaulState::Update(Civ& agent, float deltaTime)
{

	X::DrawScreenText("Haul State", 700, 50, 16, X::Math::Vector4::Green());

	if (agent.jobStatus == agent.Complete || agent.jobStatus == agent.Cancelled)
	{
		agent.PickState();
	}

}

void HaulState::Exit(Civ& agent)
{

}