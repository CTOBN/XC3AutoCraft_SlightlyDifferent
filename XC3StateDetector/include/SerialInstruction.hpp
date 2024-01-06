#pragma once

struct ButtonByte
{
	static const uint8 A = 1;
	static const uint8 B = 2;
	static const uint8 X = 3;
	static const uint8 Y = 4;
	static const uint8 L = 5;
	static const uint8 R = 6;
	static const uint8 ZL = 7;
	static const uint8 ZR = 8;
	static const uint8 Plus = 9;
	static const uint8 Minus = 10;
	static const uint8 Home = 11;
	static const uint8 Capture = 12;
	static const uint8 LStickClick = 13;
	static const uint8 LStickUp = 14;
	static const uint8 LStickDown = 15;
	static const uint8 LStickLeft = 16;
	static const uint8 LStickRight = 17;
	static const uint8 RStickClick = 18;
	static const uint8 RStickUp = 19;
	static const uint8 RStickDown = 20;
	static const uint8 RStickLeft = 21;
	static const uint8 RStickRight = 22;
	static const uint8 Up = 23;
	static const uint8 Down = 24;
	static const uint8 Left = 25;
	static const uint8 Right = 26;
};

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
