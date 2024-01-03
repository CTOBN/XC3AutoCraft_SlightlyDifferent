#include "State.hpp"
#include "Context.hpp"
#include "Camp.hpp"
#include "RecognizeItemCount.hpp"

namespace xc3
{
	Camp::Camp() : State(U"Camp", 6000) {}

	void Camp::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Camp_to_AccessoryMenu);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<RecognizeItemCount>());
		}
	}
}

