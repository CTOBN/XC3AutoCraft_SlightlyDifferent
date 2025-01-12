﻿#include "State.hpp"
#include "Context.hpp"
#include "FieldLoading.hpp"
#include "Field.hpp"

namespace xc3
{
	FieldLoading::FieldLoading(GameData config) : State(U"FieldLoading", 22000, config) {}

	void FieldLoading::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Field>(config));
		}
	}
}

