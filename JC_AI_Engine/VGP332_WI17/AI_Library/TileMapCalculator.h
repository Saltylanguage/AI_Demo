#pragma once
#include "PathingCalculator.h"
namespace AI_Library
{
	class TileMapCalculator : public PathingCalculator
	{
	public:
		int CalculateCost(int type1, int type2)
		{
			const int cost[4][4] =
			{
				{ 1,	 1,		1,	   10000 },
				{ 1,	 1,     2,     10000 },
				{ 1,	 2,     2,     10000 },
				{ 10000, 10000, 10000, 10000 }
			};

			//0 = black
			//1 = grass
			//2 = sand
			//3 = water

			return cost[type1][type2];
		}
	};
}