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
	const double ScreenshotButtonWidth = 360;
	const double ScreenshotFileNameTextWidth = 450;
	const double ScreenshotDateFormatWidth = 500;
	const double ScreenshotFileNameMaxCharacters = 20;
	const Vec2 titlePos = { 20, 10 };
	const Vec2 appLanguagePos = { 20, 100 };
	const Vec2 gameLanguagePos = { 20, 200 };
	const Vec2 HDMICapturePos = { 20, 300 };
	const Vec2 serialPortPos = { 20, 400 };
	const Vec2 craftAccessoryPos = { 20, 500 };
	const Vec2 accessoryCSVFolderPathPos = craftAccessoryPos.movedBy(0, 50);
	const Vec2 accessoryCSVFolderPathDrawPos = accessoryCSVFolderPathPos.movedBy(0, 50);
	const Vec2 accessoryCSVFilePathPos = accessoryCSVFolderPathDrawPos.movedBy(0, 50);
	const Vec2 accessoryCSVFilePathDrawPos = accessoryCSVFilePathPos.movedBy(0, 50);
	const Vec2 consecutiveStatusPos = accessoryCSVFilePathDrawPos.movedBy(0, 50);
	const Vec2 notificationToastPos = consecutiveStatusPos.movedBy(0, 50);
	const Vec2 screenshotPos = { 20, 850 };
	const Vec2 screenshotFolderPos = screenshotPos.movedBy(0, 50);
	const Vec2 screenshotFileNamePos = screenshotFolderPos.movedBy(ScreenshotButtonWidth, 0);
	const Vec2 screenshotDateFormatPos = screenshotFileNamePos.movedBy(ScreenshotFileNameTextWidth, 0);
	const Vec2 screenshotFileFormatPos = screenshotDateFormatPos.movedBy(ScreenshotDateFormatWidth, 0);
	const Vec2 screenshotFileExamplePos = screenshotFolderPos.movedBy(0, 50);
	const Vec2 applyButtonPos = { 1700, 950 };

	Array<String> LanguageSelection = { U"en-US", U"ja-JP" };
	String AppLanguage;
	String GameLanguage;
	String HDMICapture;
	String SerialPort;
	mutable String AccessoryCSVFolderPath;
	mutable String AccessoryCSVFilePath;
	mutable bool desireConsecutiveStatus;
	mutable bool enableToastNotification;
	mutable String ScreenshotFolderPath;
	mutable String ScreenshotFileName;
	mutable String ScreenshotDateFormat;
	mutable String ScreenshotFileFormat;
	mutable TextEditState ScreenshotFileNameTextEditState;
	mutable String ScreenshotFileNameText;

	Array<String> ScreenshotDateFormatSelection = { U"yyyy-MM-dd", U"yyyy-MM-dd HH.mm.ss", U"yyyy-MM-dd HH-mm-ss", U"yyyy年MM月dd日", U"yyyy年MM月dd日 HH時mm分ss秒" };
	Array<String> ScreenshotFileFormatSelection = { U".jpg", U".jpeg", U".png"};

	OpenableListBox openableListBoxAppLanguage{ font, appLanguagePos.movedBy(0, 40),  130, 35, LanguageSelection.size() };
	OpenableListBox openableListBoxGameLanguage{ font, gameLanguagePos.movedBy(0, 40), 130, 35, LanguageSelection.size() };
	OpenableListBox openableListBoxHDMICapture{ font, HDMICapturePos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox openableListBoxSerialPort{ font, serialPortPos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox openableListBoxScreenshotDateFormat{ font, screenshotDateFormatPos, ScreenshotDateFormatWidth, 35, ScreenshotDateFormatSelection.size()};
	OpenableListBox openableListBoxScreenshotFileFormat{ font, screenshotFileFormatPos, 90, 35, ScreenshotFileFormatSelection.size()};

	HashTable<String, HashTable<String, String>>& Translate = getData().Translate;

	mutable bool reload = false;
};
