#include "State.hpp"
#include "Context.hpp"
#include "GoingMainMenu.hpp"
#include "MainMenu.hpp"

namespace xc3
{
	GoingMainMenu::GoingMainMenu() : State(U"GoingMainMenu", 15000) {}

	void GoingMainMenu::handle(Context& context) {

		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::GoingMainMenu_to_MainMenu);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<MainMenu>());
		}
	}
}

