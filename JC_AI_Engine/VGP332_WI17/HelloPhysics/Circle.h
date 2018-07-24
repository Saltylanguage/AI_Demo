#pragma once
#include <math.h>
#include <Windows.h>
#include "X\Inc\XEngine.h"

#include <gl\GL.h>
#include <gl\GLU.h>

#define M_PI 3.14159

	struct Circle3D
	{
		X::Math::Vector3 currentPos;
		X::Math::Vector3 lastPos;
		X::Math::Vector3 iPos;

		X::Math::Vector3 currentV;
		X::Math::Vector3 lastV;
		X::Math::Vector3 iV;

		float radius;
		int red;
		int green;
		int blue;

		float mass;

		void Set(float rad, X::Math::Vector3 pos, X::Math::Vector3 velocity, float m, int r, int g, int b)
		{
			radius = rad;

			currentPos = pos;
			lastPos = pos;
			iPos = pos;

			currentV = velocity;
			lastV = velocity;
			iV = velocity;

			red = r;
			green = g;
			blue = b;

		}

		void Draw(double angle, int num_segments, int r = -1, int g = -1, int b = -1)
		{
			double theta = 2.0 * M_PI / double(num_segments);
			double cosTheta = cos(theta);
			double sinTheta = sin(theta);

			double x = radius;
			double y = 0.0;
			double t = x;
			x = cos(angle) * x - sin(angle) * y;
			y = sin(angle) * t + cos(angle) * y;

			r = r < 0 ? red : r;
			g = g < 0 ? green : g;
			b = b < 0 ? blue : b;

			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
			glColor3ub(r, g, b);

			for (int i = 0; i < num_segments; i++)
			{
				glVertex2d(x + currentPos.x, y + currentPos.y);

				//apply rotation matrix
				t = x;
				x = cosTheta * x - sinTheta * y;
				y = sinTheta * t + cosTheta * y;
			}
			glEnd();
		}
	};


