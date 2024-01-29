#include "State.hpp"
#include "Context.hpp"
#include "SerialInstruction.hpp"
#include "Judge.hpp"
#include "AccessoryTypeSelected.hpp"
#include "GoingMainMenu.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	Judge::Judge(GameData config) : State(U"Judge", 2000, config) {}

	void Judge::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isJudged = false;
		}
		if (not isTimeToTransition()) return; // 3秒経過していないなら何もしない

		context.currentAttemptCount = context.currentAttemptCount + 1;
		if (context.gotDesiredAccessory) // 目的のアクセサリを手に入れたなら
		{
			context.setState(std::make_unique<GotDesiredAcc>(config));
		}
		else if (context.currentEnigmatterCount >= 3) // アクセサリを作れる
		{
			context.serial.writeByte(GameManipulationByte::Judge_to_AccessorySelected);
			context.setState(std::make_unique<AccessoryTypeSelected>(config));
		}
		else // アクセサリを作れないならメインメニューに戻る
		{
			context.setState(std::make_unique<GoingMainMenu>(config));
		}
	}
}


