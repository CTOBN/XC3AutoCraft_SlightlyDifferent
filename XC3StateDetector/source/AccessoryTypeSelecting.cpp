#include "State.hpp"
#include "AccessoryTypeSelecting.hpp"
#include "AccessoryTypeSelected.hpp"
#include "Context.hpp"

namespace xc3
{
	AccessoryTypeSelecting::AccessoryTypeSelecting() : State(U"AccessoryTypeSelecting", 1000) {}

	void AccessoryTypeSelecting::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isAccessoryTypeSelected = false;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<AccessoryTypeSelected>());
		}
	}
}
