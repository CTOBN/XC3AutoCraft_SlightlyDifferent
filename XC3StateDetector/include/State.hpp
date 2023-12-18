#pragma once

namespace xc3
{
	// Forward declaration of Context
	class Context;

	class State {
	public:
		State(String name, int64 duration);
		virtual ~State();
		virtual void handle(Context& context) = 0;
		bool isTimeToTransition() const;
		String getName() const { return name; }
	protected:
		int64 transitionTime;
		int64 transitionDuration; // Add a variable to hold the transition duration
		String name;
	};
}


