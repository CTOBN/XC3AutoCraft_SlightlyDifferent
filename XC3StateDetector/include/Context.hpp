#pragma once

namespace xc3
{
	// Forward declaration of XC3State
	class State;

	class Context {
	public:
		Context(Serial& serial);
		void request();
		void setSerial(Serial& serial);
		void setState(std::unique_ptr<State> newState);
		String getCurrentStateName() const;
		Serial& serial;
		uint8 initialUnkownMatterCount = 0;
		uint8 currentUnknownMatterCount = 0;
		bool gotDesiredAccesory = false;
		bool canMakeAccesory = false;
		bool wasJudged = false;
		struct CommandByte
		{
			static const uint8 Title_to_FieldLoading = 200;
			static const uint8 Field_to_Camp = 201;
			static const uint8 Camp_to_AccessorySelection = 204;
			static const uint8 AccessorySelection_to_Make = 205;
			static const uint8 Make_to_Judge = 206;
			static const uint8 Judge_to_MainMenu = 207;
			static const uint8 MainMenu_to_SystemMenu = 208;
			static const uint8 SystemMenu_to_TitleLoading = 209;
			static const uint8 SetAccTypeAsWrist = 210;
			static const uint8 SetAccTypeAsFinger = 211;
			static const uint8 SetAccTypeAsNecklaces = 212;
			static const uint8 SetAccTypeAsCrowns = 213;

		};
	private:
		std::unique_ptr<State> m_state; // The current state.
	};
}

