#pragma once

#include "Civ_BuildState.h"
#include "Civ_GatherState.h"
#include "Civ_IdleState.h"
#include "Civ_MoveToState.h"
#include "Civ_HaulState.h"
#include "Civ_SleepState.h"
#include "AI_Library\AI_Library.h"
#include <X\Inc\XEngine.h>
#include <list>

#include "GameWorld.h"


class Civ
{
public:

	enum State
	{
		Idle,
		Move,
		Gather,
		Build,
		Haul,
		Sleep
	};

    enum JobStatus
    {
        Incomplete = 0,
        Complete,
        Cancelled
    };

	Civ() : mStateMachine(*this) {}


	GameWorld* mGameWorld;

	std::vector<AI_Library::Coord> Civ_OpenList;
	std::vector<AI_Library::Coord> Civ_ClosedList;

	std::vector<AI_Library::Coord> Civ_Path;
	std::list<X::Math::Vector2> Civ_ActualPath;

	X::Math::Vector2 Civ_Position;    
	X::Math::Vector2 Civ_TargetDestination;

	float cellSize = 64;
	bool initPath = true;

	int frame = 0;
	float angle = 0.0f;
	int spriteTextureIDs[16];

	int refineryFrame = 0;
	int storageTextureId[5];

    JobStatus jobStatus = Incomplete;

	AI_Library::StateMachine<Civ> mStateMachine;
	AI_Library::Pathfinder mPathfinder;
	
	std::vector<X::Math::Vector2> treeLocations;
	std::vector<X::Math::Vector2> BuildLocations;
	std::vector<X::Math::Vector2> StorageLocations;

	int mWoodCounter;
	int mBuildCounter;
	int mBuildTime;

	float gatherTimer;
	float buildTimer;

	X::Math::Vector2 FindTree();
	X::Math::Vector2 FindBuildingSite();
	X::Math::Vector2 FindStorage();

	void Initialize(GameWorld* world);

	void Update(float deltaTime);
	void Render();

	void MoveTo(float deltaTime);

	void GatherWood(float deltaTime);
	void ReturnWood(float deltaTime);
	void ConstructWall(float deltaTime);
		
	void ChangeState(State state);

	void PickState();

};
