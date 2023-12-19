#include "State.hpp"
#include "Context.hpp"

namespace xc3
{
	State::State(String name, int64 duration)
		: name(name)
		, transitionDuration(duration)
		, transitionTime(0)
	{}
	State::~State() {};

	// Method to check if it's time to transition
	bool State::isTimeToTransition() const
	{
		int64 now = Time::GetMillisec(); // Get the current time
		int64 duration = now - transitionTime; // Calculate the duration
		return duration >= transitionDuration; // Use the transition duration
	}
}
