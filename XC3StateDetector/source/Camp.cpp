#include "State.hpp"
#include "Context.hpp"
#include "Camp.hpp"
#include "RecognizeItemCount.hpp"
#include "Pause.hpp"

namespace xc3
{
	Camp::Camp() : State(U"Camp", 10000) {}

	void Camp::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
			context.isAccessoryCraftSelected = false;
		}
		if (isTimeToTransition())
		{
			if (context.isAccessoryCraftSelected)
			{
				context.setState(std::make_unique<RecognizeItemCount>());
			}
			else
			{
				context.message = U"休憩ポイントのカーソル取得に失敗しました";
				context.setState(std::make_unique<Pause>());
			}
		}
	}
}

