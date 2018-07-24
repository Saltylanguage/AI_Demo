#include <stdio.h>
#include <Windows.h>
#include "../X/Inc/XEngine.h"

#include "..\PhysicsLibrary\Physics.h"

using namespace Physics;

enum class GameState
{
	Init,
	Update
};

GameState state = GameState::Init;

Hermite hermite;

int numSteps = 10;

float offset = 5.0f;

float currentTime = 0.0f;

X::Math::Vector4 red = X::Math::Vector4::Red();
X::Math::Vector4 green = X::Math::Vector4::Green();
X::Math::Vector4 blue = X::Math::Vector4::Blue();

int cursorTextureID;

void AddPoint();
void GetCurve();

void DrawCurve();


void ReadInput()
{
	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		AddPoint();
	}

	if (X::IsKeyPressed(X::Keys::C))
	{
		hermite.points.clear();
		hermite.curvePoints.clear();
	}

}

void DrawCursor(int textureId)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	X::DrawSprite(textureId, X::Math::Vector2(mouseX, mouseY));
}

void AddPoint()
{
	hermite.curvePoints.clear();
	float x = X::GetMouseScreenX();
	float y = X::GetMouseScreenY();
	hermite.points.push_back(X::Math::Vector2(x, y));
	hermite.curvePoints.clear();
	GetCurve();
}

void GetCurve()
{
	if (hermite.points.size() >= 4)
	{
		for (int i = 0; i < hermite.points.size(); i++)
		{
			hermite.curvePoints.push_back(hermite.GetXY(hermite.points, numSteps, i));
		}
	}
}

void Draw()
{
	for (int i = 0; i < hermite.points.size(); i++)
	{
		X::DrawScreenRect(hermite.points[i].x - offset, hermite.points[i].y - offset, hermite.points[i].x + offset, hermite.points[i].y + offset, green);
	}

	DrawCursor(cursorTextureID);
	DrawCurve();

}

void DrawCurve()
{
	
	if (hermite.points.size() >= 4)
	{
		for (int i = 0; i < hermite.curvePoints.size() - 1; i++)
		{
			X::Math::Vector2 pointA = hermite.curvePoints[i];
			X::Math::Vector2 pointB = hermite.curvePoints[i + 1];

			X::DrawScreenLine(pointA, pointB, red);
		}
	}
}


bool GameLoop(float deltaTime)
{

	switch (state)
	{
	case GameState::Init:
	{
		cursorTextureID = X::LoadTexture("sword.png");

		X::SetCameraPosition(X::Math::Vector3(50.0f, 50.0f, 50 - 140.0f));
		X::SetCameraTarget(X::Math::Vector3(50.0f, 50.0f, 50.0f));

		state = GameState::Update;
	}
	break;
	case GameState::Update:
	{

		X::DrawScreenText("[Left Click] Add a Point", 0, 0, 16, green);
		X::DrawScreenText("[C] Clear Points", 0, 20, 16, green);

		char curveSizeDisplay[50];
		sprintf_s(curveSizeDisplay, "Points %d", hermite.points.size());
		X::DrawScreenText(curveSizeDisplay, 900, 0, 40, green);

		char timeDisplay[500];
		sprintf_s(timeDisplay, "Time: %f", hermite.currentTime);
		X::DrawScreenText(timeDisplay, 850, 40, 40, green);

		ReadInput();
		Draw();
		
		float xValue = hermite.CubicHermite(hermite.A.x, hermite.B.x, hermite.C.x, hermite.D.x, deltaTime);
		float yValue = hermite.CubicHermite(hermite.A.y, hermite.B.y, hermite.C.y, hermite.D.y, deltaTime);


		X::DrawScreenCircle(X::Math::Vector2(xValue, yValue), 10, blue);
		hermite.curvePoints.push_back(X::Math::Vector2(xValue, yValue));
	}
	break;
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::SetTexturePath("../Data/Images");

	X::Start("PhysicsTest", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}