#pragma once
#include "Common.hpp"
#include "OpenableListBox.hpp"

class Config : public App::Scene
{
public:
	Config(const InitData& init);

	void update() override;

	void draw() const override;

private:
	const int font_size = 20;
	const Font font{ font_size };
	const double screenshotButtonWidth = 360;
	const double screenshotFileNameTextWidth = 450;
	const double screenshotDateFormatWidth = 500;
	const double screenshotFileNameMaxCharacters = 20;
	const Vec2 titlePos = { 20, 20 };
	const Vec2 appLanguagePos = { 20, 110 };
	const Vec2 gameLanguagePos = { 20, 200 };
	const Vec2 HDMICapturePos = { 20, 290 };
	const Vec2 serialPortPos = { 20, 380 };
	const Vec2 craftAccessoryPos = { 20, 470 };
	const Vec2 requirementJSONFolderPathPos = craftAccessoryPos.movedBy(0, 50);
	const Vec2 requirementJSONFolderPathDrawPos = requirementJSONFolderPathPos.movedBy(0, 30);
	const Vec2 requirementJSONFilePathPos = requirementJSONFolderPathDrawPos.movedBy(0, 50);
	const Vec2 requirementJSONFilePathDrawPos = requirementJSONFilePathPos.movedBy(0, 30);
	const Vec2 notificationToastPos = requirementJSONFilePathDrawPos.movedBy(0, 100);
	const Vec2 screenshotPos = notificationToastPos.movedBy(0, 50);
	const Vec2 screenshotFolderPos = screenshotPos.movedBy(0, 50);
	const Vec2 screenshotFileNamePos = screenshotFolderPos.movedBy(screenshotButtonWidth + 20, 0);
	const Vec2 screenshotDateFormatPos = screenshotFileNamePos.movedBy(screenshotFileNameTextWidth, 0);
	const Vec2 screenshotFileFormatPos = screenshotDateFormatPos.movedBy(screenshotDateFormatWidth, 0);
	const Vec2 screenshotFileExamplePos = screenshotFolderPos.movedBy(0, 50);

	const Vec2 applyButtonPos = { 1600, 50 };
	const Vec2 goToSettingButtonPos = applyButtonPos.movedBy(0, 70);
	HashTable<String, HashTable<String, String>>& Translate = getData().Translate;
	Array<String> LanguageSelection = { U"en-US", U"ja-JP" };
	Array<std::pair<String, Array<String>>> menus;
	SimpleMenuBar menuBar;

	String AppLanguage;
	String GameLanguage;
	String HDMICapture;
	String SerialPort;
	String requirementJSONFolderPath;
	String requirementJSONFilePath;
	mutable bool enableToastNotification;
	mutable int adjustmentForCaptureDelayMs;
	String ScreenshotFolderPath;
	mutable String ScreenshotFileName;
	String ScreenshotDateFormat;
	String ScreenshotFileFormat;
	mutable TextEditState ScreenshotFileNameTextEditState;
	String ScreenshotFileNameText;

	Array<String> screenshotDateFormatSelection = { U"yyyy-MM-dd", U"yyyy-MM-dd HH.mm.ss", U"yyyy-MM-dd HH-mm-ss", U"yyyy年MM月dd日", U"yyyy年MM月dd日 HH時mm分ss秒" };
	Array<String> screenshotFileFormatSelection = { U".jpg", U".jpeg", U".png"};

	OpenableListBox openableListBoxAppLanguage{ font, appLanguagePos.movedBy(0, 40),  130, 35, LanguageSelection.size() };
	OpenableListBox openableListBoxGameLanguage{ font, gameLanguagePos.movedBy(0, 40), 130, 35, LanguageSelection.size() };
	OpenableListBox openableListBoxHDMICapture{ font, HDMICapturePos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox openableListBoxSerialPort{ font, serialPortPos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox openableListBoxScreenshotDateFormat{ font, screenshotDateFormatPos, screenshotDateFormatWidth, 35, 3 };
	OpenableListBox openableListBoxScreenshotFileFormat{ font, screenshotFileFormatPos, 90, 35, 3 };

	SeparativeSimpleGUI::Button buttonChangeSaveJSONFolder;
	SeparativeSimpleGUI::Button buttonChangeSaveJSONFile;
	SeparativeSimpleGUI::Button buttonChangeSaveScreenshotFolder;
	SeparativeSimpleGUI::Button buttonApply;
	SeparativeSimpleGUI::Button buttonGoToSetting;
};
