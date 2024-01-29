#include "State.hpp"
#include "AccessoryTypeSelecting.hpp"
#include "AccessoryTypeSelected.hpp"
#include "Context.hpp"

namespace xc3
{
	AccessoryTypeSelecting::AccessoryTypeSelecting(GameData config) : State(U"AccessoryTypeSelecting", 1000, config) {}

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
			context.setState(std::make_unique<AccessoryTypeSelected>(config));
		}
	}
}
