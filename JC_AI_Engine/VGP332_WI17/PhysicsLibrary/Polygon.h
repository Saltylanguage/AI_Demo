#pragma once
#include <X\Inc\XEngine.h>


class Polygons
{
public:
	Polygons() {}
	Polygons(X::Math::Vector3 pos, int sides, float_t radius);
	~Polygons() {}

	bool UpdateCollision(const Polygons& p2) const;
	void Update(float deltaTime);
	void CreatePolygon(X::Math::Vector3 pos, int sides, float_t radius);
	Polygons CreateOBB() const;

	void SetPosition(X::Math::Vector3 pos) { mPosition = pos; }
	X::Math::Vector3 GetPosition() { return mPosition; }
	void SetColor(X::Math::Vector4 c) { mColor = c; }

protected:
	std::vector<X::Math::Vector3> mVertices;
	X::Math::Vector3 mPosition;
	int mNumVertices;
	X::Math::Vector4 mColor;
	float_t mRadius;
};