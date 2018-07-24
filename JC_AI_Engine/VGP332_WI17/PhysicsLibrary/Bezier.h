#pragma once
#include "..\X\Inc\XEngine.h"

namespace Physics
{


	class Bezier
	{
	public:

#pragma region Notes


		// Start with 2 Points (Start & End)
		// Add a third Control Point

		//First thing to do is to interpolate from Start to the Control Point and keep track of that value (stepValueA).
		//Meanwhile a second interpolation will take place from Control Point to End (stepValueB)

		//Lastly interpolate from stepValueA to stepValueB to create the curve.
		// How this works:
		//stepValueA will attempt to "step" toward stepValue B
		//As stepValueB moves toward the end point, its X & Y values will change which causes stepValueA to move along a different vector.
		//Each frame stepValueB will be closer to the end point, causing stepValueA to move more towards the end as well.

		//Things we need to do each frame:
		//Interpolate from start to control (move stepValue * deltaTime towards controlPoint froms start)
		//Interpolate from control to end	
		//Update these values as you interpolate
		//Draw a debug lines
		//Do a third Interpolation between startToControl and controlToEnd (currentCurvePos will track this value)
		//Draw line from startToControl to controlToEnd

#pragma endregion

		//POINTS
		X::Math::Vector3 start_p0;
		X::Math::Vector3 controlPoint_p1;
		X::Math::Vector3 end_p2;
		X::Math::Vector3 controlPoint2_p3 = X::Math::Vector3(50, 50, 0);
		X::Math::Vector3 finalPoint_pFinal;

		std::vector<X::Math::Vector3> curvePoints;

		bool isForwards = true;

		float time = 0;

		X::Math::Vector3 QuadratiBezier(X::Math::Vector3& p0, X::Math::Vector3& p1, X::Math::Vector3& p2, float& t, X::Math::Vector3& pFinal)
		{
#pragma region Explanation

			//What is actually happening:

			//Lerp from p0 to p1 along the x to get pA_x
			//Lerp from p1 to p2 along the x to get pB_x
			//Lerp from pA_x to pB_x to get pFinal_x

			//Then do the same for Y

			//Simplified to its most basic form, this results in the following equation:

#pragma endregion

			pFinal.x = pow(1 - time, 2) * p0.x + (1 - time) * 2 * time * p1.x + time * time * p2.x;
			pFinal.y = pow(1 - time, 2) * p0.y + (1 - time) * 2 * time * p1.y + time * time * p2.y;
			pFinal.z = pow(1 - time, 2) * p0.z + (1 - time) * 2 * time * p1.z + time * time * p2.z;

			if (time >= 1)
			{
				curvePoints.clear();
				isForwards = false;
			}
			if (time <= 0)
			{
				curvePoints.clear();
				isForwards = true;
			}

			if (isForwards)
			{
				time += t;
			}
			else
			{
				time -= t;
			}
			curvePoints.push_back(pFinal);
			return pFinal;
		}

		X::Math::Vector3 CubicBezier(X::Math::Vector3& p0, X::Math::Vector3& p1, X::Math::Vector3& p2, X::Math::Vector3& p3, float t, X::Math::Vector3& pFinal)
		{



			pFinal.x = pow(1 - time, 3) * p0.x + pow(1 - time, 2) * 3 * time * p1.x + (1 - time) * 3 * time * time * p2.x + time * time * time * p3.x;
			pFinal.y = pow(1 - time, 3) * p0.y + pow(1 - time, 2) * 3 * time * p1.y + (1 - time) * 3 * time * time * p2.y + time * time * time * p3.y;
			pFinal.z = pow(1 - time, 3) * p0.z + pow(1 - time, 2) * 3 * time * p1.z + (1 - time) * 3 * time * time * p2.z + time * time * time * p3.z;

			if (time >= 1)
			{
				isForwards = false;
				curvePoints.clear();
			}
			if (time <= 0)
			{
				isForwards = true;
				curvePoints.clear();
			}

			if (isForwards)
			{
				time += t;
			}
			else
			{
				time -= t;
			}
			curvePoints.push_back(pFinal);
			return pFinal;
		}

	};

}