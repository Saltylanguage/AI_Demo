#include <X\Inc\XEngine.h>
#include <deque>
#include "Graph.h"
#include <stack>
#include <queue>
#include <list>

#include "PathingCalculator.h"
#include "TileMapCalculator.h"

namespace AI_Library
{
	class Pathfinder
	{
	public:


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
	bool isDraw = false;

	//Main Graph
	Graph mGraph;

	//Functor to Calculate Cost
	float(*CalculateHeuristic)(float x0, float y0, float x1, float y1);
	PathingCalculator* calculator = new TileMapCalculator();	
	
	//Rendered Object Lists
	std::vector<Coord> path;
	std::vector<Coord> closedList;
	std::list<X::Math::Vector2> actualPath;


	GameState state = GameState::Init;

	//End of Data Members

	//Main Game Loop

	//Pathfinding Functions
	std::vector<Coord> FindPath(SearchMode mode, Graph& g, Coord Start, Coord End);
	Coord GetCurrent(SearchMode mode, Graph& g, std::list<Coord>& openList);
	void ExpandWithoutCost(Graph& g, const std::vector<Coord>& neighbors, std::list<Coord>& openList, const Coord& current);
	void ExpandWithCost(Graph& g, SearchMode mode, const std::vector<Coord>& neighbors, std::list<Coord>& openList, const Coord& current, const Coord& End);
	void SetStartCoord();
	void SetEndCoord();

	//Pathfinding Helper Functions
	Coord FindCheapest(Graph& g, std::list<Coord>& open);

	int CalculateCost(Coord currentNode, Coord nextNode);
	void SetHeuristic(int mode);
	float CalculateEuclidian(float x0, float y0, float x1, float y1);
	float CalculateManhatten(float x0, float y0, float x1, float y1);

	//Draw Functions	
	
	//void DrawSearchPattern(std::vector<Coord> nodes, X::Math::Vector4 color);

	SearchMode searchMode = SearchMode(4);

	void SetSearchMode(SearchMode mode);	

	void SetPath()
	{
		path.clear();
		initPath = true;
		SetEndCoord();
		path = FindPath(searchMode, mGraph, Coord((int)position.x / (int)cellSize, (int)position.y / (int)cellSize), Coord((int)endGrid_X, (int)endGrid_Y));
		
	}




};


}
//Research items:
// SPT (Shortest path tree)
// edge relaxation
