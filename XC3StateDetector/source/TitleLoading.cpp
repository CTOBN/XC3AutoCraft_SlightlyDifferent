#include "State.hpp"
#include "Context.hpp"
#include "TitleLoading.hpp"
#include "Title.hpp"

namespace xc3
{
	TitleLoading::TitleLoading() : State(U"TitleLoading", 2000) {}

	void TitleLoading::handle(Context& context)
	{
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<xc3::Title>());
		}
	}
}

