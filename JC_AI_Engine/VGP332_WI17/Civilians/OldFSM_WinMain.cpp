#include <XEngine.h>
#include <deque>
#include <stack>
#include <queue>
#include <list>
#include "SCV.h"

enum class SearchMode
{
	Breadth = 1,
	Depth,
	Dijkstra,
	A_Star
};

enum class GameState
{
	Init,
	Update
};

//Data Members
float cellSize = 64;

int cursorTextureID;

int grassTextureID;
int sandTextureID;
int waterTextureID;

int mineralTextureID;

int tileID = 1;
int spriteTextureIDs[16];

X::Math::Vector2 position;
X::Math::Vector2 destination;

float angle = 0.0f;

int startGrid_X;
int startGrid_Y;

int endGrid_X;
int endGrid_Y;

int tile_X;
int tile_Y;

bool initPath = true;

//Main Graph
AI_Library::Pathfinder mPathfinder;

SCV mSCV;

//Main Game Loop
bool GameLoop(float deltaTime);							// NEED

//Functor to Calculate Cost

//Rendered Object Lists
std::vector<AI_Library::Coord> obstacleList;			
std::vector<AI_Library::Coord> grassTileList;			
std::vector<AI_Library::Coord> sandTileList;			
std::vector<AI_Library::Coord> waterTileList;			

std::vector<AI_Library::Coord> mineralTileList;

GameState state = GameState::Init;						// NEED

//End of Data Members

//Pathfinding Functions
void SetStartCoord();
void SetEndCoord();

//Draw Functions
void DrawCursor(int textureId);
void DrawGrid(int cols, int rows, float cellSize);

SearchMode searchMode = SearchMode(4);


int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	X::SetTexturePath("../Data/Images");

	X::Start("Pathfinding", hInstance, 1024, 768);
	X::Run(GameLoop);

	mSCV.mStateMachine.Purge();
	X::Stop();

	return 0;
}

bool GameLoop(float deltaTime)
{
	switch (state)
	{
	case GameState::Init:
	{
		cursorTextureID = X::LoadTexture("sword.png");

		grassTextureID = X::LoadTexture("green.jpg");
		sandTextureID = X::LoadTexture("yellow.jpg");
		waterTextureID = X::LoadTexture("blue.jpg");

		mineralTextureID = X::LoadTexture("minerals.png");

		mPathfinder.mGraph.mGrid = mPathfinder.mGraph.InitializeGrid(10, 10);

		mSCV.Initialize();

		state = GameState::Update;
	}
	break;

	case GameState::Update:
	{
		DrawGrid(mPathfinder.mGraph.GetColumns(), mPathfinder.mGraph.GetRows(), cellSize);
		DrawCursor(cursorTextureID);

		mSCV.Update(deltaTime);

		int currentMouseX = X::GetMouseScreenX();
		int currentMouseY = X::GetMouseScreenY();

		if (X::IsKeyPressed(X::Keys::ZERO))
		{
			tileID = 0;
		}
		if (X::IsKeyPressed(X::Keys::ONE))
		{
			tileID = 1;

		}
		if (X::IsKeyPressed(X::Keys::TWO))
		{
			tileID = 2;

		}
		if (X::IsKeyPressed(X::Keys::THREE))
		{
			tileID = 3;
		}

		if (X::IsMousePressed(X::Mouse::LBUTTON) && X::IsKeyDown(X::Keys::LSHIFT))
		{
			if (tileID == 0)
			{
				tile_X = X::GetMouseScreenX() / cellSize;
				tile_Y = X::GetMouseScreenY() / cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].isPassable = false;
				obstacleList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 1)
			{
				tile_X = X::GetMouseScreenX() / cellSize;
				tile_Y = X::GetMouseScreenY() / cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 1;
				grassTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 2)
			{
				tile_X = X::GetMouseScreenX() / cellSize;
				tile_Y = X::GetMouseScreenY() / cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 2;
				sandTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 3)
			{
				tile_X = X::GetMouseScreenX() / cellSize;
				tile_Y = X::GetMouseScreenY() / cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 3;

				mineralTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
				mSCV.MineralLocations.push_back(X::Math::Vector2(tile_X, tile_Y));

				//waterTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}

			//BREADTH FIRST
			if (X::IsKeyDown(X::Keys::F1))
			{
				searchMode = SearchMode::Breadth;
			}
			//DEPTH FIRST
			if (X::IsKeyDown(X::Keys::F2))
			{
				searchMode = SearchMode::Depth;
			}
			//DIJKSTRA
			if (X::IsKeyDown(X::Keys::F3))
			{
				searchMode = SearchMode::Dijkstra;
			}
			// A*
			if (X::IsKeyDown(X::Keys::F4))
			{
				searchMode = SearchMode::A_Star;
			}

			if (X::IsKeyPressed(X::Keys::F5))
			{
				if (!mPathfinder.isDraw)
				{
					mPathfinder.isDraw = true;
				}
				else if (mPathfinder.isDraw)
				{
					mPathfinder.isDraw = false;
				}

			}
		}
	}
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void DrawSearchPattern(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		X::DrawScreenCircle(X::Math::Vector2((nodes[i].x) * cellSize + (cellSize / 2.0f), (nodes[i].y) * cellSize + (cellSize / 2.0f)), 25, color);
	}
}

void DrawCursor(int textureId)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	X::DrawSprite(textureId, X::Math::Vector2(mouseX, mouseY));

}

