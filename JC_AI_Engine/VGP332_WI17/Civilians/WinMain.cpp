// Engine Library
#include <XEngine.h>

// My Libraries
#include "AI_Library\AI_Library.h"

//std Files
#include <deque>
#include <list>
#include <queue>
#include <stack>

// Game Layer Files
#include "Animal.h"
#include "Bed.h"
#include "Civ.h"
#include "Floor.h"
#include "Resource.h"
#include "Tree.h"
#include "Wall.h"
#include "GameWorld.h"


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
GameWorld mainGameWorld;

float cellSize = 64;

int cursorTextureID;

int grassTextureID;

int floorTextureID;
int wallTextureID;
int stockpileTextureID;

int bedTextureID;
int treeTextureID;

int foodTextureID;
int woodTextureID;

int sleepTextureID;

int startGrid_X;
int startGrid_Y;

int endGrid_X;
int endGrid_Y;

bool initPath = true;

//Main Graph
//AI_Library::Graph myGraph;

//Functor to Calculate Cost
AI_Library::PathingCalculator* TMCalculator = new AI_Library::TileMapCalculator();

std::vector<AI_Library::Coord> storageSites;

bool isDraw = false;

GameState state = GameState::Init;

Civ mCiv;
Civ mCiv2;
SearchMode searchMode = SearchMode(4);
//End of Data Members

X::Math::Vector2 centerOffset = X::Math::Vector2(32, 32);

void Initialize();

//Main Game Loop
bool GameLoop(float deltaTime);

//Pathfinding Functions
void SetStartCoord();
void SetEndCoord();

//Pathfinding Helper Functions
float(*CalculateHeuristic)(float x0, float y0, float x1, float y1);

//Draw Functions
void DrawCursor(int textureId);
void DrawBackground();
void DrawGrid(int cols, int rows, float cellSize);
void DrawPath(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color);
void DrawSearchPattern(std::vector<AI_Library::Coord> nodes, X::Math::Vector4 color);
void DrawFloors();
void DrawWalls();
void DrawResources();
void DrawBeds();

void RenderWorld();

//===================================================================================================
//	MAIN
//===================================================================================================
int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	X::SetTexturePath("../Data/Images");

	X::Start("Pathfinding", hInstance, 1224, 768);
	X::Run(GameLoop);

	mCiv.mStateMachine.Purge();

	X::Stop();

	

	return 0;
}

//===================================================================================================
//	GAME LOOP
//===================================================================================================

bool GameLoop(float deltaTime)
{
	switch (state)
	{
	case GameState::Init:
	{
		Initialize();
		state = GameState::Update;
	}
	break;
	case GameState::Update:
	{
		int currentMouseX = X::GetMouseScreenX();
		int currentMouseY = X::GetMouseScreenY();

		mCiv.Update(deltaTime);
		mainGameWorld.Update(deltaTime);

		char treeSize[50];
		char resourceSize[50];
		sprintf_s(treeSize, 50, "Tree Size: %d", mainGameWorld.mTrees.size());
		sprintf_s(resourceSize, 50, "Resource Size: %d", mainGameWorld.mResources.size());

		X::DrawScreenText(treeSize, 700, 450, 16, X::Math::Vector4::Green());
		X::DrawScreenText(resourceSize, 700, 500, 16, X::Math::Vector4::Green());

		RenderWorld();
		mCiv.Render();
		DrawResources();

	}

	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

//===================================================================================================
//	GAME LOOP ENDS HERE
//===================================================================================================


//===================================================================================================
//	INITIALIZE
//===================================================================================================


void Initialize()
{

	mainGameWorld.mGraph = &mCiv.mPathfinder.mGraph;

	cursorTextureID = X::LoadTexture("sword.png");
	grassTextureID = X::LoadTexture("myGrass.png");

	floorTextureID = X::LoadTexture("CivImages/WoodFloor.png");
	wallTextureID = X::LoadTexture("CivImages/WoodWall.png");
	stockpileTextureID = X::LoadTexture("Grey.jpg");

	bedTextureID = X::LoadTexture("CivImages/Bed.png");
	treeTextureID = X::LoadTexture("CivImages/Tree.png");

	foodTextureID = X::LoadTexture("CivImages/Food.png");
	woodTextureID = X::LoadTexture("CivImages/Wood.png");

	sleepTextureID = X::LoadTexture("CivImages/Sleep.png");

	mCiv.mPathfinder.mGraph.mGrid = mCiv.mPathfinder.mGraph.InitializeGrid(10, 10);

	for (int i = 0; i < mCiv.mPathfinder.mGraph.GetColumns(); i++)
	{
		for (int j = 0; j < mCiv.mPathfinder.mGraph.GetRows(); j++)
		{
			mainGameWorld.backgroundTiles.push_back(X::Math::Vector2(i, j));
		}
	}

	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(5, 8)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(6, 7)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(6, 8)));

	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(7, 5)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(7, 6)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(7, 7)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(7, 8)));

	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(8, 5)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(8, 6)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(8, 7)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(8, 8)));

	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(9, 5)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(9, 6)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(9, 7)));
	mainGameWorld.mTrees.push_back(Tree(treeTextureID, 10, X::Math::Vector2(9, 8)));

	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(3, 3)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(3, 4)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(3, 5)));

	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(4, 3)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(4, 4)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(4, 5)));

	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(5, 3)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(5, 4)));
	mainGameWorld.mFloors.push_back(Floor(X::Math::Vector2(5, 5)));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(2, 2) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(2, 3) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(2, 4) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(2, 5) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(2, 6) * 64));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(3, 2) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(3, 6) * 64));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(4, 2) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(4, 6) * 64));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(5, 2) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(5, 6) * 64));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(6, 2) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(6, 3) * 64));

	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(6, 5) * 64));
	mainGameWorld.mWallBuildSites.push_back((X::Math::Vector2(6, 6) * 64));

	mainGameWorld.mBeds.push_back(Bed(X::Math::Vector2(3, 3) * 64));

	mainGameWorld.stockpileTiles.push_back(X::Math::Vector2(9, 0));

	mCiv.Initialize(&mainGameWorld);
}


