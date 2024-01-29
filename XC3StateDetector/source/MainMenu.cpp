#include "State.hpp"
#include "Context.hpp"
#include "SerialInstruction.hpp"
#include "MainMenu.hpp"
#include "SystemMenu.hpp"

namespace xc3
{
	MainMenu::MainMenu(GameData config) : State(U"MainMenu", 3000, config) {}

	void MainMenu::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(GameManipulationByte::MainMenu_to_SystemMenu);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<SystemMenu>(config));
		}
	}
}

