#include <XEngine.h>
#include "AI_Library\AI_Library.h"
#include "SCV.h"

enum class State
{
	Init,
	Update
};

State state = State::Init;
int cursorTextureId = -1;
float mouseX = 0;
float mouseY = 0;

SCV mSCV;

bool GameLoop(float deltaTime)
{
	switch (state)
	{
		case State::Init:
		{
			cursorTextureId = X::LoadTexture("sword.png");

			mSCV.Initialize();
				
			state = State::Update;				
		}
		break;

		case State::Update:
		{
			mouseX = (float)X::GetMouseScreenX();
			mouseY = (float)X::GetMouseScreenY();

			X::DrawSprite(cursorTextureId, X::Math::Vector2(mouseX, mouseY));
						
			mSCV.SetDestination(X::Math::Vector2(mouseX, mouseY));
			mSCV.Update(deltaTime);		
			mSCV.Render();

			//End of important stuff


			char positionDisplay[50];
			sprintf_s(positionDisplay, "Position: %f, %f", mSCV.GetPosition().x, mSCV.GetPosition().y);

			char destinationDisplay[50];
			sprintf_s(destinationDisplay, "Destination: %f, %f", mSCV.GetDestination().x, mSCV.GetDestination().y);
			
			X::DrawScreenText(positionDisplay, 700, 50, 16, X::Math::Vector4::Green());
			X::DrawScreenText(destinationDisplay, 700, 100, 16, X::Math::Vector4::Green());
		}
		break;
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::SetTexturePath("../Data/Images");

	X::Start("Hello Sprites", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}


// For Homework

// 1) Add the following classes to the AI Library

// - Agent Class  (Pure Data Class)
// - AI_World 
// - SteeringModule
// - SteeringBehaviour


// 2) Implement the following steering behaviours

// - Seek
// - Arrive
// - Hide
// - Pursuit

//Dont worry about these yet
// - Wander
// - Obstacle Avoidance


//
//class Mario : public Agent
//{
//public:
//	void Initialize()
//	{
//		mSteeringModule = new SteeringModule(*this);
//		mSteeringModule->AddBehavior(new SeekBehavior());
//		//mSteeringModule->AddBehavior(new ArriveBehavior());
//		//etc
//	}
//
//	void DoStuff()
//	{
//		mSteeringModule->GetBehavior("Seek")->SetActive(true);
//	}
//
//	void Update()
//	{
//		X::Math::Vector2 force = mSteeringModule->Calculate();
//		//Do Physics from powerpoint
//	}
//
//	void Render()
//	{
//		//Render SCV Sprites
//	}
//
//
//
//private:
//	SteeringModule* mSteeringModule;
//};