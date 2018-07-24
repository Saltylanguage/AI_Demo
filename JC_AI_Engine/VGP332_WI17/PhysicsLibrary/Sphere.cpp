#include "Sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(X::Math::Vector3 center, double rad, X::Math::Vector4 color, double slices, double rings) :
	mColor(color),
	mSlices(slices),
	mRings(rings)
{
	mRadius = mMass = rad;
	mVelocity = X::Math::Vector3(0, 0, 0);
	mPosition = center;
}

Sphere::~Sphere()
{

}

void Sphere::Update(float deltaTime)
{

}



bool Sphere::UpdateCollision(Sphere& sphere2, float deltaTime)
{
	double dist = X::Math::Distance(mPosition, sphere2.mPosition);
	double sumOfRadii = mRadius + sphere2.mRadius;
	X::Math::Vector3 moveVector = (mVelocity - sphere2.mVelocity) * deltaTime;
	dist -= sumOfRadii;
	collisionTimer++;

	if (X::Math::Magnitude(moveVector) < dist)
	{
		if (collisionTimer >= 300)
		{
			mColor = X::Math::Vector4::White();
			sphere2.mColor = X::Math::Vector4::White();
			collisionTimer = 0;
		}
		return false;
	}

	X::Math::Vector3 normalizedMove = X::Math::Normalize(moveVector);
	X::Math::Vector3 positionDifference = sphere2.mPosition - mPosition;

	double dotProduct = X::Math::Dot(normalizedMove, positionDifference);

	if (dotProduct <= 0) // if greater than 0 it means the spheres are moving toward each other
	{
		if (collisionTimer >= 300)
		{
			mColor = X::Math::Vector4::White();
			sphere2.mColor = X::Math::Vector4::White();
			collisionTimer = 0;
		}
		return false;
	}
	double lengthOfDifference = X::Math::Magnitude(positionDifference);
	double distanceSquared = (lengthOfDifference * lengthOfDifference) - (dotProduct * dotProduct);

	double sumOfRadiiSquared = sumOfRadii * sumOfRadii;

	if (distanceSquared >= sumOfRadiiSquared)
	{
		if (collisionTimer >= 300)
		{
			mColor = X::Math::Vector4::White();
			sphere2.mColor = X::Math::Vector4::White();
			collisionTimer = 0;
		}
		return false;
	}

	double actualDistance = sumOfRadiiSquared - distanceSquared;

	if (actualDistance < 0)
	{
		if (collisionTimer >= 300)
		{
			mColor = X::Math::Vector4::White();
			sphere2.mColor = X::Math::Vector4::White();
			collisionTimer = 0;
		}
		return false;
	}

	double distance = dotProduct - sqrt(actualDistance);
	double moveMagnitude = X::Math::Magnitude(moveVector);

	if (moveMagnitude < distance)
	{
		if (collisionTimer >= 300)
		{
			mColor = X::Math::Vector4::White();
			sphere2.mColor = X::Math::Vector4::White();
			collisionTimer = 0;
		}


		return false;
	}

	if (bResolvePosition)
	{
		positionDifference = X::Math::Normalize(positionDifference);

		float a1 = X::Math::Dot(mVelocity, positionDifference);
		float a2 = X::Math::Dot(sphere2.mVelocity, positionDifference);

		float optimizedPoint = (2.0f * (a1 - a2)) / (mMass * sphere2.mMass);

		mVelocity = mVelocity - positionDifference * optimizedPoint * sphere2.mMass;
		sphere2.mVelocity = sphere2.mVelocity + positionDifference * optimizedPoint * mMass;
	}

	mColor = X::Math::Vector4::Red();
	sphere2.mColor = X::Math::Vector4::Red();
	
	return true;
}

void Sphere::DrawSphere()
{
	X::DrawSphere(mPosition, mRadius, mColor, mSlices, mRings);
}

