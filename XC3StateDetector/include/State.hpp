#pragma once

namespace xc3
{
	// Forward declaration of Context
	class Context;

	class State {
	public:
		State(int64 duration);
		virtual ~State();
		virtual void handle(Context& context) = 0;
		bool isTimeToTransition() const;
	protected:
		int64 transitionTime;
		int64 transitionDuration; // Add a variable to hold the transition duration
	};
}


