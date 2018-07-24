#pragma once
#include "..\X\Inc\XEngine.h"


namespace Physics
{
	class RigidBody
	{
	public:

		struct State
		{
			State() {}
			State(X::Math::Vector3 pos, X::Math::Vector3 vel)
				: position(pos), velocity(vel) {}

			X::Math::Vector3 position;
			X::Math::Vector3 velocity;
		};

		struct Derivative
		{
			X::Math::Vector3 dx; //dx/dt = velocity
			X::Math::Vector3 dv; //dv/dt = acceleration
		};

		virtual void CheckEdgeBoundaries() = 0;
		virtual void VerletCheckEdgeBoundaries() = 0;

		Derivative Evaluate(const State& initial, float dt, const Derivative& d)
		{
			State state;
			state.position = initial.position + d.dx * dt;
			state.velocity = initial.velocity + d.dv * dt;

			Derivative output;
			output.dx = state.velocity;
			output.dv = X::Math::Vector3(0.0f, -9.81, 0.0f);

			return output;
		}

		void Integrate(State& state, float dt)
		{
			Derivative a;
			Derivative b;
			Derivative c;
			Derivative d;

			a = Evaluate(state, 0.0f, Derivative());
			b = Evaluate(state, dt * 0.5f, a);
			c = Evaluate(state, dt * 0.5f, b);
			d = Evaluate(state, dt, c);

			X::Math::Vector3 dxdt = (a.dx + (b.dx + c.dx) * 2.0 + d.dx) * (1.0f / 6.0f);
			X::Math::Vector3 dvdt = (a.dv + (b.dv + c.dv) * 2.0 + d.dv) * (1.0f / 6.0f);

			state.position += dxdt * dt;
			state.velocity += dvdt * dt;
		}
		virtual void UpdatePrecisePhysics(double timeInc) = 0;
		virtual void UpdateSimplePhysics(double timeInc) = 0;
		virtual void UpdateEulerExplicitPhysics(double time) = 0;
		virtual void UpdateEulerSemiEXplicitPhysics(double time) = 0;
		virtual void UpdateVerletPhysics(double time) = 0;
		virtual void UpdateRK4Physics(double time) = 0;

		struct Bounds
		{
			X::Math::Vector3 min = X::Math::Vector3(0, 0, 0);
			X::Math::Vector3 max = X::Math::Vector3(100, 100, 100);
		};

	protected:
		X::Math::Vector3 mPosition;
		X::Math::Vector3 mLastPosition;
		X::Math::Vector3 mVelocity;
		double mRadius;
		double mMass;
		float WorldWidth = 100;
		float WorldHeight = 100;
		X::Math::Vector3 mGravity = X::Math::Vector3(0.0f, -9.81, 0.0f);

	};

} // Namespace: Physics 