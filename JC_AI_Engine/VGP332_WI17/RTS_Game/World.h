#ifndef INCLUDED_WORLD_H
#define INCLUDED_WORLD_H

#include "RTS.h"
#include "Unit.h"
//#include <Network/Inc/Network.h>

class World
{
public:
	static void Initialize();
	static void Terminate();
	static World* Get();

public:
	World();
	~World();

	void Update(float deltaTime);
	void Draw();

	void SpawnUnitRandom();

	//void WriteSnapshot(Network::StreamWriter& writer);
	//void ApplySnapshot(Network::StreamReader& reader);

private:
	void SelectUnits();
	void CommandUnits();

	//Network::LinkingContext mLinkingContext;

	std::vector<Unit*> mUnits;
};

#endif // #ifndef INCLUDED_WORLD_H