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
		bool getCurrentStateHasTransitioned() const;
		Serial& serial;
		size_t currentUnknownMatterCount = 0;
		bool gotDesiredAccessory = false;
		bool canMakeAccessory = false;
		bool wasJudged = true;
		bool isUnkownMatterCountUpdated = true;
		struct CommandByte
		{
			static const uint8 Title_to_FieldLoading = 200;
			static const uint8 Field_to_Camp = 201;
			static const uint8 Camp_to_AccessorySelected = 202;
			static const uint8 AccessorySelected_to_Judge = 203;
			static const uint8 Judge_to_AccessorySelected = 204;
			static const uint8 GoingMainMenu_to_MainMenu = 205;
			static const uint8 MainMenu_to_SystemMenu = 206;
			static const uint8 SystemMenu_to_TitleLoading = 207;
			static const uint8 SetAccTypeAsWrist = 208;
			static const uint8 SetAccTypeAsFinger = 209;
			static const uint8 SetAccTypeAsNecklaces = 210;
			static const uint8 SetAccTypeAsCrowns = 211;
			static const uint8 adjust = 212;

		};
		void init();
		void debugPrint() const;
	private:
		std::unique_ptr<State> m_state; // The current state.
	};
}

