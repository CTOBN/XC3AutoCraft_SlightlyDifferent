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
	const double ScreenshotFileNameTextWidth = 450;
	const double ScreenshotDateFormatWidth = 500;
	const double ScreenshotFileNameMaxCharacters = 20;
	const Vec2 TitlePos = { 20, 20 };
	const Vec2 AppLanguagePos = { 20, 120 };
	const Vec2 GameLanguagePos = { 20, 240 };
	const Vec2 HDMICapturePos = { 20, 360 };
	const Vec2 SerialPortPos = { 20, 480 };
	const Vec2 ScreenshotFolderPos = { 20, 700 };
	const Vec2 ScreenshotFileNamePos = ScreenshotFolderPos.movedBy(275, 50);
	const Vec2 ScreenshotDateFormatPos = ScreenshotFileNamePos.movedBy(ScreenshotFileNameTextWidth, 0);
	const Vec2 ScreenshotFileFormatPos = ScreenshotDateFormatPos.movedBy(ScreenshotDateFormatWidth, 0);
	const Vec2 NotificationToastPos = { 20, 650 };
	const Vec2 ApplyButtonPos = { 800, 700 };

	Array<String> LanguageSelection = { U"en-US", U"ja-JP" };
	Array<String> HDMICaptures;

	//String AppLanguage = getData().AppLanguage;
	//String GameLanguage = getData().GameLanguage;
	//String HDMICapture = getData().HDMICapture;
	//String SerialPort = getData().SerialPort;
	//mutable String ScreenshotFolderPath = getData().ScreenshotFolderPath;
	//mutable String ScreenshotFileName = getData().ScreenshotFileName;
	//mutable String ScreenshotDateFormat = getData().ScreenshotDateFormat;
	//mutable String ScreenshotFileFormat = getData().ScreenshotFileFormat;
	String AppLanguage;
	String GameLanguage;
	String HDMICapture;
	String SerialPort;
	mutable String ScreenshotFolderPath;
	mutable String ScreenshotFileName;
	mutable String ScreenshotDateFormat;
	mutable String ScreenshotFileFormat;
	mutable TextEditState ScreenshotFileNameTextEditState;
	mutable String ScreenshotFileNameText;
	mutable bool enableToastNotification;

	Array<String> ScreenshotDateFormatSelection = { U"yyyy-MM-dd", U"yyyy-MM-dd HH.mm.ss", U"yyyy-MM-dd HH-mm-ss", U"yyyy年MM月dd日", U"yyyy年MM月dd日 HH時mm分ss秒" };
	Array<String> ScreenshotFileFormatSelection = { U".jpg", U".jpeg", U".png"};

	OpenableListBox OpenableListBoxAppLanguage{ font, AppLanguagePos.movedBy(0, 50),  130, 35, LanguageSelection.size() };
	OpenableListBox OpenableListBoxGameLanguage{ font, GameLanguagePos.movedBy(0, 50), 130, 35, LanguageSelection.size() };
	OpenableListBox OpenableListBoxHDMICapture{ font, HDMICapturePos.movedBy(0, 50), 800, 35, 2 };
	OpenableListBox OpenableListBoxSerialPort{ font, SerialPortPos.movedBy(0, 50), 800, 35, 2 };
	OpenableListBox OpenableListBoxScreenshotDateFormat{ font, ScreenshotDateFormatPos, ScreenshotDateFormatWidth, 35, ScreenshotDateFormatSelection.size()};
	OpenableListBox OpenableListBoxScreenshotFileFormat{ font, ScreenshotFileFormatPos, 90, 35, ScreenshotFileFormatSelection.size()};

	HashTable<String, HashTable<String, String>>& Translate = getData().Translate;

	mutable bool reload = false;
};
