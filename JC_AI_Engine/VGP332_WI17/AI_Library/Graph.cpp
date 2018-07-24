#include "Graph.h"

// example
// 01		02	03	04	05
// START	07	08	09	10		START = 6
// 11		12	13	14	15
// 16		17	18	19	END		END   =  20
// 21		22	23	24	25

namespace AI_Library
{

	Graph::Node* Graph::InitializeGrid(int width, int height)
	{
		SetColumns(width);
		SetRows(height);
		Node* graph = new Node[mColumns * mRows];

		return graph;
	}

	int Graph::GetIndex(int x, int y)
	{
		return (x * mColumns) + y;
	}

	void Graph::SetImpassable(Coord& node)
	{
		mGrid[(GetIndex(node.x, node.y))].isPassable = false;
	}

	void Graph::ResetGraph()
	{
		for (int i = 0; i < mRows * mColumns; i++)
		{
			mGrid[i].parent = Coord(-1, -1);
			mGrid[i].isInOpen = false;
			mGrid[i].isInClosed = false;

			mGrid[i].f = 0;
			mGrid[i].g = 0;

		}
	}

	Graph::Node Graph::Get(Coord node)
	{
		XASSERT(node.x < mColumns && node.y < mRows, "[Graph] Invalid coord.");
		return mGrid[GetIndex(node.x, node.y)];
	}

	std::vector<Coord> Graph::GetGridNeighbors(Coord c) const
	{
		std::vector<Coord> neighbors;

		Coord currentNeighbor;

		for (int y = c.y - 1; y <= c.y + 1; y++)
		{
			if (y >= 0 && y < mRows)
			{
				for (int x = c.x - 1; x <= c.x + 1; x++)
				{
					if (x >= 0 && x < mColumns)
					{
						currentNeighbor.x = x;
						currentNeighbor.y = y;
						if (currentNeighbor != c)
						{
							neighbors.push_back(currentNeighbor);
						}
					}
				}
			}
		}
		return neighbors;
	}

	std::vector<Coord> Graph::GetGridNeighbors2(Coord c) const
	{
		std::vector<Coord> neighbors;

		neighbors.push_back(Coord(c.x, c.y - 1));
		neighbors.push_back(Coord(c.x, c.y + 1));
		neighbors.push_back(Coord(c.x - 1, c.y));
		neighbors.push_back(Coord(c.x + 1, c.y));

		return neighbors;
	}
}