#include "State.hpp"
#include "Context.hpp"
#include "Judge.hpp"
#include "Make.hpp"
#include "MainMenu.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	Judge::Judge() : State(U"Judge", 3000) {}

	void Judge::handle(Context& context)
	{
		if (not isTimeToTransition()) return; // 3秒経過していないなら何もしない

		if (context.gotDesiredAccesory) // 目的のアクセサリを手に入れたなら
		{
			context.setState(std::make_unique<GotDesiredAcc>());
		}
		else if (context.currentUnknownMatterCount >= 3) // アクセサリを作れる
		{
			context.serial.writeByte(Context::CommandByte::AccessorySelection_to_Make);
			context.wasJudged = false;
			context.setState(std::make_unique<Make>());
		}
		else // アクセサリを作れないならメインメニューに戻る
		{
			context.serial.writeByte(Context::CommandByte::Judge_to_MainMenu);
			context.wasJudged = false;
			context.setState(std::make_unique<MainMenu>());
		}
	}
}


