#include "SCV_IdleState.h"
#include "SCV.h"


void IdleState::Enter(SCV & agent)
{
    //Do nothing
    agent.jobStatus = agent.Complete;
}

void IdleState::Update(SCV & agent, float deltaTime)
{
    //Do nothing OR pick a new state?

    //int counter = 0;
    //while (counter <= 100)
    //{
    //	counter++;
    //}
    //agent.mStateMachine.ChangeState(agent.Move);
    //counter = 0;    
    X::DrawScreenText("Idle State", 700, 50, 16, X::Math::Vector4::Green());
    if (agent.jobStatus == agent.Complete || agent.jobStatus == agent.Cancelled)
    {
        agent.PickState();
    }
}

void IdleState::Exit(SCV & agent)
{
    agent.SCV_Path.clear();
    agent.SCV_ClosedList.clear();
    agent.SCV_ActualPath.clear();

    //agent.PickState();
}
