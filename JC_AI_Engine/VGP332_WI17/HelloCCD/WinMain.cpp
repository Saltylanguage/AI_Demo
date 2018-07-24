#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "X\Inc\XEngine.h"
#include "PhysicsLibrary\Physics.h"

enum State
{
	Stop = -1,
	Idle = 0,
	Start = 1,
	SpeedUp,
	SpeedDown,
	AngleUp,
	AngleDown
};

X::Math::Vector4 green = X::Math::Vector4::Green();

std::vector<Sphere> spheres;
int numSpheres = 10;
X::Math::Vector3 cameraPosition = X::Math::Vector3(-100.0f, 0.0f, -250.0f);
X::Math::Vector3 cameraDirection = X::Math::Vector3(50.0f, 0.0f, 50.0f);



int key = Idle;
int polysize = 3;
int polysize2 = 3;

void CreatePoly()
{
	srand(time(NULL));

	for (int i = 0; i < numSpheres; i++)
	{
		int randX = rand() % 50;
		int randY = rand() % 150;
		int randZ = 0;

		int randSize = rand() % 15;
		spheres.push_back(Sphere(X::Math::Vector3(randX, randY, randZ), polysize, X::Math::Vector4::White(), 10, 10));
	}
}

void PollKeys()
{
	////Poly1
	//if (X::IsKeyDown(X::Keys::UP))
	//{
	//	polysize = min(10, polysize + 1);
	//}
	//else if (X::IsKeyDown(X::Keys::DOWN))
	//{
	//	polysize = max(1, polysize - 1);
	//}

	////Poly2
	//if (X::IsKeyDown(X::Keys::RIGHT))
	//{
	//	polysize2 = min(10, polysize2 + 1);
	//}
	//else if (X::IsKeyDown(X::Keys::DOWN))
	//{
	//	polysize2 = max(1, polysize2 - 1);
	//}
}

void Yaw(float_t degree)
{
	X::Math::Matrix matRotate = X::Math::Matrix::RotationY(degree * X::Math::kDegToRad);
	cameraDirection = X::Math::TransformNormal(cameraDirection, matRotate);
}

void Pitch(float_t degree)
{
	const X::Math::Vector3 right = X::Math::Normalize(X::Math::Cross(X::Math::Vector3::YAxis(), cameraDirection));
	const X::Math::Matrix matRotate = X::Math::MatrixRotationAxis(right, degree * X::Math::kDegToRad);
	const X::Math::Vector3 newLook = X::Math::TransformNormal(cameraDirection, matRotate);
	const float_t dot = X::Math::Dot(newLook, X::Math::Vector3::YAxis());


	if (X::Math::Abs(dot) < 0.995)
	{
		cameraDirection = newLook;
	}
}

bool GameLoop(float deltaTime)
{
	const float kSpeed = 1.0f;
	const float_t kTurnSpeed = 80.0f;

	if (X::IsKeyPressed(X::Keys::ENTER))
	{
		for (int i = 0; i < spheres.size(); i++)
		{
			if (spheres[i].bResolvePosition)
			{
				spheres[i].bResolvePosition = false;
			}
			else
			{
				spheres[i].bResolvePosition = true;
			}
		}
	}

	//if (X::IsKeyDown(X::Keys::W))
	//{
	//	cameraPosition += cameraDirection * kSpeed * deltaTime;
	//}
	//else if (X::IsKeyDown(X::Keys::S))
	//{
	//	cameraPosition -= cameraDirection * kSpeed * deltaTime;
	//}

	//if (X::IsKeyDown(X::Keys::D))
	//{
	//	const X::Math::Vector3 right = X::Math::Normalize(X::Math::Cross(X::Math::Vector3::YAxis(), cameraDirection));
	//	cameraPosition += right * kSpeed * deltaTime;
	//}

	//else if (X::IsKeyDown(X::Keys::A))
	//{
	//	const X::Math::Vector3 right = X::Math::Normalize(X::Math::Cross(X::Math::Vector3::YAxis(), cameraDirection));
	//	cameraPosition -= right * kSpeed * deltaTime;
	//}

	//p1.SetMoveVec(vec1);

	for (int i = 0; i < numSpheres; ++i)
	{
		for (int j = i + 1; j < numSpheres; ++j)
		{
			spheres[i].UpdateCollision(spheres[j], deltaTime);
		}
		spheres[i].UpdateEulerSemiEXplicitPhysics(deltaTime);

		spheres[i].DrawSphere();

	}

	X::SetCameraPosition(cameraPosition);
	//Yaw(X::GetMouseMoveX() * kTurnSpeed * deltaTime);
	//Pitch(X::GetMouseMoveY() * kTurnSpeed * deltaTime);
	X::SetCameraDirection(cameraDirection);
	X::DrawAABB(0, 0, 0, 100, 100, 100, green);

	char cameraPosDisplay[100];
	sprintf_s(cameraPosDisplay, "Camera Position: %f, %f, %f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	X::DrawScreenText(cameraPosDisplay, 800, 40, 16, green);

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

bool Menu(float deltaTime)
{
	char polysizeDisplay[100];
	sprintf_s(polysizeDisplay, "Polysize1: %d", polysize);
	X::DrawScreenText(polysizeDisplay, 900, 0, 16, green);

	char polysize2Display[100];
	sprintf_s(polysize2Display, "Polysize2: %d", polysize2);
	X::DrawScreenText(polysize2Display, 900, 20, 16, green);

	X::DrawScreenText("S.....Start Game", 20, 0, 24, green);
	X::DrawScreenText("ESC...Exit", 20, 30, 24, green);

	PollKeys();

	if (X::IsKeyDown(X::Keys::ESCAPE))
	{
		key = Stop;
		return true;
	}

	else if (X::IsKeyDown(X::Keys::S))
	{
		key = Start;
		return true;
	}

	//initPhysics(radius, iSpeed, iAngle);
	return false;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start("PhysicsTest", hInstance, 1280, 720);
	X::Run(Menu);

	if (key == Start)
	{
		CreatePoly();
		X::Run(GameLoop);
	}
	X::Stop();
	return 0;
}