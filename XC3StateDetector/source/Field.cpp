#include "State.hpp"
#include "Context.hpp"
#include "Field.hpp"
#include "Camp.hpp"

namespace xc3
{
	Field::Field() : State(U"Field", 6000), hasTransitioned(false) {}

	void Field::handle(Context& context) {

		if (not hasTransitioned)
		{
			context.serial.writeByte(Context::CommandByte::Field_to_Camp);
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<Camp>());
		}
	}
}

