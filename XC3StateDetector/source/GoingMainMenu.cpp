#include "State.hpp"
#include "Context.hpp"
#include "SerialInstruction.hpp"
#include "GoingMainMenu.hpp"
#include "MainMenu.hpp"

namespace xc3
{
	GoingMainMenu::GoingMainMenu(GameData config) : State(U"GoingMainMenu", 15000, config) {}

	void GoingMainMenu::handle(Context& context) {

		if (not hasTransitioned)
		{
			context.serial.writeByte(GameManipulationByte::GoingMainMenu_to_MainMenu);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<MainMenu>(config));
		}
	}
}

