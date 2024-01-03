#include "State.hpp"
#include "Context.hpp"
#include "RecognizeItemCount.hpp"
#include "AccessoryTypeSelecting.hpp"
#include "Pause.hpp"

namespace xc3
{
	RecognizeItemCount::RecognizeItemCount() : State(U"RecognizeItemCount", 2000) {}

	void RecognizeItemCount::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isUnkownMatterCountUpdated = false;
		}
		if (not isTimeToTransition()) return; // 2秒経過していないなら何もしない

		if (context.isUnkownMatterCountUpdated) // アンノウンマターの数が更新されたなら
		{
			if (context.currentUnknownMatterCount >= 3) // アクセサリを作れる
			{
				context.setState(std::make_unique<AccessoryTypeSelecting>());
			}
			else // アクセサリを作れないなら一時中断
			{
				context.message = U"アンノウンマターの数が不足しています";
				context.setState(std::make_unique<Pause>());
			}
		}
	}
}
