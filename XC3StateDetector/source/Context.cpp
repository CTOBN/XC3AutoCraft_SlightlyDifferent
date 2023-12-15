#include "State.hpp"
#include "Context.hpp"

namespace xc3
{
	Context::Context(State* state, Serial& serial)
		: m_state(state)
		, serial(serial)
	{
	}

	void Context::setState(std::unique_ptr<State> newState) {
		m_state = std::move(newState);
	}

	void Context::request() {
		m_state->handle(*this);
	}
	void Context::setSerial(Serial& _serial) {
		this->serial = _serial;
	}
}

