#include <XEngine.h>

X::Math::Vector3 cameraPosition = X::Math::Vector3::YAxis();
X::Math::Vector3 cameraDirection = X::Math::Vector3::ZAxis();
float cameraElevation = 0.0f;

// TODO: Add a Transform structure (move/strafe/rise, rotation, draw)

bool GameLoop(float deltaTime)
{
	const float moveSpeed = X::IsKeyDown(X::Keys::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 45.0f;

	X::Math::Vector3 cameraRight = X::Math::Cross(X::Math::Vector3::YAxis(), cameraDirection);
	X::Math::Matrix yaw = X::Math::Matrix::RotationY(X::GetMouseMoveX() * turnSpeed * X::Math::kDegToRad * deltaTime);
	cameraDirection = X::Math::TransformNormal(cameraDirection, yaw);
	cameraElevation = X::Math::Clamp(cameraElevation + (X::GetMouseMoveY() * turnSpeed * X::Math::kDegToRad * deltaTime), -1.5f, 1.5f);

	X::Math::Matrix pitch = X::Math::MatrixRotationAxis(cameraRight, cameraElevation);
	X::Math::Vector3 viewDirection = X::Math::TransformNormal(cameraDirection, pitch);

	if (X::IsKeyDown(X::Keys::W))
	{
		cameraPosition += viewDirection * moveSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		cameraPosition -= viewDirection * moveSpeed * deltaTime;
	}
	if (X::IsKeyDown(X::Keys::D))
	{
		cameraPosition += cameraRight * moveSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::A))
	{
		cameraPosition -= cameraRight * moveSpeed * deltaTime;
	}

	X::SetCameraPosition(cameraPosition);
	X::SetCameraDirection(viewDirection);

	for (int z = -50; z <= 50; ++z)
	{
		X::DrawLine(-50.0f, 0.0f, (float)z, 50.0f, 0.0f, (float)z, X::Math::Vector4::DarkGray());
	}
	for (int x = -50; x <= 50; ++x)
	{
		X::DrawLine((float)x, 0.0f, -50.0f, (float)x, 0.0f, 50.0f, X::Math::Vector4::DarkGray());
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start("Hello X", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}