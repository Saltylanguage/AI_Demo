#include "PerceptionModule.h"
#include "Agent.h"

AI_Library::PerceptionModule::PerceptionModule(Agent & agent) : mAgent(agent)
{
}

AI_Library::PerceptionModule::~PerceptionModule()
{
}

void AI_Library::PerceptionModule::AddSensor(Sensor * sensor)
{
	mSensors.push_back(sensor);
}

void AI_Library::PerceptionModule::Purge()
{
	for (uint32_t i = 0; i < mSensors.size(); i++)
	{
		delete mSensors[i];
	}
	mSensors.clear();
}

