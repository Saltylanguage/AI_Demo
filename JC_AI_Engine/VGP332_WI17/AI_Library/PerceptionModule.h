#pragma once
#include "X\Inc\XEngine.h"
#include <list>


namespace AI_Library
{

	class Agent;


	struct MemoryRecord
	{
		MemoryRecord()
			: lastRecordedTime(0.0f)
			, importance(0.0f)
		{}
		virtual ~MemoryRecord() {}

		float lastRecordedTime;
		float importance;
	};

	typedef std::list<MemoryRecord> MemoryRecords;

	class Sensor
	{
	public:
		Sensor() {}
		virtual ~Sensor() {}

		virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
		virtual const char* GetName() const = 0;


	};

	typedef std::vector<Sensor*> Sensors;

	
	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent);
		virtual ~PerceptionModule();

		void AddSensor(Sensor* sensor);
		void Purge();

		virtual void Update(float deltaTime) = 0;



		virtual void UpdateMemoryRecords(); //  


		virtual void UpdateImportance(); // 


		virtual void CalculateImportance(); //
		
		MemoryRecords& GetMemoryRecords() { return mMemoryRecords; }
		
	private:

		Agent& mAgent;
		Sensors mSensors;
		MemoryRecords mMemoryRecords;

	};
}