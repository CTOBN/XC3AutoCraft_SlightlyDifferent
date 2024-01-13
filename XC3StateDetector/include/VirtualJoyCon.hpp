#pragma once
#include "JoyConGUI.hpp"

class VirtualJoyCon
{
public:
	VirtualJoyCon() = delete;
	VirtualJoyCon(Serial& serial, JoyConGUI joyConGUI);
	void sendButtonSerial();
	void updateLED();
	void update();
	void draw() const;
private:
	JoyConGUI m_joyConGUI;
	Serial m_serial;
	Array<uint8> m_buttons;
	size_t m_buttonsSize;
	bool (JoyConGUI::* funcs[26])() const;
};

