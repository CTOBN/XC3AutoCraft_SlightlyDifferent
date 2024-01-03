#include "State.hpp"
#include "Context.hpp"
#include "Pause.hpp"

namespace xc3
{
	Pause::Pause() : State(U"Pause", 0) {}

	void Pause::handle(Context& context) {
		return;
	}
}

