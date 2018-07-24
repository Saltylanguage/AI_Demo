#pragma once
#include "Circle.h"
#include "RigidBody.h"

class Ball : public RigidBody
{
public:
	void DrawCircle(X::Math::Vector4 color)
	{
		X::DrawSphere(circle.currentPos, 1.0f, color);
	}

	void Set(double rad, X::Math::Vector3 pos, X::Math::Vector3 vel, float m, float r, float g, float b)
	{
		circle.Set(rad, pos, vel, m, r, g, b);
	}

	X::Math::Vector3 GetPosition() { return circle.currentPos; }
	virtual void UpdatePrecisePhysics(double time) override;

	virtual void CheckEdgeBoundaries() override;
	virtual void VerletCheckEdgeBoundaries() override;

	virtual void UpdateEulerExplicitPhysics			(double time)	override;	
	virtual void UpdateEulerSemiEXplicitPhysics		(double time)	override;
	virtual void UpdateVerletPhysics				(double time)	override;
	virtual void UpdateRK4Physics					(double time)	override;

protected:
	Circle3D circle;

	float worldWidth = 100;
	float worldHeight = 100;
	X::Math::Vector3 mGravity = X::Math::Vector3(0.0f, -9.81, 0.0f);

};