﻿#pragma once
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
	const Vec2 TitlePos = { 20, 10 };
	const Vec2 AppLanguagePos = { 20, 100 };
	const Vec2 GameLanguagePos = { 20, 200 };
	const Vec2 HDMICapturePos = { 20, 300 };
	const Vec2 SerialPortPos = { 20, 400 };
	const Vec2 CraftAccessoryPos = { 20, 500 };
	const Vec2 AccessoryCSVFolderPathPos = CraftAccessoryPos.movedBy(0, 50);
	const Vec2 AccessoryCSVFolderPathDrawPos = AccessoryCSVFolderPathPos.movedBy(0, 50);
	const Vec2 AccessoryCSVFilePathPos = AccessoryCSVFolderPathDrawPos.movedBy(0, 50);
	const Vec2 AccessoryCSVFilePathDrawPos = AccessoryCSVFilePathPos.movedBy(0, 50);
	const Vec2 ConsecutiveStatusPos = AccessoryCSVFilePathDrawPos.movedBy(0, 50);
	const Vec2 NotificationToastPos = ConsecutiveStatusPos.movedBy(0, 50);
	const Vec2 ScreenshotPos = { 20, 850 };
	const Vec2 ScreenshotFolderPos = ScreenshotPos.movedBy(0, 50);
	const Vec2 ScreenshotFileNamePos = ScreenshotFolderPos.movedBy(ScreenshotButtonWidth, 0);
	const Vec2 ScreenshotDateFormatPos = ScreenshotFileNamePos.movedBy(ScreenshotFileNameTextWidth, 0);
	const Vec2 ScreenshotFileFormatPos = ScreenshotDateFormatPos.movedBy(ScreenshotDateFormatWidth, 0);
	const Vec2 ScreenshotFileExamplePos = ScreenshotFolderPos.movedBy(0, 50);
	const Vec2 ApplyButtonPos = { 1700, 950 };

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

	OpenableListBox OpenableListBoxAppLanguage{ font, AppLanguagePos.movedBy(0, 40),  130, 35, LanguageSelection.size() };
	OpenableListBox OpenableListBoxGameLanguage{ font, GameLanguagePos.movedBy(0, 40), 130, 35, LanguageSelection.size() };
	OpenableListBox OpenableListBoxHDMICapture{ font, HDMICapturePos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox OpenableListBoxSerialPort{ font, SerialPortPos.movedBy(0, 40), 800, 35, 2 };
	OpenableListBox OpenableListBoxScreenshotDateFormat{ font, ScreenshotDateFormatPos, ScreenshotDateFormatWidth, 35, ScreenshotDateFormatSelection.size()};
	OpenableListBox OpenableListBoxScreenshotFileFormat{ font, ScreenshotFileFormatPos, 90, 35, ScreenshotFileFormatSelection.size()};

	HashTable<String, HashTable<String, String>>& Translate = getData().Translate;

	mutable bool reload = false;
};