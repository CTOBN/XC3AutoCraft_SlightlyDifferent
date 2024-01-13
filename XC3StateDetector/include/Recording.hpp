# pragma once
# include "Common.hpp"
# include "VirtualJoyCon.hpp"
# include "SerialInstruction.hpp"
# include "Context.hpp"
# include "State.hpp"

# include "Title.hpp"
# include "Field.hpp"
# include "RestSpot.hpp"
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
	HashTable<String, HashTable<String, String>>& Translate = getData().Translate;
	String AppLanguage = getData().AppLanguage;

	const HashTable<String, int32> fontSizeAccessory =
	{
		{ U"en-US", 12 },
		{ U"ja-JP", 15 },
	};

	Font fontAccessory { fontSizeAccessory.at(AppLanguage), Typeface::Bold };

	Array<String> RestSpotMenuNames = { U"相談", U"レベルアップ", U"ジェムクラフト", U"料理", U"アクセサリークラフト", U"セーブ", U"クリーニング" };

	const Point ABILITY_TEXT_AREA_POS{ 679, 516 };
	const Array<Point> clipStatusPosList{ {698, 659}, {841, 659}, {985, 659}, {1128, 659} };
	const Point ABILITY_TEXI_AREA_SIZE{ 550, 76 };
	const Point UNKOWN_MATTER_NUMBER_TENS_PLACE_POS{ 1646, 522 };
	const Point UNKOWN_MATTER_NUMBER_SIZE{ 18, 23 };
	const Point UNKOWN_MATTER_NUMBER_ONES_PLACE_POS = { UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.x + UNKOWN_MATTER_NUMBER_SIZE.x, UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.y };
	const Point StateInformationPos = { 1050, 450 };
	Accessory currentAccessory{ 0 };
	Array<Accessory> RecognizedAccessories;

	AsyncTask<Webcam> task;
	Webcam webcam;
	DynamicTexture texture;
	Image image;
	const Point CAMERA_RESOLUTION = { 1920, 1080 };
	const Point VIDEO_DISPLAY_SIZE = CAMERA_RESOLUTION / 2;
	String currentSpecialEffect;

	const Point buttonPos = { 1400, 70 };
	const int STATUS_ICON_NUM = 7;
	const Point STATUS_ICON_SIZE = { 30, 30 };

	const Vec2 JOYCON_GUI_POS_Left = { 1100, 250 };
	const Point desiredAccessoriesPos = { 1000, 580 };
	const Point recognizedAccessoriesPos = { 30, 580 };
	VirtualJoyCon virtualJoyCon{ getData().serial, JoyConGUI{ JOYCON_GUI_POS_Left} };
	Array<uint8> currentSerialBytes;
	uint8 lastSerialByte;

	double adjust_interval = 8000;

	mutable double RestSpotThreshold = 242;

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
		{GameManipulationByte::Title_to_FieldLoading, U"Title_to_FieldLoading"},
		{GameManipulationByte::Field_to_RestSpot, U"Field_to_RestSpot"},
		{GameManipulationByte::AccessorySelected_to_Judge, U"AccessorySelected_to_Judge"},
		{GameManipulationByte::Judge_to_AccessorySelected, U"Judge_to_AccessorySelected"},
		{GameManipulationByte::GoingMainMenu_to_MainMenu, U"GoingMainMenu_to_MainMenu"},
		{GameManipulationByte::MainMenu_to_SystemMenu, U"MainMenu_to_SystemMenu"},
		{GameManipulationByte::SystemMenu_to_TitleLoading, U"SystemMenu_to_TitleLoading"},
	};

	xc3::Context context{ getData().serial };

	const Array<std::pair<String, Array<String>>> menus
	{
		{ Translate[AppLanguage][U"App"], { Translate[AppLanguage][U"Exit"] }},
		{ Translate[AppLanguage][U"Help"], { U"\U000F0625 {}"_fmt(Translate[AppLanguage][U"Web Document"]), U"\U000F0FC3 {}"_fmt(Translate[AppLanguage][U"License Information"]) }},
	};
	SimpleMenuBar menuBar{ menus };

	double calculateSimilarity(const Image& img1, const Image& img2);

	size_t findMostSimilarNumber(const Point pos);

	Array<StatusBoost> findMostSimilarStatusBoost();

	size_t findMostSimilarAbility();

	String findMostSimilarGameScene();

	AccessoryType recognizeSelectingAccessoryType();

	int8 recognizeSelectingRestSpotMenu();

	Accessory recognizeAccessory();

	void updateContext();

	void drawButtons();

	void addAccessory(const Accessory& accessory);

	void selectAccessoryCraft();

	void recognizeEnigmatterCount();

	bool compareAccessories();

	bool completeMission();

	void countEnigmatter();

	void selectAccessoryType();

	void judgeAccessory();

	bool openSerialPort() const;

	void receiveSerialBytes();

	void drawSerialBytesLog();

	void drawRecognitionArea() const;

	void drawRecognizedAccessories() const;

	void drawDesiredAccessories() const;
};
