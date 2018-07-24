#pragma once
#include "X\Inc\XEngine.h"

namespace AI_Library
{


	class Agent
	{
	public:

		Agent() {};
		virtual ~Agent() {};

		//add getter & setter

		X::Math::Matrix3 LocalToWorldTransform() const
		{
			return X::Math::Matrix3(
				-mHeading.y, mHeading.x, 0,
				mHeading.y, mHeading.x, 0,
				mPosition.x, mPosition.y, 1
				);
		}

		void SetPosition(X::Math::Vector2 pos) { mPosition = pos; }
		void SetDestination(X::Math::Vector2 dest) { mDestination = dest; }
		void SetHeading(X::Math::Vector2  heading) { mHeading = heading; }
		void SetVelocity(X::Math::Vector2 velocity) { mVelocity = velocity; }

		void SetMaxSpeed(float max) { mMaxSpeed = max; }

		X::Math::Vector2 GetPosition() { return mPosition; }
		X::Math::Vector2 GetDestination() { return mDestination; }
		X::Math::Vector2 GetHeading() { return mHeading; }
		X::Math::Vector2 GetVelocity() { return mVelocity; }

		float GetMaxSpeed() { return mMaxSpeed; }

	protected:

		//_World* mWorld;
		X::Math::Vector2 mPosition;
		X::Math::Vector2 mDestination;
		X::Math::Vector2 mHeading;
		X::Math::Vector2 mVelocity;

		float mMaxSpeed;

		//etc

	};

} 