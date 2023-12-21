#include "State.hpp"
#include "AccessorySelected.hpp"
#include "Judge.hpp"
#include "Context.hpp"

namespace xc3
{
	AccessorySelected::AccessorySelected() : State(U"AccessorySelected", 4000) {}

	void AccessorySelected::handle(Context& context)
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

