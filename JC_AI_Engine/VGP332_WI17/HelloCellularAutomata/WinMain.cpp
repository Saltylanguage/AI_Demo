#include <XEngine.h>

const int screenWidth = 1280;
const int screenHeight = 720;
const int cellSize = 10;
const int columns = screenWidth / cellSize;
const int rows = screenHeight / cellSize;
const float randomFillPercent = 0.1f;

class Cell
{
public:
	Cell()
		: previous(false)
		, current(false)
	{}

	void Update() { previous = current; }
	bool IsAlive() const { return current; }
	bool WasAlive() const { return previous; }
	void Spawn() { current = true; }
	void Kill() { current = false; }

	X::Math::Vector4 Color() const
	{
		if (current && !previous)
			return X::Math::Vector4::Red();
		else if (current && previous)
			return X::Math::Vector4::Green();
		else
			return X::Math::Vector4::Blue();
	}

	bool previous;
	bool current;
};

Cell* grid = nullptr;
float timer = 0.0f;

void InitGrid()
{
	grid = new Cell[columns * rows];
}

void CleanUpGrid()
{
	X::SafeDeleteArray(grid);
}

void FillRandomGrid(float fillPercent)
{
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			int index = x + (y * columns);
			if (X::RandomFloat() < fillPercent)
				grid[index].Spawn();
		}
	}
}

int GetNeighborCount(int x, int y)
{
	int count = 0;
	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			int xx = i;
			int yy = j;
			if (i < 0) xx = columns - 1;
			if (j < 0) yy = rows - 1;
			if (i >= columns) xx = 0;
			if (j >= rows) yy = 0;

			if (grid[xx + (yy * columns)].WasAlive())
			{
				++count;
			}
		}
	}
	if (grid[x + (y * columns)].WasAlive())
		--count;
	return count;
}

void UpdateGrid()
{
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			int index = x + (y * columns);
			grid[index].Update();
		}
	}

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			int neighbors = GetNeighborCount(x, y);
			int index = x + (y * columns);

			// Death
			if (grid[index].WasAlive() && neighbors < 2)
			{
				grid[index].Kill();
			}
			else if (grid[index].WasAlive() && neighbors > 3)
			{
				grid[index].Kill();
			}
			else if (!grid[index].WasAlive() && neighbors == 3)
			{
				grid[index].Spawn();
			}
		}
	}
}

void DrawGrid()
{
	for (int y = 0; y < screenHeight; y += cellSize)
	{
		X::DrawScreenLine(0.0f, (float)y, (float)screenWidth, (float)y, X::Math::Vector4::DarkGray());
	}
	for (int x = 0; x < screenWidth; x += cellSize)
	{
		X::DrawScreenLine((float)x, 0.0f, (float)x, (float)screenHeight, X::Math::Vector4::DarkGray());
	}
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			int index = x + (y * columns);
			if (grid[index].IsAlive() || grid[index].WasAlive())
			{
				float cellX = (float)x * cellSize;
				float cellY = (float)y * cellSize;
				X::Math::Vector4 color = grid[index].Color();
				X::DrawScreenLine(cellX, cellY, cellX + cellSize, cellY + cellSize, color);
				X::DrawScreenLine(cellX, cellY + cellSize, cellX + cellSize, cellY, color);
			}
		}
	}
}

bool GameLoop(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		FillRandomGrid(randomFillPercent);
	}

	timer -= deltaTime;
	if (timer <= 0.0f)
	{
		timer = 0.1f;
		UpdateGrid();
	}

	DrawGrid();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitGrid();
	FillRandomGrid(randomFillPercent);

	X::Start("Hello Cellular Automata", hInstance, screenWidth, screenHeight);
	X::Run(GameLoop);
	X::Stop();

	CleanUpGrid();

	return 0;
}