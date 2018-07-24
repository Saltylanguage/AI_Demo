#include "SteeringModule.h"

void AI_Library::SteeringModule::AddBehavior(SteeringBehavior * behavior)
{
	mBehaviors.push_back(behavior);
}

AI_Library::SteeringBehavior * AI_Library::SteeringModule::GetBehavior(const char * name)
{
	for (uint32_t i = 0; i < mBehaviors.size(); i++)
	{
		if (mBehaviors[i]->GetName() == name)
		{
			return mBehaviors[i];
		}
	}
	return nullptr;
}

void AI_Library::SteeringModule::ToggleBehavior(SteeringBehavior * behavior)
{
	if (behavior->IsActive())
	{
		behavior->SetActive(false);
	}
	else
	{
		behavior->SetActive(true);
	}
}

void AI_Library::SteeringModule::Purge()
{
	for (uint32_t i = 0; i < mBehaviors.size(); i++)
	{
		if (mBehaviors[i] != nullptr)
		{
			X::SafeDelete(mBehaviors[i]);
			mBehaviors[i] = nullptr;
		}
	}
	mBehaviors.clear();
}

X::Math::Vector2 AI_Library::SteeringModule::Calculate()
{
	//vec2 total
	//for each behavior
	// total += behavior.calculate()
	//return total
	//use weight here if time allows

	X::Math::Vector2 total(0.0f, 0.0f);

	for (uint32_t i = 0; i < mBehaviors.size(); i++)
	{
		if (mBehaviors[i]->IsActive())
		{
			total += mBehaviors[i]->Calculate(mAgent);
		}
	}

	return total;
}
