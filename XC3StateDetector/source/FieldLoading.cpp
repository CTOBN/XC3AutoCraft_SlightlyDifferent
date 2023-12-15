#include "State.hpp"
#include "Context.hpp"
#include "FieldLoading.hpp"
#include "Field.hpp"

namespace xc3
{
	FieldLoading::FieldLoading() : State(18000), hasTransitioned(false) {}

	void FieldLoading::handle(Context& context)
	{
		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::FieldLoading_to_Field);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Field>());
		}
	}
}

