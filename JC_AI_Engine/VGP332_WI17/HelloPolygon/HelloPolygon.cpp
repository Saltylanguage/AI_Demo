#include "X\Inc\XEngine.h"

// http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/97/Ian/cutting_ears.html

const float cursorRadius = 8.0f;

enum class Mode
{
	None,
	AddVertex,
	RemoveVertex
};

namespace
{
	struct Polygon
	{
		std::vector<X::Math::Vector2> vertices;
		std::vector<int> indices;
	};
Mode mode = Mode::None;
Polygon polygon;
}


void Triangulate()
{
	// TODO
}

void SetMode()
{
	if (X::IsKeyPressed(X::Keys::ESCAPE))
	{
		mode = Mode::None;
	}
	else if (X::IsKeyPressed(X::Keys::F1))
	{
		mode = Mode::AddVertex;
	}
	else if (X::IsKeyPressed(X::Keys::F2))
	{
		mode = Mode::RemoveVertex;
	}
	else if (X::IsKeyPressed(X::Keys::T))
	{
		Triangulate();
	}
	else if (X::IsKeyPressed(X::Keys::C))
	{
		polygon.vertices.clear();
		polygon.indices.clear();
	}
}

X::Math::Vector2 SnapVertex(const X::Math::Vector2& pos)
{
	for (const auto& v : polygon.vertices)
	{
		if (X::Math::DistanceSqr(v, pos) <= X::Math::Sqr(cursorRadius))
			return v;
	}
	return pos;
}

void UpdateAddVertex()
{
	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		polygon.indices.clear();

		float mouseX = (float)X::GetMouseScreenX();
		float mouseY = (float)X::GetMouseScreenY();
		X::Math::Vector2 pos(mouseX, mouseY);
		polygon.vertices.push_back(SnapVertex(pos));
	}
}

void UpdateRemoveVertex()
{
	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		polygon.indices.clear();

		float mouseX = (float)X::GetMouseScreenX();
		float mouseY = (float)X::GetMouseScreenY();
		X::Math::Vector2 pos(mouseX, mouseY);
		for (auto iter = polygon.vertices.begin(); iter != polygon.vertices.end(); ++iter)
		{
			if (X::Math::DistanceSqr(*iter, pos) <= X::Math::Sqr(cursorRadius))
			{
				polygon.vertices.erase(iter);
				break;
			}
		}
	}
}

void DrawPolygon()
{
	const int vertexCount = (int)polygon.vertices.size();
	const int indexCount = (int)polygon.indices.size();
	for (int i = 0; i < indexCount; i += 3)
	{
		const auto& a = polygon.vertices[polygon.indices[i]];
		const auto& b = polygon.vertices[polygon.indices[i + 1]];
		const auto& c = polygon.vertices[polygon.indices[i + 2]];
		X::DrawScreenLine(a, b, X::Math::Vector4::Gray());
		X::DrawScreenLine(b, c, X::Math::Vector4::Gray());
		X::DrawScreenLine(c, a, X::Math::Vector4::Gray());
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		const auto& v = polygon.vertices[i];
		X::DrawScreenLine(v.x - 2, v.y - 2, v.x + 2, v.y + 2, X::Math::Vector4::Blue());
		X::DrawScreenLine(v.x - 2, v.y + 2, v.x + 2, v.y - 2, X::Math::Vector4::Blue());
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		X::DrawScreenLine(polygon.vertices[i], polygon.vertices[(i + 1) % vertexCount], X::Math::Vector4::Blue());
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		const auto& v = polygon.vertices[i];
		X::DrawScreenLine(v.x - 2, v.y - 2, v.x + 2, v.y + 2, X::Math::Vector4::Blue());
		X::DrawScreenLine(v.x - 2, v.y + 2, v.x + 2, v.y - 2, X::Math::Vector4::Blue());
	}
}

void DrawCursor()
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();

	switch (mode)
	{
	case Mode::None:		
		
		
		X::DrawScreenCircle(mouseX, mouseY, cursorRadius * 0.5f, X::Math::Vector4::Blue());
		break;
	case Mode::AddVertex:
		X::DrawScreenCircle(mouseX, mouseY, cursorRadius, X::Math::Vector4::Green());
		break;
	case Mode::RemoveVertex:
		X::DrawScreenCircle(mouseX, mouseY, cursorRadius, X::Math::Vector4::Red());
		break;
	}
}

void DrawLegend()
{
	const char* const instruction =
		"Add Polygon Vertices in Clockwise Order!\n"
		"F1  - Add Vertex\n"
		"F2  - Remove Vertex\n"
		"T   - Triangulate\n"
		"C   - Clear\n"
		"ESC - Back\n"
		"Q   - Quit";
	X::DrawScreenText(instruction, 10.0f, 10.0f, 20.0f, X::Math::Vector4::White());
}

bool GameLoop(float deltaTime)
{
	SetMode();

	switch (mode)
	{
	case Mode::AddVertex:
		UpdateAddVertex();
		break;
	case Mode::RemoveVertex:
		UpdateRemoveVertex();
		break;
	}

	DrawPolygon();
	DrawCursor();
	DrawLegend();

	return X::IsKeyPressed(X::Keys::Q);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start("Hello Polygon", hInstance, 1280, 720);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}