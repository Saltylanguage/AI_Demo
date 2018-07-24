#include "Pathfinder.h"


namespace AI_Library
{

	void Pathfinder::SetSearchMode(SearchMode mode)
	{
		searchMode = mode;
	}


	Coord Pathfinder::GetCurrent(SearchMode mode, Graph& g, std::list<Coord>& openList)
	{
		if (mode == SearchMode::Dijkstra || mode == SearchMode::A_Star)
		{
			return FindCheapest(g, openList);
		}
		else if (mode == SearchMode::Breadth)
		{
			return openList.front();
		}
		else if (mode == SearchMode::Depth)
		{
			return openList.back();
		}
		return Coord(0, 0);
	}

	std::vector<Coord> Pathfinder::FindPath(SearchMode mode, Graph& g, Coord Start, Coord End)
	{
		g.ResetGraph();
		closedList.clear();

		bool isFound = false;

		std::list<Coord> openList;
		std::vector<Coord> parentList;

		Coord current = Start;

		openList.push_back(current);
		g.mGrid[g.GetIndex(current.x, current.y)].isInOpen = true;

		while (!openList.empty() && !isFound)
		{
			current = GetCurrent(mode, g, openList);			//Right here.
			if (current == End)
			{
				isFound = true;
			}
			else
			{
				std::vector<Coord> neighbors = g.GetGridNeighbors(current);


				if (mode == SearchMode::Breadth || mode == SearchMode::Depth)
				{
					ExpandWithoutCost(g, neighbors, openList, current);
				}
				else if (mode == SearchMode::Dijkstra || mode == SearchMode::A_Star)
				{
					ExpandWithCost(g, mode, neighbors, openList, current, End);
				}

				openList.remove(current);
				closedList.push_back(current);
				g.mGrid[g.GetIndex(current.x, current.y)].isInClosed = true;
			}
		}

		if (isFound)
		{
			while (current.x != -1 && current.y != -1)
			{
				parentList.push_back(current);
				current = Coord(g.mGrid[g.GetIndex(current.x, current.y)].parent);
			}
		}

		return parentList;
	}


	void Pathfinder::ExpandWithoutCost(Graph& g, const std::vector<Coord>& neighbors, std::list<Coord>& openList, const Coord& current)
	{
		for (uint32_t i = 0; i < neighbors.size(); i++)
		{
			bool isListed = false;

			if (g.Get(neighbors[i]).isInOpen)
			{
				isListed = true;
			}
			if (g.Get(neighbors[i]).isInClosed)
			{
				isListed = true;
			}
			if (!isListed && g.Get(neighbors[i]).isPassable)
			{
				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].parent = current;
				openList.push_back(neighbors[i]);
				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].isInOpen = true;
			}
		}
	}

	void Pathfinder::ExpandWithCost(Graph& g, SearchMode mode, const std::vector<Coord>& neighbors, std::list<Coord>& openList, const Coord& current, const Coord& End)
	{
		for (uint32_t i = 0; i < neighbors.size(); i++)
		{
			Coord next = neighbors[i];

			float oldCost = g.mGrid[g.GetIndex(current.x, current.y)].g;

			float heuristicCost = CalculateEuclidian((float)End.x, (float)End.y, (float)next.x, (float)next.y);

			if (mode == SearchMode::Dijkstra)
			{
				heuristicCost = 0.0f;
			}

			int stepCost = CalculateCost(Coord((int)current.x, (int)current.y),  
										   Coord((int)next.x, (int)next.y));

			float newCost = oldCost + stepCost;

			if (!g.Get(next).isInClosed)
			{
				if (g.Get(next).isInOpen && g.Get(next).isPassable)
				{
					if (newCost < oldCost)
					{
						g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
						g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
					}
				}
				else if (g.Get(next).isPassable)
				{
					g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
					g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
					g.mGrid[g.GetIndex(next.x, next.y)].h = heuristicCost;
					openList.push_back(next);
					g.mGrid[g.GetIndex(next.x, next.y)].isInOpen = true;
				}
			}
		}
	}

	Coord Pathfinder::FindCheapest(Graph& g, std::list<Coord>& open)
	{
		Coord cheapestNode = open.front();
		int size = open.size();

		for (std::list<Coord>::iterator iter = open.begin(); iter != open.end(); iter++)
		{
			float cheapestCost = 0.0f;
			cheapestCost = g.mGrid[g.GetIndex(cheapestNode.x, cheapestNode.y)].g + g.mGrid[g.GetIndex(cheapestNode.x, cheapestNode.y)].h;
			float comparedCost = g.mGrid[g.GetIndex(iter->x, iter->y)].g + g.mGrid[g.GetIndex(iter->x, iter->y)].h;

			if (cheapestCost >= comparedCost)
			{
				cheapestNode = *iter;
			}
		}
		return cheapestNode;
	}

	void Pathfinder::SetStartCoord()
	{
		startGrid_X = (int)X::GetMouseScreenX() / (int)cellSize;
		startGrid_Y = (int)X::GetMouseScreenY() / (int)cellSize;
	}

	void Pathfinder::SetEndCoord()
	{
		endGrid_X = (int)X::GetMouseScreenX() / (int)cellSize;
		endGrid_Y = (int)X::GetMouseScreenY() / (int)cellSize;
	}


	int Pathfinder::CalculateCost(Coord currentNode, Coord nextNode)
	{
		return  calculator->CalculateCost(mGraph.Get(currentNode).type, mGraph.Get(nextNode).type);
	}

	float Pathfinder::CalculateEuclidian(float x0, float y0, float x1, float y1)
	{
		float xDist = pow(x0 - x1, 2);
		float yDist = pow(y0 - y1, 2);
		float result = sqrt(xDist + yDist);
		return result;
	}

	float Pathfinder::CalculateManhatten(float x0, float y0, float x1, float y1)
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

	void Pathfinder::SetHeuristic(int mode)
	{
		//switch (mode)
		//{
		//case 1:
		//	CalculateHeuristic = CalculateEuclidian;
		//	break;
		//case 2:
		//	CalculateHeuristic = CalculateManhatten;
		//	break;
		//default:
		//	CalculateHeuristic = CalculateManhatten;
		//	break;

		//}
	}

}