void Sphere::CheckEdgeBoundaries()
{
	if (mPosition.x < 0. && mVelocity.x < 0.)
	{
		mPosition.x = -mPosition.x;
		mVelocity.x = -mVelocity.x;
	}
	if (mPosition.y < 0 && mVelocity.y < 0)
	{
		mPosition.y = -mPosition.y;
		mVelocity.y = -mVelocity.y;
	}
	if (mPosition.x > WorldWidth && mVelocity.x > 0.f)
	{
		mPosition.x = WorldWidth - (mPosition.x - WorldWidth);
		mVelocity.x = -mVelocity.x;
	}
	if (mPosition.y > WorldHeight && mVelocity.y > 0.f)
	{
		mPosition.y = WorldHeight - (mPosition.y - WorldHeight);
		mVelocity.y = -mVelocity.y;
	}
}

void Sphere::CircleCheckEdgeBoundaries()
{
	Bounds bounds;
	if (mPosition.x - mRadius < bounds.min.x)
	{
		mPosition.x = bounds.min.x + mRadius;
		mVelocity.x = -mVelocity.x;
	}
	if (mPosition.y - mRadius < bounds.min.y)
	{
		mPosition.y = bounds.min.y + mRadius;
		mVelocity.y = -mVelocity.y;
	}
	if (mPosition.z - mRadius < bounds.min.z)
	{
		mPosition.z = bounds.min.z + mRadius;
		mVelocity.z = -mVelocity.z;
	}
	if (mPosition.x + mRadius > bounds.max.x)
	{
		mPosition.x = bounds.max.x - mRadius;
		mVelocity.x = -mVelocity.x;
	}
	if (mPosition.y + mRadius > bounds.max.y)
	{
		mPosition.y = bounds.max.y - mRadius;
		mVelocity.y = -mVelocity.y;
	}
	if (mPosition.z + mRadius > bounds.max.z)
	{
		mPosition.z = bounds.max.z - mRadius;
		mVelocity.z = -mVelocity.z;
	}
}

void Sphere::VerletCheckEdgeBoundaries()
{
	if (mPosition.x < 0)
	{
		mPosition.x = -mPosition.x;
		mLastPosition.x = -mLastPosition.x;
	}
	if (mPosition.y < 0)
	{
		mPosition.y = -mPosition.y;
		mLastPosition.y = -mLastPosition.y;
	}
	if (mPosition.x > WorldWidth)
	{
		mPosition.x = WorldWidth - (mPosition.x - WorldWidth);
		mLastPosition.x = -mLastPosition.x;
	}
	if (mPosition.y > WorldHeight)
	{
		mPosition.y = WorldHeight - (mPosition.y - WorldHeight);
		mLastPosition.y = -mLastPosition.y;
	}
}

void Sphere::UpdatePrecisePhysics(double time)
{
	printf("update: timeInc=%f\n", time);
	double ax = 0;
	mPosition = mPosition + mVelocity * time + mGravity * (time * time * 0.5);
	mVelocity = mVelocity + mGravity * time;
	CheckEdgeBoundaries();
}

void Sphere::UpdateSimplePhysics(double time)
{
}

void Sphere::UpdateEulerExplicitPhysics(double time)
{
	mPosition += mVelocity * time;
	mVelocity += mGravity * time;

	CheckEdgeBoundaries();
}

void Sphere::UpdateEulerSemiEXplicitPhysics(double time)
{
	mVelocity += mGravity * time;
	mPosition += mVelocity * time;

	//CheckEdgeBoundaries();
	CircleCheckEdgeBoundaries();
}

void Sphere::UpdateVerletPhysics(double time)
{
	if (mPosition.x == mLastPosition.x)
	{
		mLastPosition = mPosition - (mVelocity * time) + (mGravity * 0.5f * time * time);
	}

	X::Math::Vector3 temp = mPosition;

	mPosition += (mPosition - mLastPosition) + (mGravity * time * time);
	mLastPosition = temp;

	VerletCheckEdgeBoundaries();
}

void Sphere::UpdateRK4Physics(double time)
{
	RigidBody::State s(mPosition, mVelocity);

	Integrate(s, time);

	mPosition = s.position;
	mVelocity = s.velocity + mGravity * time * time;

	CheckEdgeBoundaries();
}


