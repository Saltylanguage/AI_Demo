#include "Ball.h"

void Ball::UpdatePrecisePhysics(double time)
{
	printf("Update: time = %f\n", time);
	double ax = 0;
	circle.currentPos = circle.currentPos + circle.currentV * time + mGravity * (time * time * 0.5);
	circle.currentV = circle.currentV + mGravity * time;
	CheckEdgeBoundaries();
}

void Ball::CheckEdgeBoundaries()
{
	if (circle.currentPos.x < 0.0 && circle.currentV.x < 0.0)
	{
		circle.currentPos.x = -circle.currentPos.x;
		circle.currentV.x = -circle.currentV.x;
	}
	if (circle.currentPos.y < 0 && circle.currentV.y < 0)
	{
		circle.currentPos.y = -circle.currentPos.y;
		circle.currentV.y = -circle.currentV.y;
	}
	if (circle.currentPos.x > worldWidth && circle.currentV.x > 0.0f)
	{
		circle.currentPos.x = worldWidth - (circle.currentPos.x - worldWidth);
		circle.currentV.x = -circle.currentV.x;
	}
	if (circle.currentPos.y > worldHeight && circle.currentV.y > 0.0f)
	{
		circle.currentPos.y = worldHeight - (circle.currentPos.y - worldHeight);
		circle.lastPos.y = -circle.lastPos.y;
	}
}

void Ball::VerletCheckEdgeBoundaries()
{
	if (circle.currentPos.x < 0)
	{
		circle.currentPos.x = -circle.currentPos.x;
		circle.lastPos.x = -circle.lastPos.x;
	}
	if (circle.currentPos.y < 0)
	{
		circle.currentPos.y = -circle.currentPos.y;
		circle.lastPos.y = -circle.lastPos.y;
	}
	if (circle.currentPos.x > worldWidth)
	{
		circle.currentPos.x = worldWidth - (circle.currentPos.x - worldWidth);
		circle.lastPos.x = -circle.lastPos.x;
	}
	if (circle.currentPos.y > worldHeight)
	{
		circle.currentPos.y = worldHeight - (circle.currentPos.y - worldHeight);
		circle.lastPos.y = -circle.lastPos.y;
	}
}

void Ball::UpdateEulerExplicitPhysics(double time)
{
	//////////// your physics goes here //////////////////////////
	// we use a coordinate system in which x goes from left to right of the screen and y goes from top to bottom of the screen
	// we have 1 forces here: 1) mGravity which is in positive y direction. 
	//////////////Explicit Euler Integration:///////////////////////

	circle.currentPos += circle.currentV * time;
	circle.currentV += mGravity * time;

	CheckEdgeBoundaries();
}

void Ball::UpdateEulerSemiEXplicitPhysics(double time)
{
	//////////// your physics goes here //////////////////////////
	// we use a coordinate system in which x goes from left to right of the screen and y goes from top to bottom of the screen
	// we have 1 forces here: 1) mGravity which is in positive y direction. 
	//////////////Explicit Euler Integration:///////////////////////

	circle.currentV += mGravity * time;
	circle.currentPos += circle.currentV * time;

	CheckEdgeBoundaries();
}

void Ball::UpdateVerletPhysics(double time)
{
	// More exact formula for variable time step is the following:
	//  x_2 = x_1 + (x_1 - x_0)(dt_1/dt_0) + accel * dt_1 * dt_1

	if(circle.currentPos.x == circle.lastPos.x)
	{
		circle.lastPos = circle.currentPos - (circle.currentV * time) + (mGravity * 0.5f * time *  time);
	}

	X::Math::Vector3 temp = circle.currentPos;

	circle.currentPos += (circle.currentPos - circle.lastPos) + (mGravity * time * time);
	circle.lastPos = temp;

	VerletCheckEdgeBoundaries();
}

void Ball::UpdateRK4Physics(double time)
{
	State state(circle.currentPos, circle.currentV);

	Integrate(state, time);

	circle.currentPos = state.position;
	circle.currentV = state.velocity + mGravity * time * time;

	CheckEdgeBoundaries();
}