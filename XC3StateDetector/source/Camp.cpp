#include "State.hpp"
#include "Context.hpp"
#include "Camp.hpp"
#include "AccessorySelection.hpp"

namespace xc3
{
	Camp::Camp() : State(4600), hasTransitioned(false) {}

	void Camp::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Camp_to_AccessorySelection);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<AccessorySelection>());
		}
	}
}

