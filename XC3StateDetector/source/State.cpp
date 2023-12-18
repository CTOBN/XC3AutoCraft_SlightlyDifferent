﻿#include "State.hpp"
#include "Context.hpp"

namespace xc3
{
	State::State(String name, int64 duration)
		: name(name)
		, transitionDuration(duration)
		, transitionTime(0)
	{}
	State::~State() {};

	bool State::isTimeToTransition() const
	{
		int64 now = Time::GetMillisec();
		int64 duration = now - transitionTime;
		return duration >= transitionDuration; // Use the transition duration
	}
}
