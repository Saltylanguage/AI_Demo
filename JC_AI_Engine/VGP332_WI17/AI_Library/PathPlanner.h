#pragma once
#include "X\Inc\XEngine.h"


namespace AI_Library
{

	class Agent;

	typedef std::vector<X::Math::Vector2> Path;

	class PathPlanner
	{
	public:

		PathPlanner(Agent& agent);
		~PathPlanner();


		void RequestPath(const X::Math::Vector2& destination)
		{
			mPath.clear();

			// Check LOS
	/*		if (agent.GetWorld().HasLOS(agent.Position(), destination))
			{
				mPath.push_back(agent.Position());
				mPath.push_back(destination);
			}*/

			//Find Closest Valid Node to agent position
		}
		const Path& GetPath() const { return mPath; }
	

	private:
		Path mPath;

	};

}
