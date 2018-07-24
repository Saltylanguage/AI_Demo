#pragma once

namespace AI_Library
{
	class PathingCalculator
	{
	public:

		virtual int CalculateCost(int type1, int type2) = 0;
	};
}
