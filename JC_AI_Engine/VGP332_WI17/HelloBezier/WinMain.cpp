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


Physics::Bezier bezierCurve;
Physics::Ball lerpBall;

float offset = 5.0f;

X::Math::Vector4 red = X::Math::Vector4::Red();
X::Math::Vector4 green = X::Math::Vector4::Green();
X::Math::Vector4 blue = X::Math::Vector4::Blue();

int cursorTextureID;

bool cubicFlag;

void SetStartCoord();
void SetEndCoord();
void SetControlCoord();
void SetCubicControlCoord();

void DrawCurve();

void ReadInput()
{
	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		SetStartCoord();
	}
	if (X::IsMousePressed(X::Mouse::RBUTTON))
	{
		SetEndCoord();
	}
	if (X::IsMousePressed(X::Mouse::MBUTTON))
	{
		SetControlCoord();
	}
	if (X::IsKeyPressed(X::Keys::LSHIFT))
	{
		SetCubicControlCoord();
	}
	if (X::IsKeyPressed(X::Keys::C))
	{
		bezierCurve.curvePoints.clear();
		bezierCurve.time = 0;
		if (!cubicFlag)
		{
			cubicFlag = true;
		}
		else
		{
			bezierCurve.controlPoint2_p3 = X::Math::Vector3(50, 50, 0);
			cubicFlag = false;
		}

	}

}

void DrawCursor(int textureId)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	X::DrawSprite(textureId, X::Math::Vector2(mouseX, mouseY));

}

void SetStartCoord()
{
	bezierCurve.start_p0.x = X::GetMouseScreenX();
	bezierCurve.start_p0.y = X::GetMouseScreenY();
	bezierCurve.start_p0.z = 0.0f;
}

void SetControlCoord()
{
	bezierCurve.controlPoint_p1.x = X::GetMouseScreenX();
	bezierCurve.controlPoint_p1.y = X::GetMouseScreenY();
	bezierCurve.controlPoint_p1.z = 0.0f;
}

void SetCubicControlCoord()
{
	bezierCurve.controlPoint2_p3.x = X::GetMouseScreenX();
	bezierCurve.controlPoint2_p3.y = X::GetMouseScreenY();
	bezierCurve.controlPoint2_p3.z = 0.0f;
}

void SetEndCoord()
{
	bezierCurve.end_p2.x = X::GetMouseScreenX();
	bezierCurve.end_p2.y = X::GetMouseScreenY();
	bezierCurve.end_p2.z = 0.0f;
}

void Draw()
{
//	int circleSections = 10;

	X::DrawScreenRect(bezierCurve.start_p0.x - offset, bezierCurve.start_p0.y - offset, bezierCurve.start_p0.x + offset, bezierCurve.start_p0.y + offset, green);
	X::DrawScreenRect(bezierCurve.end_p2.x - offset, bezierCurve.end_p2.y - offset, bezierCurve.end_p2.x + offset, bezierCurve.end_p2.y + offset, red);
	X::DrawScreenRect(bezierCurve.controlPoint_p1.x - offset, bezierCurve.controlPoint_p1.y - offset, bezierCurve.controlPoint_p1.x + offset, bezierCurve.controlPoint_p1.y + offset, X::Math::Vector4::Orange());
	X::DrawScreenRect(bezierCurve.controlPoint2_p3.x - offset, bezierCurve.controlPoint2_p3.y - offset, bezierCurve.controlPoint2_p3.x + offset, bezierCurve.controlPoint2_p3.y + offset, X::Math::Vector4::Yellow());

	DrawCursor(cursorTextureID);

	DrawCurve();

	
	X::DrawScreenCircle(X::Math::Vector2(bezierCurve.finalPoint_pFinal.x, bezierCurve.finalPoint_pFinal.y), 10.0f, blue);
	//lerpBall.DrawCircle(X::Math::Vector4::Blue(), 1.0f);

}

void DrawCurve()
{
	if (bezierCurve.curvePoints.size() >= 3)
	{
		for (int i = 0; i < bezierCurve.curvePoints.size() - 1; i++)
		{
			X::Math::Vector2 pointA = X::Math::Vector2(bezierCurve.curvePoints[i].x, bezierCurve.curvePoints[i].y);
			X::Math::Vector2 pointB = X::Math::Vector2(bezierCurve.curvePoints[i + 1].x, bezierCurve.curvePoints[i + 1].y);
			X::DrawScreenLine(pointA, pointB, red);
		}
	}
}

void InitializePhysics(double rad, double speed, double angle)
{
	double vx = speed * cos(angle);
	double vy = speed * sin(angle);

	X::Math::Vector3 iPos(rad, rad, 0);
	X::Math::Vector3 iVel(vx, vy, 0);

	lerpBall.Set(rad, iPos, iVel, 2, 128, 128, 128);


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

		bezierCurve.start_p0 = X::Math::Vector3(200, 500, 0);
		bezierCurve.controlPoint_p1 = X::Math::Vector3(500, 70, 0);
		bezierCurve.end_p2 = X::Math::Vector3(800, 500, 0);

		cubicFlag = true;
		state = GameState::Update;
	}
	break;
	case GameState::Update:
	{

		if (!cubicFlag)
		{
			bezierCurve.QuadratiBezier(bezierCurve.start_p0, bezierCurve.controlPoint_p1, bezierCurve.end_p2, deltaTime, bezierCurve.finalPoint_pFinal);
		}
		else
		{
			bezierCurve.CubicBezier(bezierCurve.start_p0, bezierCurve.controlPoint_p1, bezierCurve.controlPoint2_p3, bezierCurve.end_p2, deltaTime, bezierCurve.finalPoint_pFinal);
		}

		lerpBall.SetPosition(bezierCurve.finalPoint_pFinal);

		X::DrawScreenText("[Left Click] Set Start Point", 0, 0, 16, green);
		X::DrawScreenText("[Right Click] Set End Point", 0, 20, 16, green);
		X::DrawScreenText("[Middle Click] Set Control Point 1 (Quadratic)", 0, 40, 16, green);
		X::DrawScreenText("[Left Shift] Set Control Point 2 (Cubic)", 0, 60, 16, green);
		X::DrawScreenText("[C] Toggle Quadratic / Cubic Bezier Curve", 0, 80, 16, green);

		char ballPosDisplay[50];
		sprintf_s(ballPosDisplay, "Ball Position: %f, %f", lerpBall.GetPosition().x, lerpBall.GetPosition().y);
		X::DrawScreenText(ballPosDisplay, 900, 0, 16, green);

		char bezierFinalDisplay[50];
		sprintf_s(bezierFinalDisplay, "Lerp Result: %f, %f", bezierCurve.finalPoint_pFinal.x, bezierCurve.finalPoint_pFinal.y);
		X::DrawScreenText(bezierFinalDisplay, 900, 20, 16, green);

		char timeDisplay[50];
		sprintf_s(timeDisplay, 50, "Time: %f", bezierCurve.time);
		X::DrawScreenText(timeDisplay, 900, 40, 16, green);

		ReadInput();
		Draw();
	}
	break;
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::SetTexturePath("../Data/Images");
	double iAngle = M_PI / 3.0; // projectile inclination angle in radians
	double iSpeed = 15.0; // initial ball speed
	double radius = 0.5;

	InitializePhysics(radius, iSpeed, iAngle);
	X::Start("PhysicsTest", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}