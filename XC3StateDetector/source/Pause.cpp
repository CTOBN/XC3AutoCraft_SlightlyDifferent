#include "State.hpp"
#include "Context.hpp"
#include "Pause.hpp"

namespace xc3
{
	Pause::Pause(GameData config) : State(U"Pause", 0, config) {}

	void Pause::handle(Context& context) {
		return;
	}
}

