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
		size_t currentUnknownMatterCount = 0;
		bool gotDesiredAccessory = false;
		bool canMakeAccessory = false;
		bool isAccessoryCraftSelected = true;
		bool isUnkownMatterCountUpdated = true;
		bool isAccessoryTypeSelected = true;
		bool isJudged = true;

		String message;
		struct CommandByte
		{
			static const uint8 Title_to_FieldLoading = 200;
			static const uint8 Field_to_Camp = 201;
			static const uint8 Camp_to_AccessoryMenu = 202;
			static const uint8 AccessorySelected_to_Judge = 203;
			static const uint8 Judge_to_AccessorySelected = 204;
			static const uint8 GoingMainMenu_to_MainMenu = 205;
			static const uint8 MainMenu_to_SystemMenu = 206;
			static const uint8 SystemMenu_to_TitleLoading = 207;
			static const uint8 SetAccessoryTypeAsBracelet = 208;
			static const uint8 SetAccessoryTypeAsRing = 209;
			static const uint8 SetAccessoryTypeAsNecklace = 210;
			static const uint8 SetAccessoryTypeAsCrown = 211;
		};
		void init();
	private:
		std::unique_ptr<State> m_state; // The current state.
	};
}

