#include "Civ.h"


X::Math::Vector2 Civ::FindTree()
{
	XASSERT(!mGameWorld->mTrees.empty(), "[Find Tree] mTrees is empty.");
	return mGameWorld->mTrees.back().mPosition * 64;
}

X::Math::Vector2 Civ::FindBuildingSite()
{
	XASSERT(!mGameWorld->mWallBuildSites.empty(), "[Find Building Site] mWallBuildingSites is empty.");
	return mGameWorld->mWallBuildSites.back();
}


X::Math::Vector2 Civ::FindStorage()
{
	XASSERT(!mGameWorld->stockpileTiles.empty(), "[Find Storage] stockPileTiles is empty.");
	return X::Math::Vector2(3, 3);
}

void Civ::Initialize(GameWorld* world)
{
	mGameWorld = world;

	mStateMachine.AddState(new IdleState());
	mStateMachine.AddState(new MoveToState());
	mStateMachine.AddState(new GatherState());
	mStateMachine.AddState(new BuildState());
	mStateMachine.AddState(new HaulState());
	mStateMachine.AddState(new SleepState());

	spriteTextureIDs[0] = X::LoadTexture("scv_01.png");
	spriteTextureIDs[1] = X::LoadTexture("scv_02.png");
	spriteTextureIDs[2] = X::LoadTexture("scv_03.png");
	spriteTextureIDs[3] = X::LoadTexture("scv_04.png");
	spriteTextureIDs[4] = X::LoadTexture("scv_05.png");
	spriteTextureIDs[5] = X::LoadTexture("scv_06.png");
	spriteTextureIDs[6] = X::LoadTexture("scv_07.png");
	spriteTextureIDs[7] = X::LoadTexture("scv_08.png");
	spriteTextureIDs[8] = X::LoadTexture("scv_09.png");
	spriteTextureIDs[9] = X::LoadTexture("scv_10.png");
	spriteTextureIDs[10] = X::LoadTexture("scv_11.png");
	spriteTextureIDs[11] = X::LoadTexture("scv_12.png");
	spriteTextureIDs[12] = X::LoadTexture("scv_13.png");
	spriteTextureIDs[13] = X::LoadTexture("scv_14.png");
	spriteTextureIDs[14] = X::LoadTexture("scv_15.png");
	spriteTextureIDs[15] = X::LoadTexture("scv_16.png");

	mStateMachine.ChangeState(Idle);

	mWoodCounter = 0;
	mBuildCounter = 0;
}

void Civ::Update(float deltaTime)
{
	char buffer[50];
	sprintf_s(buffer, "Wood: %d", mWoodCounter);
	X::DrawScreenText(buffer, 700, 150, 16, X::Math::Vector4::Green());
	MoveTo(deltaTime);
	mStateMachine.Update(deltaTime);

	if (!mGameWorld->mResources.empty())
	{
		if (mGameWorld->mResources.back().isPickedUp)
		{
			mGameWorld->mResources.back().mPosition = Civ_Position;
		}
	}

	if (jobStatus == Complete || jobStatus == Cancelled)
	{
		PickState();
	}
}

void Civ::ChangeState(State state)
{
	mStateMachine.ChangeState(state);
}

void Civ::PickState()
{
	float modX = 16.0f * ((mGameWorld->mResources.size() - 1) % 4);
	float modY = floor(((mGameWorld->mResources.size() - 1) / 4.0f)) * 16;

	X::Math::Vector2 offset = X::Math::Vector2(modX, modY);

	X::Math::Vector2 tBuildingSite;
	X::Math::Vector2 tWood;
	X::Math::Vector2 tStorageSite;

	if (!mGameWorld->mTrees.empty())
	{
		tWood = FindTree();
	}

	if (!mGameWorld->mWallBuildSites.empty())
	{
		tBuildingSite = FindBuildingSite();
	}

	if (mGameWorld->stockpileTiles.empty())
	{
		tStorageSite = FindStorage();
	}

	if (mGameWorld->mResources.empty() && mGameWorld->mTrees.empty() && !mGameWorld->mBeds.empty())
	{

		Civ_TargetDestination = mGameWorld->mBeds.back().mPosition;


		if (X::Math::Distance(Civ_Position, Civ_TargetDestination) > 64.0f)
		{
			ChangeState(Move);
		}
		else
		{
			ChangeState(Sleep);
		}
	}
	else if (X::Math::Distance(Civ_Position, Civ_TargetDestination) > 64.0f)
	{
		ChangeState(Move);
	}

	else if (mWoodCounter <= 9)
	{
		if (!mGameWorld->mTrees.empty())
		{

			if (X::Math::Distance(Civ_Position, mGameWorld->mTrees.back().mPosition *cellSize) > cellSize)
			{
				Civ_TargetDestination = tWood;
				mStateMachine.ChangeState(Move);
			}
			else
			{
				mStateMachine.ChangeState(Gather);
			}
		}

	}

	else if (mGameWorld->mTrees.empty() && !mGameWorld->mWallBuildSites.empty())
	{
		if (!mGameWorld->noMoreTrees)
		{
			mGameWorld->mResources.back().isPickedUp = false;
			mGameWorld->mResources.back().mPosition = ((mGameWorld->stockpileTiles.back() * 64) + offset);
			mGameWorld->noMoreTrees = true;
		}

		Civ_TargetDestination = tBuildingSite + offset;
		ChangeState(Move);
		if (!mGameWorld->mResources.empty())
		{
			mGameWorld->mResources.back().isPickedUp = true;
		}
		if (X::Math::Distance(Civ_Position, Civ_TargetDestination) < 64.0f && !mGameWorld->mResources.empty())
		{
			ChangeState(Build);
		}


	}
	else if (mWoodCounter >= 10 && !mGameWorld->mResources.back().isPickedUp && !mGameWorld->mTrees.empty())
	{
		mStateMachine.ChangeState(Haul);
	}
	else if (mGameWorld->mResources.back().isPickedUp && X::Math::Distance(Civ_Position - X::Math::Vector2(32, 32), mGameWorld->stockpileTiles.back() * 64) > 20.0f)
	{
		Civ_TargetDestination = mGameWorld->stockpileTiles.back() * 64;
		ChangeState(Move);
	}

	else if (X::Math::Distance(Civ_Position - X::Math::Vector2(32, 32), mGameWorld->stockpileTiles.back() * 64) < 20.0f && !mGameWorld->mTrees.empty())
	{
		mGameWorld->mResources.back().isPickedUp = false;
		mGameWorld->mResources.back().mPosition = ((mGameWorld->stockpileTiles.back() * 64) + offset);
		mWoodCounter = 0;

		if (!mGameWorld->mTrees.empty())
		{
			Civ_TargetDestination = mGameWorld->mTrees.back().mPosition * 64;
			ChangeState(Move);
		}
	}



}


