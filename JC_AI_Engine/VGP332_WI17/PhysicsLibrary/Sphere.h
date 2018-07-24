#pragma once
#include "Polygon.h"
#include "RigidBody.h"

class Sphere : public Physics::RigidBody
{
public:
	Sphere();
	Sphere(X::Math::Vector3 center, double rad, X::Math::Vector4 color, double slices, double rings);
	~Sphere();

	bool UpdateCollision(Sphere& sphere2, float deltaTime);	
	void DrawSphere();
	void Update(float delataTime);

	void SetMoveVector(X::Math::Vector3 move) { mVelocity = move; }
	X::Math::Vector3 GetMoveVector() { return mVelocity; }

	void SetColor(X::Math::Vector4 color) { mColor = color; }

	virtual void CheckEdgeBoundaries() override;
	virtual void VerletCheckEdgeBoundaries() override;

	virtual void UpdatePrecisePhysics(double time) override;
	virtual void UpdateSimplePhysics(double time) override;
	virtual void UpdateEulerExplicitPhysics(double time)	override;
	virtual void UpdateEulerSemiEXplicitPhysics(double time)	override;
	virtual void UpdateVerletPhysics(double time)	override;
	virtual void UpdateRK4Physics(double time)	override;

	void CircleCheckEdgeBoundaries();


public:
	double mSlices;
	double mRings;
	X::Math::Vector4 mColor;

	bool bResolvePosition = true;
	int collisionTimer = 0;

};