#include <NintendoSwitchControlLibrary.h>

const int BUTTON_INTERVAL = 1000;
const int STICK_INTERVAL = 200;
constexpr int BUTTONS_COUNT = 26;
enum AccType { Wrist, Finger, Necklaces, Crowns };
enum AccType selectedAccType = AccType::Wrist;

namespace siv3dswitch
{
	struct ButtonByte
	{
		static const uint8_t A = 1;
		static const uint8_t B = 2;
		static const uint8_t X = 3;
		static const uint8_t Y = 4;
		static const uint8_t L = 5;
		static const uint8_t R = 6;
		static const uint8_t ZL = 7;
		static const uint8_t ZR = 8;
		static const uint8_t Plus = 9;
		static const uint8_t Minus = 10;
		static const uint8_t Home = 11;
		static const uint8_t Capture = 12;
		static const uint8_t LStickClick = 13;
		static const uint8_t LStickUp = 14;
		static const uint8_t LStickDown = 15;
		static const uint8_t LStickLeft = 16;
		static const uint8_t LStickRight = 17;
		static const uint8_t RStickClick = 18;
		static const uint8_t RStickUp = 19;
		static const uint8_t RStickDown = 20;
		static const uint8_t RStickLeft = 21;
		static const uint8_t RStickRight = 22;
		static const uint8_t Up = 23;
		static const uint8_t Down = 24;
		static const uint8_t Left = 25;
		static const uint8_t Right = 26;
	};

	struct CommandByte
	{
		static const uint8_t Title_to_FieldLoading = 201;
		static const uint8_t FieldLoading_to_Field = 202;
		static const uint8_t Field_to_Camp = 203;
		static const uint8_t TitleLoading_to_Title = 200;
		static const uint8_t Camp_to_AccessoryCraft	= 204;
		static const uint8_t AccessoryCraft_to_AccTypeSelected = 205;
		static const uint8_t AccTypeSelected_to_Make = 206;
		static const uint8_t Make_to_Judge = 207;
		static const uint8_t Judge_to_MainMenu	= 208;
		static const uint8_t MainMenu_to_SystemMenu = 209;
		static const uint8_t SystemMenu_to_TitleLoading = 210;
	};
}

uint8_t siv3dswitch_buttons[BUTTONS_COUNT] = 
{
	siv3dswitch::ButtonByte::A,
	siv3dswitch::ButtonByte::B,
	siv3dswitch::ButtonByte::X,
	siv3dswitch::ButtonByte::Y,
	siv3dswitch::ButtonByte::L,
	siv3dswitch::ButtonByte::R,
	siv3dswitch::ButtonByte::ZL,
	siv3dswitch::ButtonByte::ZR,
	siv3dswitch::ButtonByte::Plus,
	siv3dswitch::ButtonByte::Minus,
	siv3dswitch::ButtonByte::Home,
	siv3dswitch::ButtonByte::Capture,
	siv3dswitch::ButtonByte::LStickClick,
	siv3dswitch::ButtonByte::LStickUp,
	siv3dswitch::ButtonByte::LStickDown,
	siv3dswitch::ButtonByte::LStickLeft,
	siv3dswitch::ButtonByte::LStickRight,
	siv3dswitch::ButtonByte::RStickClick,
	siv3dswitch::ButtonByte::RStickUp,
	siv3dswitch::ButtonByte::RStickDown,
	siv3dswitch::ButtonByte::RStickLeft,
	siv3dswitch::ButtonByte::RStickRight,
	siv3dswitch::ButtonByte::Up,
	siv3dswitch::ButtonByte::Down,
	siv3dswitch::ButtonByte::Left,
	siv3dswitch::ButtonByte::Right,
};

void func_Button_A()
{
	pushButton(Button::A, BUTTON_INTERVAL);
}

void func_Button_B()
{
	pushButton(Button::B, BUTTON_INTERVAL);
}

void func_Button_X()
{
	pushButton(Button::X, BUTTON_INTERVAL);
}

void func_Button_Y()
{
	pushButton(Button::Y, BUTTON_INTERVAL);
}

void func_Button_L()
{
	pushButton(Button::L, BUTTON_INTERVAL);
}

void func_Button_R()
{
	pushButton(Button::R, BUTTON_INTERVAL);
}

void func_Button_ZL()
{
	pushButton(Button::ZL, BUTTON_INTERVAL);
}

void func_Button_ZR()
{
	pushButton(Button::ZR, BUTTON_INTERVAL);
}

void func_Button_PLUS()
{
	pushButton(Button::PLUS, BUTTON_INTERVAL);
}

void func_Button_MINUS()
{
	pushButton(Button::MINUS, BUTTON_INTERVAL);
}

void func_Button_LCLICK()
{
	pushButton(Button::LCLICK, BUTTON_INTERVAL);
}

void func_Button_RCLICK()
{
	pushButton(Button::RCLICK, BUTTON_INTERVAL);
}

void func_Button_HOME()
{
	pushButton(Button::HOME, BUTTON_INTERVAL);
}

void func_Button_CAPTURE()
{
	pushButton(Button::CAPTURE, BUTTON_INTERVAL);
}

void func_Button_LStickUp()
{
	pushHat(Hat::UP, HAT_INTERVAL);
}

void func_Button_LStickDown()
{
	pushHat(Hat::DOWN, HAT_INTERVAL);
}

void func_Button_LStickLeft()
{
	pushHat(Hat::LEFT, HAT_INTERVAL);
}

void func_Button_LStickRight()
{
	pushHat(Hat::RIGHT, HAT_INTERVAL);
}

