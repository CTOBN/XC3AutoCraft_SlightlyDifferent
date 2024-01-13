# include "SerialInstruction.hpp"
#include "JoyConGUI.hpp"
#include "VirtualJoyCon.hpp"

VirtualJoyCon::VirtualJoyCon(Serial& serial, JoyConGUI joyConGUI) :
	m_serial(serial),
	m_joyConGUI(joyConGUI)
{
	m_buttons =
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

	m_buttonsSize = m_buttons.size();

	funcs[0] = &JoyConGUI::getButtonA;
	funcs[1] = &JoyConGUI::getButtonB;
	funcs[2] = &JoyConGUI::getButtonX;
	funcs[3] = &JoyConGUI::getButtonY;
	funcs[4] = &JoyConGUI::getButtonL;
	funcs[5] = &JoyConGUI::getButtonR;
	funcs[6] = &JoyConGUI::getButtonZL;
	funcs[7] = &JoyConGUI::getButtonZR;
	funcs[8] = &JoyConGUI::getButtonPlus;
	funcs[9] = &JoyConGUI::getButtonMinus;
	funcs[10] = &JoyConGUI::getButtonHome;
	funcs[11] = &JoyConGUI::getButtonCapture;
	funcs[12] = &JoyConGUI::getButtonLStickClick;
	funcs[13] = &JoyConGUI::getButtonLStickUp;
	funcs[14] = &JoyConGUI::getButtonLStickDown;
	funcs[15] = &JoyConGUI::getButtonLStickLeft;
	funcs[16] = &JoyConGUI::getButtonLStickRight;
	funcs[17] = &JoyConGUI::getButtonRStickClick;
	funcs[18] = &JoyConGUI::getButtonRStickUp;
	funcs[19] = &JoyConGUI::getButtonRStickDown;
	funcs[20] = &JoyConGUI::getButtonRStickLeft;
	funcs[21] = &JoyConGUI::getButtonRStickRight;
	funcs[22] = &JoyConGUI::getButtonUp;
	funcs[23] = &JoyConGUI::getButtonDown;
	funcs[24] = &JoyConGUI::getButtonLeft;
	funcs[25] = &JoyConGUI::getButtonRight;
}


void VirtualJoyCon::sendButtonSerial()
{
	for (size_t i = 0; i < m_buttonsSize; i++)
	{
		if ((m_joyConGUI.*(funcs[i]))())
		{
			m_serial.writeByte(m_buttons[i]);
			// Console << m_buttons[i] << U" を送りました";
		}
	}
}

void VirtualJoyCon::updateLED()
{
	m_joyConGUI.setLEDStatusLeft(m_serial.isOpen(), 0);
	m_joyConGUI.setLEDStatusRight(m_serial.isOpen(), 0);
}

void VirtualJoyCon::update()
{
	m_joyConGUI.update();
	sendButtonSerial();
	updateLED();
}

void VirtualJoyCon::draw() const
{
	m_joyConGUI.draw();
}
