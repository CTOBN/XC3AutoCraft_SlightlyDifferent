#include "State.hpp"
#include "Context.hpp"
#include "Judge.hpp"
#include "AccessoryTypeSelected.hpp"
#include "GoingMainMenu.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	Judge::Judge() : State(U"Judge", 2000) {}

	void Judge::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isJudged = false;
		}
		if (not isTimeToTransition()) return; // 3秒経過していないなら何もしない

		if (context.gotDesiredAccessory) // 目的のアクセサリを手に入れたなら
		{
			context.setState(std::make_unique<GotDesiredAcc>());
		}
		else if (context.currentUnknownMatterCount >= 3) // アクセサリを作れる
		{
			context.serial.writeByte(Context::CommandByte::Judge_to_AccessorySelected);
			context.setState(std::make_unique<AccessoryTypeSelected>());
		}
		else // アクセサリを作れないならメインメニューに戻る
		{
			context.setState(std::make_unique<GoingMainMenu>());
		}
	}
}


