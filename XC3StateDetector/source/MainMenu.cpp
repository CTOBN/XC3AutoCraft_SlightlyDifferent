#include "State.hpp"
#include "Context.hpp"
#include "MainMenu.hpp"
#include "SystemMenu.hpp"

namespace xc3
{
	MainMenu::MainMenu() : State(9000) {}

	void MainMenu::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::MainMenu_to_SystemMenu);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<SystemMenu>());
		}
	}
}

