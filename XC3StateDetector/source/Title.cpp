﻿#include "State.hpp"
#include "Context.hpp"
#include "Title.hpp"
#include "FieldLoading.hpp"

namespace xc3
{
	Title::Title() : State(U"Title", 2000), hasTransitioned(false) {}

	void Title::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Title_to_FieldLoading);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}

		if (isTimeToTransition())
		{
			context.setState(std::make_unique<FieldLoading>());
		}
	}
}

