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

	void Context::init()
	{
		gotDesiredAccessory = false;
		canMakeAccessory = false;
		wasJudged = true;
	}

	void Context::debugPrint() const
	{
		Print << U"Context: {";
		Print << U"  current state: " << getCurrentStateName();
		Print << U"  initialUnkownMatterCount: " << initialUnkownMatterCount;
		Print << U"  currentUnknownMatterCount: " << currentUnknownMatterCount;
		Print << U"  gotDesiredAccessory: " << gotDesiredAccessory;
		Print << U"  canMakeAccessory: " << canMakeAccessory;
		Print << U"  wasJudged: " << wasJudged;
		Print << U"}";
	}
}

