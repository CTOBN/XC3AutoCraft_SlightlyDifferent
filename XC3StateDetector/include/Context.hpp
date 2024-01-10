#pragma once

namespace xc3
{
	extern const HashTable<String, String> StateNameToDescription;

	// Forward declaration of XC3State
	class State;

	class Context {
	public:
		Context(Serial& serial);
		void request();
		void setSerial(Serial& serial);
		void setState(std::unique_ptr<State> newState);
		void deleteState();
		String getCurrentStateName() const;
		String getCurrentStateDescription() const;
		bool getCurrentStateHasTransitioned() const;
		Serial& serial;
		size_t currentEnigmatterCount = 0;
		bool gotDesiredAccessory = false;
		bool canMakeAccessory = false;
		bool isAccessoryCraftSelected = true;
		bool isUnkownMatterCountUpdated = true;
		bool isAccessoryTypeSelected = true;
		bool isJudged = true;

		String message;

		void init();
	private:
		std::unique_ptr<State> m_state; // The current state.
	};
}

