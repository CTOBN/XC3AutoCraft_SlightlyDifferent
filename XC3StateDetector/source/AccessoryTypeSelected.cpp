#include "State.hpp"
#include "Context.hpp"
#include "Common.hpp"
#include "SerialInstruction.hpp"
#include "AccessoryTypeSelected.hpp"
#include "Judge.hpp"

namespace xc3
{
	AccessoryTypeSelected::AccessoryTypeSelected(GameData config) : State(U"AccessoryTypeSelected", 4000 + config.adjustmentForCaptureDelayMs, config) {}

	void AccessoryTypeSelected::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(GameManipulationByte::AccessorySelected_to_Judge);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.currentEnigmatterCount -= 3;
			context.setState(std::make_unique<Judge>(config));
		}
	}
}

