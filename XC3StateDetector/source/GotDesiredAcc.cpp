#include "State.hpp"
#include "Context.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	GotDesiredAcc::GotDesiredAcc(GameData config) : State(U"GotDesiredAcc", 0, config) {}
	void GotDesiredAcc::handle(Context& context)
	{
		return;
	}
}

