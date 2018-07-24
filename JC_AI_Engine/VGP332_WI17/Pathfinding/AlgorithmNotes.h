//std::vector<Coord> RunBFS(Graph& g, Coord Start, Coord End)
//{
//	g.ResetGraph();
//	Coord currentNode = Start;
//	closedList.clear();
//
//	std::deque<Coord>  openList;
//	std::vector<Coord> parentList;
//
//	openList.push_back(currentNode);
//	g.mGrid[g.GetIndex(currentNode.x, currentNode.y)].isInOpen = true;
//
//	while (currentNode != End && openList.size() != 0)
//	{
//		std::vector<Coord> neighbors = g.GetGridNeighbors(currentNode);
//
//		for (int i = 0; i < neighbors.size(); i++)
//		{
//			bool isListed = false;
//
//			if (!myGraph.Get(neighbors[i]).isPassable)
//			{
//				isListed = true;
//			}
//
//			if (myGraph.Get(neighbors[i]).isInOpen)
//			{
//				isListed = true;
//			}
//
//			if (myGraph.Get(neighbors[i]).isInClosed)
//			{
//				isListed = true;
//			}
//
//			if (!isListed)
//			{
//				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].parent = currentNode;
//				openList.push_back(neighbors[i]);
//				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].isInOpen = true;
//			}
//		}
//
//		closedList.push_back(openList.front());
//		g.mGrid[g.GetIndex(openList.front().x, openList.front().y)].isInClosed = true;
//
//		openList.pop_front();
//		if (!openList.empty())
//		{
//			currentNode = openList.front();
//		}
//	}
//
//	while (currentNode.x != -1 && currentNode.y != -1)
//	{
//		parentList.push_back(currentNode);
//		currentNode = Coord(g.mGrid[g.GetIndex(currentNode.x, currentNode.y)].parent);
//	}
//	return parentList;
//}
//
//





//std::vector<Coord> RunDFS(Graph& g, Coord Start, Coord End)
//{
//	Coord currentNode = Start;
//
//	g.ResetGraph();
//	closedList.clear();
//
//	std::vector<Coord>  openList;
//	std::vector<Coord> parentList;
//
//	openList.push_back(currentNode);
//	g.mGrid[g.GetIndex(currentNode.x, currentNode.y)].isInOpen = true;
//
//	while (currentNode != End && openList.size() != 0)
//	{
//		currentNode = openList.back();
//		std::vector<Coord> neighbors = myGraph.GetGridNeighbors(currentNode);
//		closedList.push_back(currentNode);
//		g.mGrid[g.GetIndex(currentNode.x, currentNode.y)].isInClosed = true;
//		openList.pop_back();
//
//		for (int i = 0; i < neighbors.size(); i++)
//		{
//			bool isListed = false;
//
//
//			if (myGraph.Get(neighbors[i]).isInOpen)
//			{
//				isListed = true;
//			}
//
//			if (myGraph.Get(neighbors[i]).isInClosed)
//			{
//				isListed = true;
//			}
//
//			if (!isListed && myGraph.Get(neighbors[i]).isPassable)
//			{
//				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].parent = currentNode;
//				openList.push_back(neighbors[i]);
//				g.mGrid[g.GetIndex(neighbors[i].x, neighbors[i].y)].isInOpen = true;
//			}
//		}
//	}
//
//	while (currentNode.x != -1 && currentNode.y != -1)
//	{
//		parentList.push_back(currentNode);
//		currentNode = Coord(g.mGrid[g.GetIndex(currentNode.x, currentNode.y)].parent);
//	}
//	return parentList;
//}


