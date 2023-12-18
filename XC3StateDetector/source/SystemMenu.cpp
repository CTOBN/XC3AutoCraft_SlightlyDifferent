#include "State.hpp"
#include "Context.hpp"
#include "SystemMenu.hpp"
#include "TitleLoading.hpp"

namespace xc3
{
	SystemMenu::SystemMenu() : State(U"SystemMenu", 2600), hasTransitioned(false) {}

	void SystemMenu::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::SystemMenu_to_TitleLoading);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.currentUnknownMatterCount = context.initialUnkownMatterCount;
			context.setState(std::make_unique<TitleLoading>());
		}
	}
}


