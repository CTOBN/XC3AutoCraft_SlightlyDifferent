#pragma once
#include "JoyConGUI.hpp"

class VirtualJoyCon
{
public:
	VirtualJoyCon() = delete;
	VirtualJoyCon(Serial& serial, Vec2 L_pos, Vec2 R_pos);
	void sendButtonSerial();
	void update();
	void draw() const;
private:
	JoyConGUI m_joyConGUI;
	Serial m_serial;
	const Array<uint8> m_buttons;
	const size_t m_buttonsSize = m_buttons.size();
	bool (JoyConGUI::* funcs[26])() const;
};

