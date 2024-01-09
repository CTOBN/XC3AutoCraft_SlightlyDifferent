#pragma once
# include <Siv3D.hpp>
# include "Accessory.hpp"

// 共有するデータ
struct GameData
{
	HashTable<String, HashTable<String, String>> Translate;
	uint32 cameraIndex = 0;
	String cameraName;
	Array<Accessory> desiredAccessories;
	Array<Image> binarizedAbilities;
	Array<Image> icons;
	Array<Image> binarizedIcons;
	Array<Image> binarizedUnkownMatterNumbers;
	Array<Image> binarizedGameScenes;
	Array<String> GameSceneNames = { U"Undefined", U"Title", U"Field", U"Camp", U"AccessoryMenu" };
	Serial serial;
	size_t serialIndex = 0;
	String serialName;

	// すべて同じ能力上昇値のアクセサリを希望するか
	bool desireConsecutiveStatus = false;

	// どのアクセサリを作成するか
	AccessoryType selectedAccessoryType = AccessoryType::Undefined; // 1:腕輪, 2:指輪, 3:首飾, 4:冠

	const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();

	String WebManualURL = U"https://github.com/eightgamedev/XC3AutoCraft";
	INI ini{ U"config.ini" };

	String AppLanguage;
	bool enableToastNotification = true;
	String ScreenshotFolderPath;
	String ScreenshotDateFormat;
	String ScreenshotFileFormat;
};

// 共有するデータの型を指定
using App = SceneManager<String, GameData>;

/// @brief 上下方向のグラデーションの背景を描画します。
/// @param topColor 上部の色
/// @param bottomColor 下部の色
void DrawVerticalGradientBackground(const ColorF& topColor, const ColorF& bottomColor);
