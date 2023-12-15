#include "State.hpp"
#include "Context.hpp"
#include "SystemMenu.hpp"
#include "TitleLoading.hpp"

namespace xc3
{
	SystemMenu::SystemMenu() : State(2600), hasTransitioned(false) {}

	void SystemMenu::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::SystemMenu_to_TitleLoading);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<TitleLoading>());
		}
	}
}


