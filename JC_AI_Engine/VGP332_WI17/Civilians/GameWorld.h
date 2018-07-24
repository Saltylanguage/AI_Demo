#pragma once
#include <vector>
#include "Civ.h"

#include "Wall.h"
#include "Floor.h"
#include "Tree.h"
#include "Resource.h"
#include "Animal.h"
#include "Bed.h"

#include "AI_Library\Graph.h"
#include <algorithm>

class GameWorld
{
public:


	std::vector<X::Math::Vector2>  backgroundTiles;

	std::vector<Wall> mWalls;
	std::vector<Floor> mFloors;
	std::vector<Civ> mCivs;
	std::vector<Tree> mTrees;
	std::vector<Bed> mBeds;
	std::vector<Animal> mAnimals;
	std::vector<Resource> mResources;

	std::vector<X::Math::Vector2> stockpileTiles;

	int woodCounter;
	int foodCounter;

	std::vector<X::Math::Vector2> mWallBuildSites;
	std::vector<X::Math::Vector2> mFloorBuildSites;

	AI_Library::Graph* mGraph;

	bool noMoreTrees = false;
	bool isAsleep = false;
	/*bool createFloor = false;*/
	int counter = 0;

	void Update(float deltaTime)
	{
		char resourcePosition[50];

		if (!mResources.empty())
		{
			sprintf_s(resourcePosition, 50, "Resource: %f, %f", mResources.back().mPosition.x, mResources.back().mPosition.y);
			X::DrawScreenText(resourcePosition, 700, 400, 16, X::Math::Vector4::Green());
		}

		//if (createFloor)
		//{
		//	for (int i = 3; i < 6; i++) //TODO change i to not 0
		//	{
		//		if (counter >= 30)
		//		{
		//			for (int j = 3; j < 6; j++)
		//			{
		//				mFloors.push_back(Floor(X::Math::Vector2(i, j)));
		//			}
		//		}
		//		counter++;
		//	}
		//	mBeds.push_back(Bed(X::Math::Vector2(3, 3)));
		//	createFloor = false;
		//}



		RemoveTree();
	}

	void CheckTrees()
	{
		if (!mTrees.empty())
		{
			if (mTrees.back().woodCounter <= 0)
			{
				mTrees.back().isChoppedDown = true;
			}
		}

	}

	void RemoveTree()
	{
		if (!mTrees.empty())
		{
			if (mTrees.back().isChoppedDown)
			{
				mTrees.pop_back();
			}
		}
	}
};
