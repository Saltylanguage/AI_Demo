#pragma once
#include <vector>
#include <deque>
#include <X\Inc\XEngine.h>


namespace AI_Library
{

	struct Coord
	{
		Coord() : x(-1), y(-1) {}
		Coord(int xPos, int yPos) : x(xPos), y(yPos) {}


		//Container Library for graphs trees etc

		int x, y;


		bool operator!=(Coord other)
		{
			if (x == other.x && y == other.y)
			{
				return false;
			}
			return true;
		}

		bool operator==(Coord other)
		{
			if (x == other.x && y == other.y)
			{
				return true;
			}
			return false;
		}

		void operator=(Coord other)
		{
			x = other.x;
			y = other.y;
		}

	};

	class Graph
	{
	public:

		Graph() : mColumns(0), mRows(0) {}
		Graph(int cols, int rows) : mColumns(cols), mRows(rows) {}
		struct Node
		{
			Coord parent;

			int type = 1;

			bool isInOpen = false;
			bool isInClosed = false;
			bool isPassable = true;

			//f(x) = g(x) + h(x) 
			//g(x) = this node's g value + myCalculator.CalculateCost(g.mGrid.Get(CurrentNode).type, next.type);
			//h(x) = heuristic value (0.0f in Dijkstra)

			float f = 0.0f; // accumlated cost from start to where we are now 
			float g = 0.0f; // accumlated cost from start to where we are now
			float h = 0.0f; // heuristic cost (only used in A*)

		};

		Node* mGrid;


		Node* InitializeGrid(int width, int height);
		void ClearGrid(Coord* grid) {};

		Node Get(Coord node);
		std::vector<Coord> GetGridNeighbors(Coord c) const;
		std::vector<Coord> GetGridNeighbors2(Coord c) const;

		int GetIndex(int x, int y);

		int GetColumns() { return mColumns; }
		void SetColumns(int x) { mColumns = x; }

		int GetRows() { return mRows; }
		void SetRows(int y) { mRows = y; }

		void SetImpassable(Coord& node);

		void ResetGraph();



	private:
		int mColumns;
		int mRows;
	};

}