//===================================================================================================
//	PATHFINDING HELPER FUNCTIONS
//===================================================================================================

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


//===================================================================================================
//	DRAW FUNCTIONS
//===================================================================================================

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
			//X::DrawScreenLine(path[i], path[i + 1], color);
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
}



void DrawBeds()
{
	for (uint32_t i = 0; i < mainGameWorld.mBeds.size(); i++)
	{
		if (mainGameWorld.mResources.empty() && mainGameWorld.mTrees.empty())
		{
			X::DrawSprite(mainGameWorld.mBeds[i].textureID, mainGameWorld.mBeds[i].mPosition);
		}
	}
}

void DrawSleep()
{
	if (mainGameWorld.isAsleep)
	{
		X::DrawSprite(sleepTextureID, mainGameWorld.mBeds.back().mPosition + X::Math::Vector2(32, -64));
	}
}

void DrawResources()
{
	if (!mainGameWorld.mResources.empty())
	{
		for (uint32_t i = 0; i < mainGameWorld.mResources.size(); i++)
		{
			X::DrawSprite(mainGameWorld.mResources[i].resourceTextureID, mainGameWorld.mResources[i].mPosition);
		}
	}
}

void DrawWalls()
{
	for (uint32_t i = 0; i < mainGameWorld.mWalls.size(); i++)
	{
		X::DrawSprite(floorTextureID, X::Math::Vector2((mainGameWorld.mWalls[i].mPosition.x) * 64.0f, (mainGameWorld.mWalls[i].mPosition.y) * 64.0f));
	}
}

void DrawFloors()
{
	if (mainGameWorld.mResources.empty() && mainGameWorld.mTrees.empty())
	{
		for (uint32_t i = 0; i < mainGameWorld.mFloors.size(); i++)
		{
			X::DrawSprite(wallTextureID, X::Math::Vector2((mainGameWorld.mFloors[i].mPosition.x) * 64, (mainGameWorld.mFloors[i].mPosition.y) * 64));
		}
	}
}

void DrawTrees()
{
	for (uint32_t i = 0; i < mainGameWorld.mTrees.size(); i++)
	{
		X::DrawSprite(treeTextureID, X::Math::Vector2((mainGameWorld.mTrees[i].mPosition.x) * 64, (mainGameWorld.mTrees[i].mPosition.y) * 64));
	}
}

void DrawStockPile()
{
	for (uint32_t i = 0; i < mainGameWorld.stockpileTiles.size(); i++)
	{
		X::DrawSprite(stockpileTextureID, mainGameWorld.stockpileTiles[i] * 64);
	}
}

void DrawBackground()
{
	for (uint32_t i = 0; i < mainGameWorld.backgroundTiles.size(); i++)
	{
		X::DrawSprite(grassTextureID, mainGameWorld.backgroundTiles[i] * 64);
	}
}


void RenderWorld()
{

	DrawGrid(mCiv.mPathfinder.mGraph.GetColumns(), mCiv.mPathfinder.mGraph.GetRows(), cellSize);
	DrawBackground();
	DrawPath(mCiv.Civ_Path, X::Math::Vector4::Blue());
	DrawFloors();
	DrawWalls();
	DrawTrees();
	DrawStockPile();
	DrawBeds();
	DrawSleep();

	
	DrawCursor(cursorTextureID);

	//X::DrawScreenLine(mCiv.Civ_Position, mCiv.Civ_TargetDestination, X::Math::Vector4::Green());
}




/*

		What I want to do:

		Construction:
		1) Set up a list of build sites for walls
		2) Set up a list of build sites for beds
		3) Set up a build site for a Table


		Chores
		1) Set up a cook site (one position to move to in order to enter cook state)
		2) Set up a list of trees (lumberjack will move to the tree and enter the gather state)
		3) Set up a list of Animals (animals will simply wander until they are hunted and killed)


*/