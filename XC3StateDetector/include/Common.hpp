#pragma once
# include <Siv3D.hpp>
# include "Accessory.hpp"
#include "SeparativeSimpleGUI.hpp"

// 共有するデータ
struct GameData
{
	HashTable<String, HashTable<String, String>> Translate;
	uint32 HDMICaptureIndex = 0;
	String HDMICaptureName;
	Array<Accessory> desiredAccessories;
	Array<Image> binarizedSpecialEffects;
	Array<Image> icons;
	Array<Image> binarizedIcons;
	Array<Image> binarizedAccessoryTypes;
	Array<Image> binarizedEnigmatterNumbers;
	Array<Image> binarizedGameScenes;
	Array<Image> binarizedRestSpotMenus;
	Array<String> RestSpotMenus = { U"Discuss", U"LevelUp", U"CraftGems", U"Cook", U"CraftAccessories", U"Save", U"CleanClothes" };
	Array<String> GameSceneNames = { U"Undefined", U"Title", U"Field", U"RestSpot", U"CraftAccessories" };
	Serial serial;
	size_t serialPortIndex = 0;
	String serialPortName;

	// すべて同じ能力上昇値のアクセサリを希望するか
	bool desireConsecutiveStatus = false;

	// どのアクセサリを作成するか
	AccessoryType selectedAccessoryType = AccessoryType::Undefined; // 1:腕輪, 2:指輪, 3:首飾, 4:冠

	const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();

	String WebManualURL = U"https://github.com/eightgamedev/XC3AutoCraft";
	INI ini{ U"config.ini" };

	String AppLanguage;
	String GameLanguage;
	String HDMICapture;
	String SerialPort;
	String requirementJSONFolderPath;
	String requirementJSONFilePath;
	bool enableToastNotification = true;
	String ScreenshotFolderPath;
	String ScreenshotFileName;
	String ScreenshotDateFormat;
	String ScreenshotFileFormat;

	bool needToReloadGameLanguageDependentImages = false;
};

// 共有するデータの型を指定
using App = SceneManager<String, GameData>;

/// @brief 上下方向のグラデーションの背景を描画します。
/// @param topColor 上部の色
/// @param bottomColor 下部の色
void DrawVerticalGradientBackground(const ColorF& topColor, const ColorF& bottomColor);
