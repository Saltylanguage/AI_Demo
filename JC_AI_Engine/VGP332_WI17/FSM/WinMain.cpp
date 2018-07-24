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
int mineralTextureId;


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
std::vector<AI_Library::Coord> obstacleList;
std::vector<AI_Library::Coord> grassTileList;
std::vector<AI_Library::Coord> sandTileList;
std::vector<AI_Library::Coord> waterTileList;

std::vector<AI_Library::Coord> mineralSites;
std::vector<AI_Library::Coord> storageSites;

bool isDraw = false;

GameState state = GameState::Init;
//AI_Library::Pathfinder mPathfinder;

SCV mSCV;

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
void DrawMinerals();
void DrawRefinery();
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

			mineralTextureId = X::LoadTexture("minerals.png");

			mSCV.mPathfinder.mGraph.mGrid = mSCV.mPathfinder.mGraph.InitializeGrid(10, 10);

			mineralSites.push_back(AI_Library::Coord(5, 5));
			storageSites.push_back(AI_Library::Coord(9, 9));

			mSCV.SCV_TargetDestination = X::Math::Vector2(5 * cellSize, 5 * cellSize);
			mSCV.Initialize();

			state = GameState::Update;
		}
		break;
		case GameState::Update:
		{
			DrawGrid(mSCV.mPathfinder.mGraph.GetColumns(), mSCV.mPathfinder.mGraph.GetRows(), cellSize);
			DrawCursor(cursorTextureID);
			DrawPath(mSCV.SCV_Path, X::Math::Vector4::Blue());
			DrawMinerals();
			DrawRefinery();


			int currentMouseX = X::GetMouseScreenX();
			int currentMouseY = X::GetMouseScreenY();

			mSCV.Update(deltaTime);
			mSCV.Render();
			X::DrawScreenLine(mSCV.SCV_Position, mSCV.SCV_NextDestination, X::Math::Vector4::Green());



		}

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

	if (nodes.size() >= 2)
	{
		for (uint32_t i = 0; i < nodes.size() - 1; i++)
		{
			X::DrawScreenLine(path[i], path[i + 1], color);
		}
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



void DrawMinerals()
{
	for (uint32_t i = 0; i < mineralSites.size(); i++)
	{
		X::DrawSprite(mineralTextureId, X::Math::Vector2((mineralSites[i].x) * 64.0f, (mineralSites[i].y) * 64.0f));
	}
}

void DrawRefinery()
{
	for (uint32_t i = 0; i < storageSites.size(); i++)
	{
		X::DrawSprite(mSCV.storageTextureId[mSCV.refineryFrame], X::Math::Vector2((storageSites[i].x) * (float)64 - 32.0f, (storageSites[i].y) * (float)64 - 32.0f));
	}
}

//Research items:
// SPT (Shortest path tree)
// edge relaxation