void Civ::MoveTo(float deltaTime)
{

	if (!Civ_Path.empty()) // as long as stuffs in the list
	{
		X::Math::Vector2 nextPos;

		if (initPath)   // set temp path once
		{
			Civ_ActualPath.clear();
			for (uint32_t i = 0; i < Civ_Path.size(); ++i)
			{
				X::Math::Vector2 newPos;
				newPos.x = (float)Civ_Path[i].x * cellSize + (cellSize / 2.0f);
				newPos.y = (float)Civ_Path[i].y *cellSize + (cellSize / 2.0f);
				Civ_ActualPath.push_back(newPos);
			}
			std::reverse(Civ_ActualPath.begin(), Civ_ActualPath.end());
			Civ_ActualPath.pop_front();
			initPath = false; // never do it again
		}

		if (!Civ_ActualPath.empty())
		{
			Civ_TargetDestination = Civ_ActualPath.back();

			X::Math::Vector2 next;
			next = Civ_ActualPath.front();

			if (X::Math::Distance(next, Civ_Position) < 5.0f)
			{
				if (!Civ_ActualPath.empty())
				{
					Civ_ActualPath.pop_front();
				}
			}
			else
			{
				float dist = X::Math::Distance(Civ_Position, next);
				float speed = 250.0f;

				if (dist > 0.1f)
				{
					X::Math::Vector2 direction = X::Math::Normalize(next - Civ_Position);
					Civ_Position += direction * speed * deltaTime;
				}
				else
				{
					if (!Civ_ActualPath.empty())
					{
						Civ_ActualPath.pop_front();
					}
				}
			}
		}
	}
}

void Civ::GatherWood(float deltaTime)
{
	int counter = 0;
	while (mWoodCounter <= 10)
	{
		if (counter >= 300)
		{
			mWoodCounter++;
		}
		counter++;
	}
}

void Civ::ReturnWood(float deltaTime)
{
	int counter = 0;
	while (mWoodCounter > 0)
	{
		if (counter >= 50)
		{
			mWoodCounter--;
		}
		counter++;
	}
}

void Civ::ConstructWall(float deltaTime)
{
	int counter = 0;
	while (mBuildTime >= 0)
	{
		if (counter >= 100)
		{
			mBuildTime--;
		}
		counter++;
	}
}

void Civ::Render()
{
	X::Math::Vector2 direction = X::Math::Normalize(Civ_TargetDestination - Civ_Position);
	angle = atan2(-direction.x, direction.y) + X::Math::kPi;

	int frame = (int)(angle / X::Math::kTwoPi * 16.0f) % 16;

	X::DrawSprite(spriteTextureIDs[frame], Civ_Position - X::Math::Vector2(32.0f, 32.0f));
	//X::DrawScreenCircle(Civ_TargetDestination, 20.0f, X::Math::Vector4::Red());
}


//ALL
// if hunger > 5 move to food
// if at food, pickup food and move to table
// if at table eat until complete
// if sleep >= 8 move to bed
// sleep state until complete

//BUILDER:
//if no building sites, gather until complete
//if building sites !empty move to closest build site
// build state until complete
//if build state empty hunt

//CHEF
//if no raw meat in stockpile, hunt
//if stove is occupied continue to hunt
// if raw meat in stockpile && stove !occupied, pick up and move to stove
// cook until complete
// when complete, pick up food and move to stockpile
// drop food and pick up raw meat
// move to stove and cook until complete