#include "State.hpp"
#include "Context.hpp"
#include "FieldLoading.hpp"
#include "Field.hpp"

namespace xc3
{
	FieldLoading::FieldLoading() : State(U"FieldLoading", 18000), hasTransitioned(false) {}

	void FieldLoading::handle(Context& context)
	{
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Field>());
		}
	}
}

