#pragma once
# include <Siv3D.hpp>
# include "Accessory.hpp"

// 共有するデータ
struct GameData
{
	size_t cameraIndex = 0;
	String cameraName = U"未選択";

	const Array<String> statusTypeFileName = { U"HP", U"Attack", U"HealingPower", U"Dexterity", U"Agility", U"BlockRate",  U"CriticalRate" };
	const Array<String> statusTypeFileNameJP = { U"HP", U"攻撃力", U"回復力", U"器用さ", U"素早さ", U"ガード率", U"ｸﾘﾃｨｶﾙ率" };
	Array<Accessory> desiredAccessories = { };

	Array<Image> binarizedAbilities;
	Array<Image> icons;
	Array<Image> binarizedIcons;
	Array<Image> binarizedUnkownMatterNumbers;

	Serial serial;
	size_t serialIndex = 0;
	String serialName = U"未選択";

	// すべて同じ能力上昇値のアクセサリを希望するか
	bool desireConsencutiveStatus = false;

	// どのアクセサリを作成するか
	int8 accessoryTypeIndex = -1; // 0:腕輪, 1:指輪, 2:首飾, 3:冠

	// 開始時のアンノウンマターの数
	int initialUnknownMatterCount = 99;

	// 開始時の所持金
	int initialGold = 9999999;

	// 作成するアクセサリの最大数
	int maxAccessoryCreation = Min(initialUnknownMatterCount / 3, initialGold / 10000);

	const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
};

// 共有するデータの型を指定
using App = SceneManager<String, GameData>;

/// @brief 上下方向のグラデーションの背景を描画します。
/// @param topColor 上部の色
/// @param bottomColor 下部の色
void DrawVerticalGradientBackground(const ColorF& topColor, const ColorF& bottomColor);
