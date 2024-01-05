# pragma once
# include "Common.hpp"
# include "VirtualJoyCon.hpp"
# include "ButtonByte.hpp"
# include "Context.hpp"
# include "State.hpp"

# include "Title.hpp"
# include "Field.hpp"
# include "Camp.hpp"
# include "RecognizeItemCount.hpp"

# include "AccessoryTypeSelected.hpp"
# include "GotDesiredAcc.hpp"

# include "OpenableListBox.hpp"



class Recording : public App::Scene
{

public:
	Recording(const InitData& init);

	void update() override;

	void draw() const override;
private:
	const Point ABILITY_TEXT_AREA_POS{ 679, 516 };
	const Array<Point> clipStatusPosList{ {698, 659}, {841, 659}, {985, 659}, {1128, 659} };
	const Point ABILITY_TEXI_AREA_SIZE{ 550, 76 };
	const Point UNKOWN_MATTER_NUMBER_TENS_PLACE_POS{ 1646, 522 };
	const Point UNKOWN_MATTER_NUMBER_SIZE{ 18, 23 };
	const Point UNKOWN_MATTER_NUMBER_ONES_PLACE_POS = { UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.x + UNKOWN_MATTER_NUMBER_SIZE.x, UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.y };
	Accessory currentAccessory{ 0 };
	Array<Accessory> RecognizedAccessories;

	AsyncTask<Webcam> task;
	Webcam webcam;
	DynamicTexture texture;
	Image image;
	const Point CAMERA_RESOLUTION = { 1920, 1080 };
	const Point VIDEO_DISPLAY_SIZE = CAMERA_RESOLUTION / 2;
	String currentAccAbilityJapanese = U"未解析";
	String currentAccAbilityEnglish = U"Unrecorded";

	const Point buttonPos = { 1450, 50 };
	const int STATUS_ICON_NUM = 7;
	const Point STATUS_ICON_SIZE = { 30, 30 };

	const Vec2 JOYCON_GUI_POS_Left = { 1200, 250 };
	const Vec2 JOYCON_GUI_POS_Right = { 1300, 250 };
	const Point desiredAccessoriesPos = { 1000, 580 };
	const Point recognizedAccessoriesPos = { 30, 580 };
	VirtualJoyCon virtualJoyCon{ getData().serial, JOYCON_GUI_POS_Left, JOYCON_GUI_POS_Right };
	Array<uint8> currentSerialBytes;
	uint8 lastSerialByte;

	double adjust_interval = 8000;

	String screenShotFolderPath = U"ScreenShots";

	HashTable<uint8, uint8> accessoryTypeIndexToCommandByte = {
		{0, xc3::Context::CommandByte::SetAccessoryTypeAsBracelet},
		{1, xc3::Context::CommandByte::SetAccessoryTypeAsRing},
		{2, xc3::Context::CommandByte::SetAccessoryTypeAsNecklace},
		{3, xc3::Context::CommandByte::SetAccessoryTypeAsCrown}
	};

	HashTable<uint8, String> commandByteToString = {
		{ButtonByte::A, U"A"},
		{ButtonByte::B, U"B"},
		{ButtonByte::X, U"X"},
		{ButtonByte::Y, U"Y"},
		{ButtonByte::L, U"L"},
		{ButtonByte::R, U"R"},
		{ButtonByte::ZL, U"ZL"},
		{ButtonByte::ZR, U"ZR"},
		{ButtonByte::Plus, U"Plus"},
		{ButtonByte::Minus, U"Minus"},
		{ButtonByte::Home, U"Home"},
		{ButtonByte::Capture, U"Capture"},
		{ButtonByte::LStickClick, U"LStickClick"},
		{ButtonByte::LStickUp, U"LStickUp"},
		{ButtonByte::LStickDown, U"LStickDown"},
		{ButtonByte::LStickLeft, U"LStickLeft"},
		{ButtonByte::LStickRight, U"LStickRight"},
		{ButtonByte::RStickClick, U"RStickClick"},
		{ButtonByte::RStickUp, U"RStickUp"},
		{ButtonByte::RStickDown, U"RStickDown"},
		{ButtonByte::RStickLeft, U"RStickLeft"},
		{ButtonByte::RStickRight, U"RStickRight"},
		{ButtonByte::Up, U"Up"},
		{ButtonByte::Down, U"Down"},
		{ButtonByte::Left, U"Left"},
		{ButtonByte::Right, U"Right"},
		{xc3::Context::CommandByte::Title_to_FieldLoading, U"Title_to_FieldLoading"},
		{xc3::Context::CommandByte::Field_to_Camp, U"Field_to_Camp"},
		{xc3::Context::CommandByte::Camp_to_AccessoryMenu, U"Camp_to_AccessoryMenu"},
		{xc3::Context::CommandByte::AccessorySelected_to_Judge, U"AccessorySelected_to_Judge"},
		{xc3::Context::CommandByte::Judge_to_AccessorySelected, U"Judge_to_AccessorySelected"},
		{xc3::Context::CommandByte::GoingMainMenu_to_MainMenu, U"GoingMainMenu_to_MainMenu"},
		{xc3::Context::CommandByte::MainMenu_to_SystemMenu, U"MainMenu_to_SystemMenu"},
		{xc3::Context::CommandByte::SystemMenu_to_TitleLoading, U"SystemMenu_to_TitleLoading"},
		{xc3::Context::CommandByte::SetAccessoryTypeAsBracelet, U"SetAccessoryTypeAsBracelet"},
		{xc3::Context::CommandByte::SetAccessoryTypeAsRing, U"SetAccessoryTypeAsRing"},
		{xc3::Context::CommandByte::SetAccessoryTypeAsNecklace, U"SetAccessoryTypeAsNecklace"},
		{xc3::Context::CommandByte::SetAccessoryTypeAsCrown, U"SetAccessoryTypeAsCrown"},
	};

	xc3::Context context{ getData().serial };

	const Array<std::pair<String, Array<String>>> menus
	{
		{ U"ファイル", { U"終了" }},
		{ U"設定", { U"完成時にトースト通知する"}},
		{ U"ヘルプ", { U"\U000F0625 Webマニュアル", U"\U000F0FC3 ライセンス情報" } },
	};
	SimpleMenuBar menuBar{ menus };

	double calculateSimilarity(const Image& img1, const Image& img2);

	size_t findMostSimilarNumber(const Point pos);

	Array<StatusBoost> findMostSimilarStatusBoost();

	size_t findMostSimilarAbility();

	String findMostSimilarGameScene();

	AccessoryType recognizeSelectingAccessoryType();

	Accessory recognizeAccessory();

	void updateContext();

	void drawButtons();

	void addAccessory(const Accessory& accessory);

	void recognizeUnknownMatterCount();

	bool compareAccessories();

	bool completeMission();

	void countUnknownMatter();

	void selectAccessoryType();

	void judgeAccessory();

	bool openSerialPort() const;

	void receiveSerialBytes();

	void drawSerialBytesLog();

	void drawRecognitionArea() const;

	void drawRecognizedAccessories() const;

	void drawDesiredAccessories() const;
};
