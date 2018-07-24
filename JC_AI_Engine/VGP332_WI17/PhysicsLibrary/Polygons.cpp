#include "Polygon.h"
#include "Eigen.h"

static double hypoteneuse(double x, double y) {
	return sqrt(x*x + y*y);
}


static void tred2(double V[3][3], double d[3], double e[3])
{
	for (int j = 0; j < 3; j++)
	{
		d[j] = V[3 - 1][j];
	}
}



Polygons::Polygons(X::Math::Vector3 pos, int sides, float_t radius) :
	mPosition(pos),
	mNumVertices(sides),
	mRadius(radius)
{
	CreatePolygon(pos, mNumVertices, mRadius);
}

void Polygons::Update(float deltaTime)
{
	for (uint32_t i = 0; i < mNumVertices; ++i)
	{
		X::Math::Vector3 a = mVertices[i] + mPosition;
		X::Math::Vector3 b = mVertices[(i + 1) % mNumVertices] + mPosition;

		X::DrawScreenLine(a.x, a.y, b.x, b.y, mColor);
	}
}
bool Polygons::UpdateCollision(const Polygons& p2) const
{
	std::vector < X::Math::Vector3> axis;

	for (int i = 0; i < mNumVertices; ++i)
	{
		X::Math::Vector3 newA = X::Math::Normalize(X::Math::Cross(mVertices[(i + 1) % mNumVertices] - mVertices[i], X::Math::Vector3::ZAxis()));

		axis.push_back(newA);
	}

	for (int i = 0; i < p2.mNumVertices; i++)
	{
		X::Math::Vector3 newA = X::Math::Normalize(X::Math::Cross(p2.mVertices[(i + 1) % p2.mNumVertices] - p2.mVertices[i], X::Math::Vector3::ZAxis()));

		axis.push_back(newA);
	}

	for (int i = 0; i < axis.size(); i++)
	{
		float min1 = FLT_MAX;
		float min2 = FLT_MAX;
		float max1 = FLT_MAX;
		float max2 = FLT_MAX;

		for (int j = 0; j < mNumVertices; j++)
		{
			X::Math::Vector3 projected1 = X::Math::Project(mVertices[j] + mPosition, axis[i]);
			float mag = X::Math::Dot(projected1, axis[i]);

			if (mag > max1)
			{
				max1 = mag;
			}
			if (mag < min1)
			{
				min1 = mag;
			}
		}

		X::DrawScreenLine(min1*axis[i].x, min1*axis[i].y, max1*axis[i].x, max1*axis[i].y, X::Math::Vector4::Blue());
		X::DrawScreenLine(min2*axis[i].x, min2*axis[i].y, max2*axis[i].x, max2*axis[i].y, X::Math::Vector4::Blue());

		if (!(max1 > min2 && max2 > min1))
		{
			return false;
		}
	}
	return true;
}

Polygons Polygons::CreateOBB() const
{
	X::Math::Vector3 mean;
	X::Math::Matrix coVarianceMatrix = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	float n = 1.0f / mNumVertices;

	for (int i = 0; i < mNumVertices; ++i)
	{
		mean += mVertices[i];
	}
	mean *= n;


	for (int i = 0; i < mNumVertices; ++i)
	{
		coVarianceMatrix._11 += (mVertices[i].x - mean.x) * (mVertices[i].x - mean.x);
		coVarianceMatrix._22 += (mVertices[i].y - mean.y) * (mVertices[i].y - mean.y);
		coVarianceMatrix._33 += (mVertices[i].z - mean.z) * (mVertices[i].z - mean.z);

		coVarianceMatrix._12 += (mVertices[i].x - mean.x) * (mVertices[i].y - mean.y);
		coVarianceMatrix._13 += (mVertices[i].x - mean.x) * (mVertices[i].z - mean.z);
		coVarianceMatrix._23 += (mVertices[i].y - mean.y) * (mVertices[i].z - mean.z);
	}

	coVarianceMatrix._11 *= n;
	coVarianceMatrix._22 *= n;
	coVarianceMatrix._33 *= n;
	coVarianceMatrix._12 *= n;
	coVarianceMatrix._13 *= n;
	coVarianceMatrix._23 *= n;
	coVarianceMatrix._21 = coVarianceMatrix._12;
	coVarianceMatrix._31 = coVarianceMatrix._13;
	coVarianceMatrix._32 = coVarianceMatrix._23;

	X::Math::Matrix v;
	X::Math::Vector3 d;

	Eigen::eigen_decomposition(coVarianceMatrix, v, d);

	std::vector<X::Math::Vector3> axis;
	axis.push_back(X::Math::Vector3(v._13, v._32, v._33));
	axis.push_back(X::Math::Vector3(v._21, v._22, v._23));
	axis.push_back(X::Math::Vector3(v._11, v._12, v._13));

	float min[3];
	float max[3];

	X::Math::Vector3 e[6];

	for (int j = 0; j < 3; j++)
	{
		min[j] = FLT_MAX;
		max[j] = -FLT_MAX;

		for (int i = 0; i < mVertices.size(); i++)
		{
			float mag = X::Math::Dot(axis[j], mVertices[i] + mPosition);

			if (mag > max[j])
			{
				max[j] = mag;
			}
			if (mag < min[j])
			{
				min[j] = mag;
			}
		}
		e[j * 2] = axis[j] * max[j];
		e[j * 2 + 1] = axis[j] * min[j];

	}

	std::vector<X::Math::Vector3> OBBPoints;
	OBBPoints.push_back(X::Math::Vector3(e[0] + e[2]));
	OBBPoints.push_back(X::Math::Vector3(e[2] + e[1]));
	OBBPoints.push_back(X::Math::Vector3(e[1] + e[3]));
	OBBPoints.push_back(X::Math::Vector3(e[3] + e[0]));

	Polygons mOBB;
	mOBB.mVertices = OBBPoints;
	mOBB.mNumVertices = 4;

	for (int i = 0; i < mOBB.mNumVertices; i++)
	{
		X::DrawScreenLine(mOBB.mVertices[i].x, mOBB.mVertices[i].y,
			mOBB.mVertices[(i + 1) % mOBB.mNumVertices].x,
			mOBB.mVertices[(i + 1) % mOBB.mNumVertices].y,
			X::Math::Vector4::Cyan());
	}

	return mOBB;

}

void Polygons::CreatePolygon(X::Math::Vector3 pos, int sides, float_t radius)
{
	mColor = X::Math::Vector4::White();
	float_t pi = X::Math::kTwoPi;

	for (int i = 0; i < sides; i)
	{
		int x = radius * cos(pi * i / sides);
		int y = radius * sin(pi * i / sides);

		mVertices.push_back(X::Math::Vector3(x, y, 0));
	}
}


