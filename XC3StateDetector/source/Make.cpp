#include "State.hpp"
#include "Context.hpp"
#include "Make.hpp"
#include "Judge.hpp"

namespace xc3
{
	Make::Make() : State(1500), hasTransitioned(false) {}

	void Make::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Make_to_Judge);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Judge>());
		}
	}
}

