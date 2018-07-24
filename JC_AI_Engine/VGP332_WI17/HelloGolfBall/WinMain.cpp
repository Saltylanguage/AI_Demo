#include <stdio.h>
#include <Windows.h>
#include <string>
#include "X\Inc\XEngine.h"
#include "PhysicsLibrary\Physics.h"

using namespace Physics;


enum BallState
{
	stop = -1,
	idle = 0,
	start = 1,
	speedUp,
	speedDown,
	angleUp,
	angleDown
};


Ball simBall;
Ball otherBall;

// This is the ball's state
int key = idle;

// These are the dampening forces
double friction = 0.1;
double airResistance = 0.1;

// These values represent the position the user will see from.
float viewX = 25.0f;
float viewY = 5.0f;
float viewZ = 70.0f;

double mAngle = M_PI / 3;  // Inclination of the projectile in radians
const double angleIncrement = M_PI / 180;
double mSpeed = 25.0f;
double radius = .05;

X::Math::Vector3 windVector;
bool resetFlag = false;

X::Math::Vector3 windSpeedIncrement = X::Math::Vector3(0.04, 0.1, 0.1);


void ReadInput()
{

	// Friction Adjustment
	if (X::IsKeyDown(X::Keys::UP))
	{
		friction += 0.1;
		viewY += 0.6;
	}
	if (X::IsKeyDown(X::Keys::DOWN))
	{
		friction = max(1., friction - 0.1);
		viewY -= 0.6;
	}

	// Projectile Slope Adjustment
	if (X::IsKeyDown(X::Keys::LEFT))
	{
		mAngle = max(0., mAngle - angleIncrement);
		viewX -= 0.6;
	}
	if (X::IsKeyDown(X::Keys::RIGHT))
	{
		mAngle = min(90.0, mAngle + angleIncrement);
		viewX += 0.6;
	}

	// Projectile Speed Adjustment
	if (X::IsKeyDown(X::Keys::E))
	{
		mSpeed = max(mSpeed - 5.0, 0.0);
	}
	if (X::IsKeyDown(X::Keys::W))
	{
		mSpeed = min(mSpeed + 5.0, 100.0);
	}

	// View Angle Adjustment
	if (X::IsKeyDown(X::Keys::I))
	{
		viewY = min(viewY + 1.0, 100);
	}
	if (X::IsKeyDown(X::Keys::K))
	{
		viewY = max(viewY - 1.0, 1.0);
	}


}

void InitializePhysics(double rad, double speed, double angle)
{
	windVector = X::Math::Vector3(0.3f, 0.2f, 0.0f);

	double xVelocity = speed * cos(angle);
	double yVelocity = speed * sin(angle);

	simBall.Set(rad, X::Math::Vector3(0.0f, 0.0f, 0.0f), X::Math::Vector3(xVelocity, yVelocity, 0.0), 2, 128, 128, 0);
	simBall.AddForce(windVector);
}


void ResetPhysics()
{
	InitializePhysics(radius, mSpeed, mAngle);
}

void Draw()
{
	X::DrawAABB(50.0f, 50.0f, 50.0f, 100.0f, 100.0f, 100.0f, X::Math::Vector4::White());

	float range = 500.0f;

	for (int i = -range; i <= range; i++)
	{
		X::DrawLine(i, 0.0f, -range, i, 0.0f, range, X::Math::Vector4::Blue());
		X::DrawLine(-range, 0.0f, i, range, 0.0f, i, X::Math::Vector4::Red());
	}
}


bool GameLoop(float deltaTime)
{
	X::Math::Vector3 pos = simBall.GetPosition();

	simBall.AddForce(windSpeedIncrement * deltaTime);

	if (pos.y < -0.01)
	{
		ResetPhysics();
		resetFlag = true;
	}

	if (!resetFlag)
	{
		simBall.UpdateSimplePhysics(deltaTime);
		otherBall.SetPosition(X::Math::Vector3(pos.x, 0.0f, pos.z));
	}

	simBall.DrawCircle(X::Math::Vector4::Red(), radius);
	simBall.DrawCircle(X::Math::Vector4::Blue(), radius);

	Draw();

	X::Math::Vector3 temp = X::Math::Vector3(5.0f, 0.0f, 0.0f);
	X::Math::Matrix rotationZ = X::Math::Matrix::RotationZ(45.0f);
	temp = X::Math::TransformNormal(temp, rotationZ);

	X::SetCameraPosition(X::Math::Vector3(temp + pos));
	X::SetCameraTarget(pos);

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

bool Menu(float deltaTime)
{

	char frictionDisplay[50];
	sprintf_s(frictionDisplay, 50, "[Up / Down] friction: %f", friction);
	X::DrawScreenText(frictionDisplay, 20, 20, 24, X::Math::Vector4::White());

	char slopeDisplay[50];
	sprintf_s(slopeDisplay, 50, "[Left/Right] Slope:  %f", mAngle * 180 / M_PI);
	X::DrawScreenText(slopeDisplay, 20, 50, 24, X::Math::Vector4::White());

	char projSpeedDisplay[50];
	sprintf_s(projSpeedDisplay, 50, "[W/E] Projectile Speed: %f", mSpeed);
	X::DrawScreenText(projSpeedDisplay, 20, 80, 24, X::Math::Vector4::White());

	char viewYDisplay[50];
	sprintf_s(viewYDisplay, 50, "[I/K] Camera Angle: %f", viewY, X::Math::Vector4::White());
	X::DrawScreenText(viewYDisplay, 20, 110, 24, X::Math::Vector4::White());

	X::DrawScreenText("S.....Start Game", 20, 140, 24, X::Math::Vector4::White());
	X::DrawScreenText("ESC...Exit", 20, 170, 24, X::Math::Vector4::White());


	ReadInput();

	if (X::IsKeyDown(X::Keys::ESCAPE))
	{
		key = stop;
		return true;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		key = start;
		return true;
	}

	return false;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	double iAngle = M_PI / 3.0; // projectile inclination angle in radians
	double iSpeed = 15.0; // initial ball speed
	double radius = 0.5;

	InitializePhysics(radius, iSpeed, iAngle);
	X::Start("PhysicsTest", hInstance, 1280, 720);

	X::Run(Menu);
	if (key == start)
	{
		X::Run(GameLoop);
	}

	X::Stop();
	return 0;
}