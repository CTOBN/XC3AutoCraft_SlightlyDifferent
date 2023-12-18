#include "State.hpp"
#include "Context.hpp"
#include "GotDesiredAcc.hpp"

namespace xc3
{
	GotDesiredAcc::GotDesiredAcc() : State(U"GotDesiredAcc", 0) {}
	void GotDesiredAcc::handle(Context& context)
	{
		return;
	}
}

