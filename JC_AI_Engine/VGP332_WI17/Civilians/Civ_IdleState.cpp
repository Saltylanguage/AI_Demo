#include "Civ_IdleState.h"
#include "Civ.h"


void IdleState::Enter(Civ & agent)
{

	agent.jobStatus = agent.Complete;
}

void IdleState::Update(Civ & agent, float deltaTime)
{
	X::DrawScreenText("Idle State", 700, 50, 16, X::Math::Vector4::Green());

	agent.PickState();
}

void IdleState::Exit(Civ & agent)
{
	agent.Civ_Path.clear();
	agent.Civ_ClosedList.clear();
	agent.Civ_ActualPath.clear();

}



//While in haul state:
//move to a resource
//mark the resource as pickedUp
//while it is picked up position is updated every frame to CivPosition + offset;
//mark the resource as !pickedUp
//increase the relevant stockpile resource counter by the value of the resource
//jobStatus == complete
