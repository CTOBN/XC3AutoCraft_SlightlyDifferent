#include "State.hpp"
#include "Context.hpp"
#include "Camp.hpp"
#include "AccessorySelected.hpp"

namespace xc3
{
	Camp::Camp() : State(U"Camp", 6000), hasTransitioned(false) {}

	void Camp::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Camp_to_AccessorySelected);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<AccessorySelected>());
		}
	}
}

