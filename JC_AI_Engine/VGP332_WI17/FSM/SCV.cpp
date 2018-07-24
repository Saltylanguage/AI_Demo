#include "SCV.h"


X::Math::Vector2 SCV::FindMinerals()
{
	return X::Math::Vector2(5 *cellSize, 5 *cellSize);
}

X::Math::Vector2 SCV::FindBuildingSite()
{
	return X::Math::Vector2(9 * cellSize, 9 * cellSize);
}

X::Math::Vector2 SCV::FindStorage()
{
	return X::Math::Vector2(3, 3);
}

void SCV::Initialize()
{
	mStateMachine.AddState(new IdleState());
	mStateMachine.AddState(new MoveToState());
	mStateMachine.AddState(new GatherState());
	mStateMachine.AddState(new BuildState());

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

	storageTextureId[0] = X::LoadTexture("refinery_01.png");
	storageTextureId[1] = X::LoadTexture("refinery_02.png");
	storageTextureId[2] = X::LoadTexture("refinery_03.png");
	storageTextureId[3] = X::LoadTexture("refinery_04.png");
	storageTextureId[4] = X::LoadTexture("refinery_05.png");


	mStateMachine.ChangeState(Idle);

	mMineralCounter = 0;
	mBuildCounter = 0;
}

void SCV::Update(float deltaTime)
{
	char buffer[50];
	sprintf_s(buffer, "Minerals: %d", mMineralCounter);	
	X::DrawScreenText(buffer, 700, 150, 16, X::Math::Vector4::Green());
	MoveTo(deltaTime);
	mStateMachine.Update(deltaTime);

    if (jobStatus == Complete || jobStatus == Cancelled)
    {
        PickState();
    }
}

void SCV::ChangeState(State state)
{
	mStateMachine.ChangeState(state);
}

void SCV::PickState()
{
	
	X::Math::Vector2 tMinerals = FindMinerals();
	X::Math::Vector2 tBuildingSite = FindBuildingSite();
	X::Math::Vector2 tStorageSite = FindStorage();

	if (mMineralCounter <= 5)
	{

		if (X::Math::Distance(SCV_Position, tMinerals) > cellSize)
		{
			SCV_TargetDestination = tMinerals;
			mStateMachine.ChangeState(Move);			
		}
		else
		{
			mStateMachine.ChangeState(Gather);
		}

	}
	else
	{

		if (X::Math::Distance(SCV_Position, tBuildingSite) > cellSize)
		{
			SCV_TargetDestination = tBuildingSite;
			mStateMachine.ChangeState(Move);
		}
		else
		{
			mStateMachine.ChangeState(Build);
		}
	}
	/*else if (!StorageLocations.empty())
	{
		if (X::Math::Distance(SCV_Position, tStorageSite) > 5.0f)
		{
			SCV_TargetDestination = tStorageSite;
			mStateMachine.ChangeState(Move);
		}
		else
		{
			mStateMachine.ChangeState(Return);
		}
	}
	else
	{
		mStateMachine.ChangeState(Idle);
	}*/

	
}


void SCV::MoveTo(float deltaTime)
{

	if (!SCV_Path.empty()) // as long as stuffs in the list
	{
		X::Math::Vector2 nextPos;

		if (initPath)   // set temp path once
		{
			SCV_ActualPath.clear();
			for (uint32_t i = 0; i < SCV_Path.size(); ++i)
			{
				X::Math::Vector2 newPos;
				newPos.x = (float)SCV_Path[i].x * cellSize + (cellSize / 2.0f);
				newPos.y = (float)SCV_Path[i].y *cellSize + (cellSize / 2.0f);
				SCV_ActualPath.push_back(newPos);
			}
			std::reverse(SCV_ActualPath.begin(), SCV_ActualPath.end());
			SCV_ActualPath.pop_front();
			initPath = false; // never do it again
		}

		if (!SCV_ActualPath.empty())
		{
			SCV_NextDestination = SCV_ActualPath.back();

			X::Math::Vector2 next;
			next = SCV_ActualPath.front();

			if (X::Math::Distance(next, SCV_Position) < 5.0f)
			{
				if (!SCV_ActualPath.empty())
				{
					SCV_ActualPath.pop_front();
				}
			}
			else
			{
				float dist = X::Math::Distance(SCV_Position, next);
				float speed = 100;

				if (dist > 0.1f)
				{
					X::Math::Vector2 direction = X::Math::Normalize(next - SCV_Position);
					SCV_Position += direction * speed * deltaTime;			
				}
				else
				{
					if (!SCV_ActualPath.empty())
					{
						SCV_ActualPath.pop_front();
					}
				}
			}
		}			
	}
}

void SCV::GatherMinerals(float deltaTime)
{
	int counter = 0;
	while (mMineralCounter <= 10)
	{
		if (counter >= 300)
		{
			mMineralCounter++;
		}
		counter++;
	}
}

void SCV::ReturnMinerals(float deltaTime)
{
	int counter = 0;
	while (mMineralCounter > 0)
	{
		if (counter >= 50)
		{
			mMineralCounter--;
		}
		counter++;
	}
}

void SCV::ConstructWall(float deltaTime)
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

void SCV::Render()
{
	X::Math::Vector2 direction = X::Math::Normalize(SCV_NextDestination- SCV_Position);
	angle = atan2(-direction.x, direction.y) + X::Math::kPi;

	int frame = (int)(angle / X::Math::kTwoPi * 16.0f) % 16;

	X::DrawSprite(spriteTextureIDs[frame], SCV_Position - X::Math::Vector2(32.0f, 32.0f));
	X::DrawScreenCircle(SCV_NextDestination, 20.0f, X::Math::Vector4::Red());
}