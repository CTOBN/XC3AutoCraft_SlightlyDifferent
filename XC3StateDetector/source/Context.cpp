#include "State.hpp"
#include "Context.hpp"

namespace xc3
{
	Context::Context(Serial& serial)
		: serial(serial)
	{
	}

	void Context::setState(std::unique_ptr<State> newState) {
		m_state = std::move(newState);
	}

	void Context::request()
	{
		if (m_state == nullptr)
		{
			return;
		}
		m_state->handle(*this);
	}
	void Context::setSerial(Serial& _serial) {
		this->serial = _serial;
	}

	String Context::getCurrentStateName() const
	{
		if (m_state == nullptr)
		{
			return U"Undefined";
		}
		return m_state->getName();
	}
}

