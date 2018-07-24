#pragma once
#include "Circle.h"
#include "RigidBody.h"

namespace Physics
{

	class Ball : public RigidBody
	{
	public:

		Ball();
		~Ball();

		void DrawCircle(X::Math::Vector4 color, float radius)
		{
			X::DrawSphere(circle.currentPos, radius, color);
		}

		void Set(double rad, X::Math::Vector3 pos, X::Math::Vector3 vel, float m, float r, float g, float b)
		{
			circle.Set(rad, pos, vel, m, r, g, b);
		}

		X::Math::Vector3 GetPosition() { return circle.currentPos; }
		void SetPosition(X::Math::Vector3 pos) { circle.currentPos = pos; }

		void AddForce(X::Math::Vector3 force);
		void AddDrag(X::Math::Vector3 force);

		virtual void CheckEdgeBoundaries() override;
		virtual void VerletCheckEdgeBoundaries() override;

		virtual void UpdatePrecisePhysics(double time) override;
		virtual void UpdateSimplePhysics(double time) override;
		virtual void UpdateEulerExplicitPhysics(double time)	override;
		virtual void UpdateEulerSemiEXplicitPhysics(double time)	override;
		virtual void UpdateVerletPhysics(double time)	override;
		virtual void UpdateRK4Physics(double time)	override;

	protected:
		Circle3D circle;

		float worldWidth = 100;
		float worldHeight = 100;
		X::Math::Vector3 mGravity;
		X::Math::Vector3 mCurrentDrag;
		X::Math::Vector3 mPreviousDrag;
	
	};

}