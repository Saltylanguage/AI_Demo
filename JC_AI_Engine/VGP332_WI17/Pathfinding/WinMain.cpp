#include <XEngine.h>
#include <deque>
#include <stack>
#include <queue>
#include <list>

#include "AI_Library\AI_Library.h"
#include "FSM\SCV.h"

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
//AI_Library::Graph myGraph;

//Functor to Calculate Cost
AI_Library::PathingCalculator* TMCalculator = new AI_Library::TileMapCalculator();

//Rendered Object Lists
std::vector<AI_Library::Coord> path;
std::vector<AI_Library::Coord> closedList;
std::list<X::Math::Vector2> actualPath;

std::vector<AI_Library::Coord> obstacleList;
std::vector<AI_Library::Coord> grassTileList;
std::vector<AI_Library::Coord> sandTileList;
std::vector<AI_Library::Coord> waterTileList;

bool isDraw = false;

GameState state = GameState::Init;
AI_Library::Pathfinder mPathfinder;

//End of Data Members

//Main Game Loop
bool GameLoop(float deltaTime);

//Pathfinding Functions

void SetStartCoord();
void SetEndCoord();

//Pathfinding Helper Functions

float(*CalculateHeuristic)(float x0, float y0, float x1, float y1);

//Draw Functions
void DrawCursor(int textureId);
void DrawGrid(int cols, int rows, float cellSize);
void DrawPath(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color);
void DrawSearchPattern(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color);

SearchMode searchMode = SearchMode(4);

