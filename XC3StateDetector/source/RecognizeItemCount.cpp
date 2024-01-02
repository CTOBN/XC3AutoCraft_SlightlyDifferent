#include "State.hpp"
#include "Context.hpp"
#include "RecognizeItemCount.hpp"
#include "AccessorySelected.hpp"

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
			context.setState(std::make_unique<AccessorySelected>());
		}
	}
}
