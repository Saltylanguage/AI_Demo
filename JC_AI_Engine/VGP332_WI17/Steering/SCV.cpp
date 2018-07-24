#include "SCV.h"

void SCV::Initialize()
{

	scvTextureIds[0] = X::LoadTexture("scv_01.png");
	scvTextureIds[1] = X::LoadTexture("scv_02.png");
	scvTextureIds[2] = X::LoadTexture("scv_03.png");
	scvTextureIds[3] = X::LoadTexture("scv_04.png");
	scvTextureIds[4] = X::LoadTexture("scv_05.png");
	scvTextureIds[5] = X::LoadTexture("scv_06.png");
	scvTextureIds[6] = X::LoadTexture("scv_07.png");
	scvTextureIds[7] = X::LoadTexture("scv_08.png");
	scvTextureIds[8] = X::LoadTexture("scv_09.png");
	scvTextureIds[9] = X::LoadTexture("scv_10.png");
	scvTextureIds[10] = X::LoadTexture("scv_11.png");
	scvTextureIds[11] = X::LoadTexture("scv_12.png");
	scvTextureIds[12] = X::LoadTexture("scv_13.png");
	scvTextureIds[13] = X::LoadTexture("scv_14.png");
	scvTextureIds[14] = X::LoadTexture("scv_15.png");
	scvTextureIds[15] = X::LoadTexture("scv_16.png");

	mSteeringModule.AddBehavior(new AI_Library::SeekBehavior());
	mSteeringModule.AddBehavior(new AI_Library::FleeBehavior());
	mSteeringModule.AddBehavior(new AI_Library::ArriveBehavior());
	mSteeringModule.AddBehavior(new AI_Library::WanderBehavior());

	//mSteeringModule.mBehaviors[0]->SetActive(true);
	//mSteeringModule.mBehaviors[1]->SetActive(false);
	//mSteeringModule.mBehaviors[2]->SetActive(false);
	//mSteeringModule.mBehaviors[3]->SetActive(false);

	//mSteeringModule.mBehaviors[0]->SetActive(false);
	//mSteeringModule.mBehaviors[1]->SetActive(true);
	//mSteeringModule.mBehaviors[2]->SetActive(false);

	mSteeringModule.mBehaviors[0]->SetActive(false);
	mSteeringModule.mBehaviors[1]->SetActive(true);
	mSteeringModule.mBehaviors[2]->SetActive(false);
	mSteeringModule.mBehaviors[3]->SetActive(false);

	SetPosition(X::Math::Vector2(5.0f * 64, 5.0f * 64));

	SetMaxSpeed(300);
	mVelocity = X::Math::Vector2::Zero();

}


void SCV::Update(float deltaTime)
{

	X::Math::Vector2 force = mSteeringModule.Calculate();
	X::Math::Vector2 acceleration = force;
	mVelocity = mVelocity + acceleration * deltaTime;
	mVelocity.Truncate(mMaxSpeed);


	mPosition += mVelocity * deltaTime;

	if (mVelocity.LengthSq() > 0.00000001f)
	{
		mHeading = X::Math::Normalize(mVelocity);
		//mVelocity += force;									//if you want speed to be constant leave this line uncommented
	}


}

void SCV::Render()
{
	X::Math::Vector2 direction = X::Math::Normalize(mDestination - mPosition);
	angle = atan2(-direction.x, direction.y) + X::Math::kPi;

	int frame = (int)(angle / X::Math::kTwoPi * 16.0f) % 16;

	X::DrawSprite(scvTextureIds[frame], mPosition);
}

void SCV::Terminate()
{
	mSteeringModule.Purge();
}

