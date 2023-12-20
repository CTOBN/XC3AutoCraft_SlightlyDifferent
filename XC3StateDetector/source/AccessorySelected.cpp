#include "State.hpp"
#include "AccessorySelection.hpp"
#include "Judge.hpp"
#include "Context.hpp"

namespace xc3
{
	AccessorySelection::AccessorySelection() : State(U"AccessorySelection", 4000), hasTransitioned(false) {}

	void AccessorySelection::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::AccessorySelected_to_Judge);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.currentUnknownMatterCount -= 3;
			context.setState(std::make_unique<Judge>());
		}
	}
}

