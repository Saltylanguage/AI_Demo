#pragma once
#include "X\Inc\XEngine.h"

class Agent;

class AI_World
{
public:


	typedef std::vector<Agent*> AgentList;
	typedef std::vector<X::Math::Circle> Obstacles;
	typedef std::vector<X::Math::LineSegment> Walls;

	void Initialize(const X::Math::Vector2& worldSize);
	void Update(float deltaTime);

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	AgentList GetNeighborhood(const X::Math::Circle& range);

	void AddObstacle();
	void RemoveObstacles();

	const Obstacles& GetObstacles() const { return mObstacles; }

	void AddWall();
	void RemoveWall();

	const Walls& GetWalls() { return mWalls; }

	bool HasLOS(const X::Math::Vector2 start, const X::Math::Vector2 end);

private:

	AgentList mAgents;
	Obstacles mObstacles;
	Walls mWalls;
	//PartitionGrid Grid;
	//AI_Library::Graph mNavGraph;



};