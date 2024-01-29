#include "State.hpp"
#include "Context.hpp"
#include "SerialInstruction.hpp"
#include "SystemMenu.hpp"
#include "TitleLoading.hpp"

namespace xc3
{
	SystemMenu::SystemMenu(GameData config) : State(U"SystemMenu", 2000, config) {}

	void SystemMenu::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(GameManipulationByte::SystemMenu_to_TitleLoading);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<TitleLoading>(config));
		}
	}
}


