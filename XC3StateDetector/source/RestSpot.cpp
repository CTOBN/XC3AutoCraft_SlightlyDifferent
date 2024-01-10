#include "State.hpp"
#include "Context.hpp"
#include "RestSpot.hpp"
#include "RecognizeItemCount.hpp"
#include "Pause.hpp"

namespace xc3
{
	RestSpot::RestSpot() : State(U"RestSpot", 10000) {}

	void RestSpot::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isAccessoryCraftSelected = false;
		}
		if (isTimeToTransition())
		{
			if (context.isAccessoryCraftSelected)
			{
				context.setState(std::make_unique<RecognizeItemCount>());
			}
			else
			{
				context.message = U"Failed to get cursor for break point";
				context.setState(std::make_unique<Pause>());
			}
		}
	}
}