int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	X::SetTexturePath("../Data/Images");

	X::Start("Pathfinding", hInstance, 1024, 768);
	X::Run(GameLoop);
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

		spriteTextureIDs[0] = X::LoadTexture("scv_01.png");
		spriteTextureIDs[1] = X::LoadTexture("scv_02.png");
		spriteTextureIDs[2] = X::LoadTexture("scv_03.png");
		spriteTextureIDs[3] = X::LoadTexture("scv_04.png");
		spriteTextureIDs[4] = X::LoadTexture("scv_05.png");
		spriteTextureIDs[5] = X::LoadTexture("scv_06.png");
		spriteTextureIDs[6] = X::LoadTexture("scv_07.png");
		spriteTextureIDs[7] = X::LoadTexture("scv_08.png");
		spriteTextureIDs[8] = X::LoadTexture("scv_09.png");
		spriteTextureIDs[9] = X::LoadTexture("scv_10.png");
		spriteTextureIDs[10] = X::LoadTexture("scv_11.png");
		spriteTextureIDs[11] = X::LoadTexture("scv_12.png");
		spriteTextureIDs[12] = X::LoadTexture("scv_13.png");
		spriteTextureIDs[13] = X::LoadTexture("scv_14.png");
		spriteTextureIDs[14] = X::LoadTexture("scv_15.png");
		spriteTextureIDs[15] = X::LoadTexture("scv_16.png");

		grassTextureID = X::LoadTexture("green.jpg");
		sandTextureID = X::LoadTexture("yellow.jpg");
		waterTextureID = X::LoadTexture("blue.jpg");

		mPathfinder.mGraph.mGrid = mPathfinder.mGraph.InitializeGrid(10, 10);
		
		//mPathfinder.SetHeuristic(1);
	
		state = GameState::Update;
	}
	break;
	case GameState::Update:
	{
		DrawGrid(mPathfinder.mGraph.GetColumns(), mPathfinder.mGraph.GetRows(), cellSize);
		DrawCursor(cursorTextureID);

		int currentMouseX = X::GetMouseScreenX();
		int currentMouseY = X::GetMouseScreenY();

		if (X::IsMouseDown(X::Mouse::LBUTTON) && !X::IsKeyDown(X::Keys::LSHIFT))
		{
			SetStartCoord();
		}
		if (X::IsMouseDown(X::Mouse::RBUTTON) && !X::IsKeyDown(X::Keys::LSHIFT))
		{
			path.clear();
			initPath = true;
			SetEndCoord();
			path = mPathfinder.FindPath(mPathfinder.searchMode, mPathfinder.mGraph, AI_Library::Coord((int)position.x / (int)cellSize, (int)position.y / (int)cellSize), AI_Library::Coord((int)endGrid_X, (int)endGrid_Y));
		}

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

		//BREADTH FIRST
		if (X::IsKeyDown(X::Keys::F1))
		{
			mPathfinder.searchMode = AI_Library::Pathfinder::SearchMode::Breadth;
		}
		//DEPTH FIRST
		if (X::IsKeyDown(X::Keys::F2))
		{
			mPathfinder.searchMode = AI_Library::Pathfinder::SearchMode::Depth;
		}
		//DIJKSTRA
		if (X::IsKeyDown(X::Keys::F3))
		{
			mPathfinder.searchMode = AI_Library::Pathfinder::SearchMode::Dijkstra;
		}
		// A*
		if (X::IsKeyDown(X::Keys::F4))
		{
			mPathfinder.searchMode = AI_Library::Pathfinder::SearchMode::A_Star;
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

		if (X::IsMousePressed(X::Mouse::LBUTTON) && X::IsKeyDown(X::Keys::LSHIFT))
		{
			if (tileID == 0)
			{
				tile_X = (int)X::GetMouseScreenX() / (int)cellSize;
				tile_Y = (int)X::GetMouseScreenY() / (int)cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].isPassable = false;
				obstacleList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 1)
			{
				(int)tile_X = X::GetMouseScreenX() / (int)cellSize;
				(int)tile_Y = X::GetMouseScreenY() / (int)cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 1;
				grassTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 2)
			{
				(int)tile_X = X::GetMouseScreenX() / (int)cellSize;
				(int)tile_Y = X::GetMouseScreenY() / (int)cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 2;
				sandTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
			if (tileID == 3)
			{
				(int)tile_X = X::GetMouseScreenX() / (int)cellSize;
				(int)tile_Y = X::GetMouseScreenY() / (int)cellSize;
				mPathfinder.mGraph.mGrid[mPathfinder.mGraph.GetIndex(tile_X, tile_Y)].type = 3;
				waterTileList.push_back(AI_Library::Coord(tile_X, tile_Y));
			}
		}


		if (!path.empty()) // as long as stuffs in the list
		{
			X::Math::Vector2 nextPos;

			if (initPath)   // set temp path once
			{
				actualPath.clear();
				for (uint32_t i = 0; i < path.size(); ++i)
				{
					X::Math::Vector2 newPos;
					newPos.x = (float)path[i].x * cellSize + (cellSize / 2.0f);
					newPos.y = (float)path[i].y *cellSize + (cellSize / 2.0f);
					actualPath.push_back(newPos);
				}
				std::reverse(actualPath.begin(), actualPath.end());
				actualPath.pop_front();
				initPath = false; // never do it again
			}

			if (!actualPath.empty())
			{
				destination = actualPath.back();

				X::Math::Vector2 next;
				next = actualPath.front();

				if (X::Math::Distance(next, position) < 5.0f)
				{
					if (!actualPath.empty())
					{
						actualPath.pop_front();
					}
				}
				else
				{
					float dist = X::Math::Distance(position, next);  // set buffer distance

					float speed = 100;
					if (dist > 0.1f)
					{
						X::Math::Vector2 direction = X::Math::Normalize(next - position);
						position += direction * speed * deltaTime;
						angle = atan2(-direction.x, direction.y) + X::Math::kPi;
					}
					else
					{
						if (!actualPath.empty())
						{
							actualPath.pop_front();
						}
					}
				}
			}

			int frame = (int)(angle / X::Math::kTwoPi * 16.0f) % 16;
			X::DrawSprite(spriteTextureIDs[frame], position - X::Math::Vector2(32.0f, 32.0f));
			DrawPath(path, X::Math::Vector4::Red());
			//X::DrawScreenLine(position, destination, X::Math::Vector4::Green());
			X::DrawScreenCircle(destination, 20.0f, X::Math::Vector4::Red());
			if (mPathfinder.isDraw)
			{
				DrawSearchPattern(mPathfinder.closedList, X::Math::Vector4::Green());
			}
		}
	}

	break;
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

//

void DrawSearchPattern(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color)
{
	for (uint32_t i = 0; i < nodes.size(); i++)
	{
		X::DrawScreenCircle(X::Math::Vector2((nodes[i].x) * cellSize + (cellSize / 2.0f), (nodes[i].y) * cellSize + (cellSize / 2.0f)), 25, color);
	}
}


void DrawPath(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color)
{
	std::vector<X::Math::Vector2> path;
	for (uint32_t i = 0; i < nodes.size(); i++)
	{
		path.push_back(X::Math::Vector2(nodes[i].x * cellSize + (cellSize / 2.0f), nodes[i].y * cellSize + (cellSize / 2.0f)));
	}

	for (uint32_t i = 0; i < nodes.size() - 1; i++)
	{
		X::DrawScreenLine(path[i], path[i + 1], color);
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

	for (uint32_t i = 0; i < grassTileList.size(); i++)
	{
		X::DrawSprite(grassTextureID, X::Math::Vector2((grassTileList[i].x) * cellSize, (grassTileList[i].y) * cellSize));
	}
	for (uint32_t i = 0; i < sandTileList.size(); i++)
	{
		X::DrawSprite(sandTextureID, X::Math::Vector2((sandTileList[i].x) * cellSize, (sandTileList[i].y) * cellSize));
	}
	for (uint32_t i = 0; i < waterTileList.size(); i++)
	{
		X::DrawSprite(waterTextureID, X::Math::Vector2((waterTileList[i].x) * cellSize, (waterTileList[i].y) * cellSize));
	}
	for (uint32_t i = 0; i < obstacleList.size(); i++)
	{
		X::DrawScreenText("X", (obstacleList[i].x + 0.25f) * cellSize, (obstacleList[i].y - 0.1f) * cellSize, cellSize, X::Math::Vector4::Red());
	}
}

void SetStartCoord()
{
	startGrid_X = (int)X::GetMouseScreenX() / (int)cellSize;
	startGrid_Y = (int)X::GetMouseScreenY() / (int)cellSize;
}

void SetEndCoord()
{
	endGrid_X = (int)X::GetMouseScreenX() / (int)cellSize;
	endGrid_Y = (int)X::GetMouseScreenY() / (int)cellSize;
}







//Research items:
// SPT (Shortest path tree)
// edge relaxation
