#include "State.hpp"
#include "Context.hpp"
#include "Judge.hpp"
#include "AccessorySelection.hpp"
#include "MainMenu.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	Judge::Judge() : State(U"Judge", 4000) , hasTransitioned(false) {}

	void Judge::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.wasJudged = false;
		}
		if (not isTimeToTransition()) return; // 3秒経過していないなら何もしない

		if (context.gotDesiredAccesory) // 目的のアクセサリを手に入れたなら
		{
			context.setState(std::make_unique<GotDesiredAcc>());
		}
		else if (context.currentUnknownMatterCount >= 3) // アクセサリを作れる
		{
			context.serial.writeByte(Context::CommandByte::Judge_to_AccessorySelected);
			context.setState(std::make_unique<AccessorySelection>());
		}
		else // アクセサリを作れないならメインメニューに戻る
		{
			context.serial.writeByte(Context::CommandByte::Judge_to_MainMenu);
			context.setState(std::make_unique<MainMenu>());
		}
	}
}


