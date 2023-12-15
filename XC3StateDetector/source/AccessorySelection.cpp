#include "State.hpp"
#include "AccessorySelection.hpp"
#include "Make.hpp"
#include "Context.hpp"

namespace xc3
{
	AccessorySelection::AccessorySelection() : State(1400), hasTransitioned(false) {}

	void AccessorySelection::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::AccessorySelection_to_Make);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Make>());
		}
	}
}

