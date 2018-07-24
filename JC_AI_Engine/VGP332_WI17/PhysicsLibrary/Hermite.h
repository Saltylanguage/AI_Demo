#pragma once
#include "..\X\Inc\XEngine.h"

namespace Physics
{



	class Hermite
	{

	public:
		Hermite() {};
		~Hermite() {};

		X::Math::Vector2 A;
		X::Math::Vector2 B;
		X::Math::Vector2 C;
		X::Math::Vector2 D;

		std::vector<X::Math::Vector2> points;
		std::vector<X::Math::Vector2> curvePoints;

		bool isForwards = true;
		float currentTime = 0.0f;
		int pointIndex;

		//forumla:  a * t^3 + b * t^2 + c*t + d 
		//where a = (-p0 - (3 * p1) - (3 * p2) + p3) / 2
		//		b =  p0 - (( 5 * p1) / 2) + (2 * p2) - (p3 / 2)
		//		c = (-p0 + p2) / 2
		//		d = p1

		X::Math::Vector2 GetPointAtClampedIndex(std::vector < X::Math::Vector2> points, int index)
		{
			if (index < 0)
			{
				return points[0];
			}
			else if (index >= int(points.size()))
			{
				return points.back();
			}
			else
			{
				return points[index];
			}
		}

		float CubicHermite(float A, float B, float C, float D, float t)
		{
			float a = (-A / 2.0f) + (3.0f * B / 2.0f) - (3.0f * C / 2.0f) + (D / 2.0f);
			float b = A - (5.0f * B / 2.0f) + (2.0f * C) - (D / 2.0f);
			float c = (-A / 2.0f) + (C / 2.0f);
			float d = B;

			currentTime += t;

			return a*currentTime*currentTime*currentTime + b*currentTime*currentTime + c*currentTime + d;
		}



		X::Math::Vector2 GetXY(std::vector<X::Math::Vector2> points, int numSteps, int i)
		{

				float percent = (float)i / (float)(numSteps - 1);

				float x = 0.0f;
				float y = 0.0f;

				float stepAmount = (points.size() - 1) * percent;
				int index = int(stepAmount);
				currentTime = stepAmount - floor(stepAmount);

				A = GetPointAtClampedIndex(points, index - 1);
				B = GetPointAtClampedIndex(points, index);
				C = GetPointAtClampedIndex(points, index + 1);
				D = GetPointAtClampedIndex(points, index + 2);

				x = CubicHermite(A.x, B.x, C.x, D.x, currentTime);
				y = CubicHermite(A.y, B.y, C.y, D.y, currentTime);

				curvePoints.push_back(X::Math::Vector2(x, y));

				return X::Math::Vector2(x, y);		
		}

	};

}		//namespace physics