//std::vector<Coord> RunDijkstraSearch(Graph& g, Coord Start, Coord End)
//{
//	//Dijkstra steps
//
//	//while (!destination && !openlist.empty())
//	//{
//
//	// 1) Search for the cheapest Node in Open List (could be front if sorted)
//	// 2) Once cheapest node is found: check if it is the destination
//	// 3) if it is the destination, break. otherwise, expand the node (push all the neighbors to the neighbor list)
//	// 4) For Each Neighbor: (check all even if already in open list)
//	//		a) calculate the cost from current node to current neighbor by running functor class (ie. TileMapCalculator myCalculator;)
//	//		b) if the current neighbor is already in the open list:
//	//			i) if the new cost is less than the old cost: update the cost and parent, and move on to the next neighbor
//	//			ii) if new cost is greater than or equal to the old cost do nothing and move on to next neighbor
//	//		c) otherwise, set cost and parent (for the first time) and push current neighbor to open list (setting isInOpen to true) and move on to next neighbor
//	// 5) Return To Step 1
//
//	// }
//	g.ResetGraph();
//
//	std::list<Coord> openList;
//	std::vector<Coord> neighbors;
//	std::vector<Coord> parentList;
//	Coord current = Start;
//
//	//SETUP
//	bool found = false;
//	closedList.clear();
//	openList.push_back(current);
//	g.mGrid[g.GetIndex(current.x, current.y)].isInOpen = true;
//	//END OF SETUP
//
//	while (!openList.empty() && !found)
//	{
//		current = FindCheapest(openList);
//		Coord next;
//
//		if (current == End)
//		{
//			found = true;
//		}
//		else
//		{
//			neighbors = g.GetGridNeighbors(current);
//			for (int i = 0; i < neighbors.size(); i++)
//			{
//				next = neighbors[i];
//
//				float oldCost = g.mGrid[g.GetIndex(current.x, current.y)].g;
//
//				float heuristicCost = 0;
//
//				float stepCost = CalculateCost(current, next);
//
//				float newCost = oldCost + stepCost;
//
//				if (!g.Get(next).isInClosed)
//				{
//					if (g.Get(next).isInOpen && g.Get(next).isPassable)
//					{
//						if (newCost < oldCost)
//						{
//							g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
//							g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
//						}
//					}
//					else if (g.Get(next).isPassable)
//					{
//						g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
//						g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
//						g.mGrid[g.GetIndex(next.x, next.y)].h = heuristicCost;
//						openList.push_back(next);
//						g.mGrid[g.GetIndex(next.x, next.y)].isInOpen = true;
//					}
//				}
//			}
//			openList.remove(current);
//			closedList.push_back(current);
//			g.mGrid[g.GetIndex(current.x, current.y)].isInClosed = true;
//		}
//	}
//
//	while (current.x != -1 && current.y != -1)
//	{
//		parentList.push_back(current);
//		current = Coord(g.mGrid[g.GetIndex(current.x, current.y)].parent);
//	}
//	return parentList;
//}


//std::vector<Coord> RunAStarSearch(Graph& g, Coord Start, Coord End)
//{
//	g.ResetGraph();
//	closedList.clear();
//
//	std::list<Coord> openList;
//	std::vector<Coord> neighbors;
//	std::vector<Coord> parentList;
//	Coord current = Start;
//
//	//SETUP
//	bool found = false;
//	closedList.clear();
//	openList.push_back(current);
//	g.mGrid[g.GetIndex(current.x, current.y)].isInOpen = true;
//	//END OF SETUP
//
//	while (!openList.empty() && !found)
//	{
//		current = FindCheapest(openList);
//		Coord next;
//
//		if (current == End)
//		{
//			found = true;
//		}
//		else
//		{
//			neighbors = g.GetGridNeighbors(current);
//
//			for (int i = 0; i < neighbors.size(); i++)
//			{
//				next = neighbors[i];
//
//				float oldCost = g.mGrid[g.GetIndex(current.x, current.y)].g;
//
//				float heuristicCost = CalculateHeuristic(End.x, End.y, next.x, next.y);
//
//				float stepCost = CalculateCost(current, next);
//
//				float newCost = oldCost + stepCost;
//
//				if (!g.Get(next).isInClosed)
//				{
//					if (g.Get(next).isInOpen && g.Get(next).isPassable)
//					{
//						if (newCost < oldCost)
//						{
//							g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
//							g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
//						}
//					}
//					else if (g.Get(next).isPassable)
//					{
//						g.mGrid[g.GetIndex(next.x, next.y)].parent = current;
//						g.mGrid[g.GetIndex(next.x, next.y)].g = newCost;
//						g.mGrid[g.GetIndex(next.x, next.y)].h = heuristicCost;
//						openList.push_back(next);
//						g.mGrid[g.GetIndex(next.x, next.y)].isInOpen = true;
//					}
//				}
//			}
//			openList.remove(current);
//			closedList.push_back(current);
//			g.mGrid[g.GetIndex(current.x, current.y)].isInClosed = true;
//		}
//	}
//
//	while (current.x != -1 && current.y != -1)
//	{
//		parentList.push_back(current);
//		current = Coord(g.mGrid[g.GetIndex(current.x, current.y)].parent);
//	}
//	return parentList;
//}