void func_Button_RStickClick()
{
	pushButton(Button::RCLICK, BUTTON_INTERVAL);
}

void func_Button_RStickUp()
{
	pushHat(Hat::UP, HAT_INTERVAL);
}

void func_Button_RStickDown()
{
	pushHat(Hat::DOWN, HAT_INTERVAL);
}

void func_Button_RStickLeft()
{
	pushHat(Hat::LEFT, HAT_INTERVAL);
}

void func_Button_RStickRight()
{
	pushHat(Hat::RIGHT, HAT_INTERVAL);
}

void func_Button_Up()
{
	pushHat(Hat::UP, HAT_INTERVAL);
}

void func_Button_Down()
{
	pushHat(Hat::DOWN, HAT_INTERVAL);
}

void func_Button_Left()
{
	pushHat(Hat::LEFT, HAT_INTERVAL);
}

void func_Button_Right()
{
	pushHat(Hat::RIGHT, HAT_INTERVAL);
}


void (*button_funcs[BUTTONS_COUNT])() = 
{
	func_Button_A,
	func_Button_B,
	func_Button_X,
	func_Button_Y,
	func_Button_L,
	func_Button_R,
	func_Button_ZL,
	func_Button_ZR,
	func_Button_PLUS,
	func_Button_MINUS,
	func_Button_HOME,
	func_Button_CAPTURE,
	func_Button_LCLICK,
	func_Button_LStickUp,
	func_Button_LStickDown,
	func_Button_LStickLeft,
	func_Button_LStickRight,
	func_Button_RCLICK,
	func_Button_RStickUp,
	func_Button_RStickDown,
	func_Button_RStickLeft,
	func_Button_RStickRight,
	func_Button_Up,
	func_Button_Down,
	func_Button_Left,
	func_Button_Right,
};

void Title_to_FieldLoading()
{
	pushButton(Button::A, BUTTON_INTERVAL);
	pushButton(Button::A, BUTTON_INTERVAL);
}

void Field_to_Camp()
{
	// ネウロ湖のキャンプ前で[A調べる]
	pushButton(Button::A, BUTTON_INTERVAL);
}
	
void Camp_to_AccessoryCraft()
{
	// 休憩ポイントメニューからアクセサリクラフトを選択
	pushHat(Hat::LEFT, HAT_INTERVAL);
	pushHat(Hat::LEFT, HAT_INTERVAL);
	pushHat(Hat::LEFT, HAT_INTERVAL);
	pushButton(Button::A, BUTTON_INTERVAL);
	delay(3000);
}

void Set_AccType(enum AccType acc_type)
{
	selectedAccType = acc_type;
}

// アクセサリの種類を選択
void AccessoryCraft_to_AccTypeSelected()
{
	if (selectedAccType == AccType::Wrist)
	{
		// 何もしない
	}
	else if (selectedAccType == AccType::Finger)
	{
		tiltLeftStick(Stick::NEUTRAL, Stick::MAX, STICK_INTERVAL); // 左スティックを下に傾ける

	}
	else if (selectedAccType == AccType::Necklaces)
	{
		tiltLeftStick(Stick::NEUTRAL, Stick::MAX, STICK_INTERVAL); // 左スティックを下に傾ける
		tiltLeftStick(Stick::NEUTRAL, Stick::MAX, STICK_INTERVAL); // 左スティックを下に傾ける
	}
	else if (selectedAccType == AccType::Crowns)
	{
		tiltLeftStick(Stick::NEUTRAL, Stick::MIN, STICK_INTERVAL); // 左スティックを上に傾ける
	}
}

void AccessorySelection_to_Make()
{
	// アクセサリを作成
	holdButton(Button::A, 1500);
}

void Make_to_Judge()
{
	// アクセサリの作成結果を確認
	pushButton(Button::A, BUTTON_INTERVAL);
}

void Judge_to_MainMenu()
{
	pushButton(Button::B, BUTTON_INTERVAL);
	delay(5000);
	pushButton(Button::B, BUTTON_INTERVAL);
	pushButton(Button::X, BUTTON_INTERVAL);
}

void MainMenu_to_SystemMenu()
{
	// メインメニューからシステムメニューを選択
	tiltLeftStick(Stick::NEUTRAL, Stick::MIN, STICK_INTERVAL); // 左スティックを上に傾ける
	tiltLeftStick(Stick::NEUTRAL, Stick::MIN, STICK_INTERVAL); // 左スティックを上に傾ける
	pushButton(Button::A, BUTTON_INTERVAL);
}

void (*xc3_macros[])() =
{
	Title_to_FieldLoading,
	Field_to_Camp,
	Camp_to_AccessoryCraft,
	AccessoryCraft_to_AccTypeSelected,
	AccessorySelection_to_Make,
	Make_to_Judge,
	Judge_to_MainMenu,
};

void setup()
{
	// 9600bps でシリアルポートを開く
	Serial1.begin(9600);
	pushButton(Button::A, BUTTON_INTERVAL);
}


void loop()
{
	// 250 ミリ秒止める
	delay(250);

	// シリアル通信で受信したデータを読み込む
	const int val = Serial1.read();
	
	for (size_t i = 0; i < BUTTONS_COUNT; i++)
	{
		if (val == siv3dswitch_buttons[i])
		{
			button_funcs[i]();
		}
	}

	for (size_t i = 0; i < sizeof(xc3_macros) / sizeof(xc3_macros[0]); i++)
	{
		if (val == i + 200)
		{
			xc3_macros[i]();
		}
	}
}
