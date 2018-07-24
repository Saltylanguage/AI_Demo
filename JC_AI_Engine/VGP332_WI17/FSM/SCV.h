#pragma once

#include "SCV_BuildState.h"
#include "SCV_GatherState.h"
#include "SCV_IdleState.h"
#include "SCV_MoveToState.h"
#include "AI_Library\AI_Library.h"
#include <X\Inc\XEngine.h>
#include <list>

class SCV
{
public:

	enum State
	{
		Idle,
		Move,
		Gather,
		Build,
		Return
	};

    enum JobStatus
    {
        Incomplete = 0,
        Complete,
        Cancelled
    };

	SCV() : mStateMachine(*this) {}

	std::vector<AI_Library::Coord> SCV_OpenList;
	std::vector<AI_Library::Coord> SCV_ClosedList;

	std::vector<AI_Library::Coord> SCV_Path;
	std::list<X::Math::Vector2> SCV_ActualPath;

	X::Math::Vector2 SCV_Position;
    X::Math::Vector2 SCV_NextDestination;
	X::Math::Vector2 SCV_TargetDestination;

	float cellSize = 64;
	bool initPath = true;

	int frame = 0;
	float angle = 0.0f;
	int spriteTextureIDs[16];

	int refineryFrame = 0;
	int storageTextureId[5];

    JobStatus jobStatus = Incomplete;

	AI_Library::StateMachine<SCV> mStateMachine;
	AI_Library::Pathfinder mPathfinder;
	
	std::vector<X::Math::Vector2> MineralLocations;
	std::vector<X::Math::Vector2> BuildLocations;
	std::vector<X::Math::Vector2> StorageLocations;

	int mMineralCounter;
	int mBuildCounter;
	int mBuildTime;

	float gatherTimer;
	float buildTimer;

	X::Math::Vector2 FindMinerals();
	X::Math::Vector2 FindBuildingSite();
	X::Math::Vector2 FindStorage();

	void Initialize();
	void InitializePath();

	void Update(float deltaTime);
	void Render();
	
	void SetPath(AI_Library::Graph& g);

	void MoveTo(float deltaTime);

	void GatherMinerals(float deltaTime);
	void ReturnMinerals(float deltaTime);
	void ConstructWall(float deltaTime);
		
	void ChangeState(State state);

	void PickState();

};


//class IdleState : AI_Library::State<Mario>
//{
//public:
//	virtual ~IdleState() {}
//	void Enter(Mario& agent) override
//	{
//
//	}
//
//	void Update(Mario& agent, float deltaTime) override
//	{
//
//	}
//
//	void Exit(Mario& agent) override
//	{
//
//	}
//
// };
//
//class Mario
//{
//public:
//
//	enum State
//	{
//		Idle,
//		Run,
//		Jump,
//		Kill,
//		Etc
//	};
//
//	Mario()
//		: mStateMachine(*this)
//	{}
//
//	void Initialize()
//	{
//		mStateMachine.AddState(new IdleState());
//		mStateMachine.AddState(.......);
//		.....
//		mStateMachine.ChangeState(/*Your starting state*/);
//	}
//
//	void Update(float deltaTime)
//	{
//		mStateMachine.Update(deltaTime);
//	}
//
//	void ChangeState(State state)
//	{
//		mStateMachine.ChangeState((uint32_t)state)
//	}
//
//	AI_Library::StateMachine<Mario> mStateMachine;
//};