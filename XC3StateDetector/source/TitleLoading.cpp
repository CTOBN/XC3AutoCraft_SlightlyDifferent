#include "State.hpp"
#include "Context.hpp"
#include "TitleLoading.hpp"
#include "Title.hpp"

namespace xc3
{
	TitleLoading::TitleLoading(GameData config) : State(U"TitleLoading", 24000, config) {}

	void TitleLoading::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<xc3::Title>(config));
		}
	}
}

