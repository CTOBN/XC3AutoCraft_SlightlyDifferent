#include "State.hpp"
#include "Context.hpp"
#include "SerialInstruction.hpp"
#include "Field.hpp"
#include "RestSpot.hpp"

namespace xc3
{
	Field::Field() : State(U"Field", 6000) {}

	void Field::handle(Context& context) {

		if (not hasTransitioned)
		{
			context.serial.writeByte(GameManipulationByte::Field_to_RestSpot);
			transitionTime = Time::GetMillisec();
			hasTransitioned = true;
		}
		if (isTimeToTransition())
		{
			context.setState(std::make_unique<RestSpot>());
		}
	}
}

