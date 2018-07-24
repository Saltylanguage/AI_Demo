#include <stdio.h>
#include <Windows.h>
#include "X\Inc\XEngine.h"
#include "PhysicsLibrary\Physics.h"

using namespace Physics;

Ball ExplicitEulerBall;
Ball SemiExplicitEulerBall;
Ball VerletBall;
Ball RK4Ball;

void DrawBalls()
{
	int circleSections = 10;
	
	ExplicitEulerBall.DrawCircle(X::Math::Vector4::Blue(), 1.0f);
	SemiExplicitEulerBall.DrawCircle(X::Math::Vector4::Cyan(), 1.0f);
	VerletBall.DrawCircle(X::Math::Vector4::Red(), 1.0f);
	RK4Ball.DrawCircle(X::Math::Vector4::Magenta(), 1.0f);
}

void InitializePhysics(double rad, double speed, double angle)
{
	double vx = speed * cos(angle);
	double vy = speed * sin(angle);

	X::Math::Vector3 iPos(rad, rad, 0);
	X::Math::Vector3 iVel(vx, vy, 0);
	ExplicitEulerBall.Set(rad, iPos, iVel, 2, 128, 128, 128);
	SemiExplicitEulerBall.Set(rad, iPos, iVel, 2, 230, 0, 0);
	VerletBall.Set(rad, iPos, iVel, 2, 0, 200, 0);
	RK4Ball.Set(rad, iPos, iVel, 2, 0, 200, 200);

}

bool GameLoop(float deltaTime)
{
	ExplicitEulerBall.UpdateEulerExplicitPhysics(deltaTime);
	SemiExplicitEulerBall.UpdateEulerSemiEXplicitPhysics(deltaTime);
	VerletBall.UpdateVerletPhysics(deltaTime);
	RK4Ball.UpdateRK4Physics(deltaTime);

	DrawBalls();
	X::DrawAABB(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 100.0f, X::Math::Vector4::White());

	X::SetCameraPosition(X::Math::Vector3(50.0f, 50.0f, 50 - 140.0f));

	X::SetCameraTarget(X::Math::Vector3(50.0f, 50.0f, 50.0f));

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	
	double iAngle = M_PI / 3.0; // projectile inclination angle in radians
	double iSpeed = 15.0; // initial ball speed
	double radius = 0.5;

	InitializePhysics(radius, iSpeed, iAngle);
	X::Start("PhysicsTest", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}