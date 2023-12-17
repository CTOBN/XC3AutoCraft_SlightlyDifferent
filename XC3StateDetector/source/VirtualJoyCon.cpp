#include "ButtonByte.hpp"
#include "JoyConGUI.hpp"
#include "VirtualJoyCon.hpp"

VirtualJoyCon::VirtualJoyCon(Serial& serial, Vec2 L_pos, Vec2 R_pos) :
	m_serial(serial),
	m_joyConGUI(L_pos, R_pos)
{
}

const Array<uint8> m_buttons = 
{
	ButtonByte::A,
	ButtonByte::B,
	ButtonByte::X,
	ButtonByte::Y,
	ButtonByte::L,
	ButtonByte::R,
	ButtonByte::ZL,
	ButtonByte::ZR,

	ButtonByte::Plus,
	ButtonByte::Minus,
	ButtonByte::Home,
	ButtonByte::Capture,

	ButtonByte::LStickClick,
	ButtonByte::LStickUp,
	ButtonByte::LStickDown,
	ButtonByte::LStickLeft,
	ButtonByte::LStickRight,

	ButtonByte::RStickClick,
	ButtonByte::RStickUp,
	ButtonByte::RStickDown,
	ButtonByte::RStickLeft,
	ButtonByte::RStickRight,

	ButtonByte::Up,
	ButtonByte::Down,
	ButtonByte::Left,
	ButtonByte::Right,
};


bool (JoyConGUI::* funcs[26])() const =
	{
		& JoyConGUI::getButtonA,
		& JoyConGUI::getButtonB,
		& JoyConGUI::getButtonX,
		& JoyConGUI::getButtonY,
		& JoyConGUI::getButtonL,
		& JoyConGUI::getButtonR,
		& JoyConGUI::getButtonZL,
		& JoyConGUI::getButtonZR,

		& JoyConGUI::getButtonPlus,
		& JoyConGUI::getButtonMinus,
		& JoyConGUI::getButtonHome,
		& JoyConGUI::getButtonCapture,

		& JoyConGUI::getButtonLStickClick,
		& JoyConGUI::getButtonLStickUp,
		& JoyConGUI::getButtonLStickDown,
		& JoyConGUI::getButtonLStickLeft,
		& JoyConGUI::getButtonLStickRight,

		& JoyConGUI::getButtonRStickClick,
		& JoyConGUI::getButtonRStickUp,
		& JoyConGUI::getButtonRStickDown,
		& JoyConGUI::getButtonRStickLeft,
		& JoyConGUI::getButtonRStickRight,

		& JoyConGUI::getButtonUp,
		& JoyConGUI::getButtonDown,
		& JoyConGUI::getButtonLeft,
		& JoyConGUI::getButtonRight,
	};

void VirtualJoyCon::sendButtonSerial()
{
	for (size_t i = 0; i < m_buttonsSize; i++) {
		if ((m_joyConGUI.*(funcs[i]))()) {
			m_serial.writeByte(m_buttons[i]);
		}
	}
}

void VirtualJoyCon::update()
{
	m_joyConGUI.update();
	sendButtonSerial();
}

void VirtualJoyCon::draw() const
{
	m_joyConGUI.draw();
}