void DrawGrid(int cols, int rows, float cellSize)
{
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			float left = x * cellSize;
			float top = y * cellSize;
			float right = (x + 1) * cellSize;
			float bottom = (y + 1) * cellSize;

			X::DrawScreenRect(left, top, right, bottom, X::Math::Vector4::DarkGray());
		}
	}

	for (int i = 0; i < grassTileList.size(); i++)
	{
		X::DrawSprite(grassTextureID, X::Math::Vector2((grassTileList[i].x) * cellSize, (grassTileList[i].y) * cellSize));
	}
	for (int i = 0; i < sandTileList.size(); i++)
	{
		X::DrawSprite(sandTextureID, X::Math::Vector2((sandTileList[i].x) * cellSize, (sandTileList[i].y) * cellSize));
	}
	for (int i = 0; i < mineralTileList.size(); i++)
	{
		X::DrawSprite(mineralTextureID, X::Math::Vector2((mineralTileList[i].x) * cellSize, (mineralTileList[i].y) * cellSize));
	}
	//for (int i = 0; i < waterTileList.size(); i++)
	//{
	//	X::DrawSprite(waterTextureID, X::Math::Vector2((waterTileList[i].x) * cellSize, (waterTileList[i].y) * cellSize));
	//}
	for (int i = 0; i < obstacleList.size(); i++)
	{
		X::DrawScreenText("X", (obstacleList[i].x + 0.25f) * cellSize, (obstacleList[i].y - 0.1f) * cellSize, cellSize, X::Math::Vector4::Red());
	}
}

void SetStartCoord()
{
	startGrid_X = X::GetMouseScreenX() / cellSize;
	startGrid_Y = X::GetMouseScreenY() / cellSize;
}

void SetEndCoord()
{
	endGrid_X = X::GetMouseScreenX() / cellSize;
	endGrid_Y = X::GetMouseScreenY() / cellSize;
}


float CalculateEuclidian(float x0, float y0, float x1, float y1)
{
	float xDist = pow(x0 - x1, 2);
	float yDist = pow(y0 - y1, 2);
	float result = sqrt(xDist + yDist);
	return result;
}

float CalculateManhatten(float x0, float y0, float x1, float y1)
{
	float deltaX = x0 - x1;
	float deltaY = y0 - y1;

	if (deltaX < 0)
	{
		deltaX *= -1;
	}
	if (deltaY < 0)
	{
		deltaY *= -1;
	}

	float result = deltaX + deltaY;
	if (deltaX > 0 && deltaY > 0)
	{
		result *= 1.4f;
	}

	return result;
}

//Homework:

//Implement:

//Seek & Flee
//Arrive
//Wander
//Obstacle Avoidance




