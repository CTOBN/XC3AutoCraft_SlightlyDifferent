#pragma once
# include <Siv3D.hpp>
# include "Accessory.hpp"

// 共有するデータ
struct GameData
{
	HashTable<String, HashTable<String, String>> Translate =
	{
		{U"en-US", {
			{ U"Now Loading", U"Now Loading" },
			{ U"File", U"File" },
			{ U"Open CSV File", U"Open CSV File" },
			{ U"Save As", U"Save As" },
			{ U"Exit", U"Exit" },
			{ U"Configuration", U"Configuration" },
			{ U"Toast notification upon completion", U"Toast notification upon completion" },
			{ U"Change the save destination of the screenshot", U"Change the save destination of the screenshot" },
			{ U"Help", U"Help"},
			{ U"Web Document", U"Web Document" },
			{ U"License Information", U"License Information" },
			{ U"Desired accessories", U"Desired accessories" },
			{ U"Special Effects", U"Special Effects" },
			{ U"Status", U"Status" },
			{ U"Compatibility", U"Compat" },
			{ U"Sum", U"Sum"},
			{ U"Desire all accessories to have the same type of status regardless of special effects (Option)", U"Desire all accessories to have the same type of status regardless of special effects (Option)" },
			{ U"Select the type of accessory you want to make", U"Select the type of accessory you want to make" },
			{ U"There is a 0% chance that the desired accessory will be available", U"There is a 0% chance that the desired accessory will be available" },
			{ U"HDMI Capture", U"HDMI Capture" },
			{ U"Please select a HDMI Capture", U"Please select a HDMI Capture" },
			{ U"Serial Port" , U"Serial Port" },
			{ U"Please select a Serial Port", U"Please select a Serial Port" },
			{ U"Connection test available(Option)→", U"Connection test available(Option)→" },
			{ U"Serial connection test", U"Serial connection test" },
			{ U"Serial Port connection succeeded", U"Serial Port connection succeeded" },
			{ U"Serial Port connection failed", U"Serial Port connection failed" },
			{ U"Unselected", U"Unselected" },
			{ U"Anything", U"Anything" },
			{ U"Bracelet", U"Bracelet" },
			{ U"Ring", U"Ring" },
			{ U"Necklace", U"Necklace" },
			{ U"Crown", U"Crown" }
	
		}},
		{U"ja-JP", {
			{ U"Now Loading", U"読み込み中" },
			{ U"File", U"ファイル" },
			{ U"Open CSV File", U"CSVファイルを開く" },
			{ U"Save As", U"名前を付けて保存" },
			{ U"Exit", U"アプリを終了する" },
			{ U"Configuration", U"環境設定" },
			{ U"Toast notification upon completion", U"完成時にトースト通知する" },
			{ U"Change the save destination of the screenshot", U"スクリーンショットの保存先を変更" },
			{ U"Help", U"ヘルプ"},
			{ U"Web Document", U"Webマニュアル" },
			{ U"License Information", U"ライセンス情報" },
			{ U"Desired accessories", U"希望のアクセサリー" },
			{ U"Special Effects", U"特殊効果" },
			{ U"Status", U"ｽﾃｰﾀｽ" },
			{ U"Compatibility", U"互換" },
			{ U"Sum", U"合計" },
			{ U"Desire all accessories to have the same type of status regardless of special effects (Option)", U"特殊効果にかかわらず全て同じ種類のステータスのアクセサリを希望する (オプション)" },
			{ U"Select the type of accessory you want to make", U"作るアクセサリの種類を選択してください" },
			{ U"There is a 0% chance that the desired accessory will be available", U"目的のアクセサリーが出る可能性は0%です"},
			{ U"HDMI Capture", U"HDMIキャプチャー" },
			{ U"Please select a HDMI Capture", U"HDMIキャプチャーを選択してください" },
			{ U"Serial Port" , U"シリアルポート" },
			{ U"Please select a Serial Port", U"シリアルポートを選択してください" },
			{ U"Connection test available(Option)→", U"接続テストが可能です(オプション)→" },
			{ U"Serial connection test", U"シリアル接続テスト" },
			{ U"Serial Port connection succeeded", U"シリアルポートの接続に成功しました"},
			{ U"Serial Port connection failed", U"シリアルポートの接続に失敗しました"},
			{ U"Unselected", U"未選択" },
			{ U"Anything", U"任意" },
			{ U"Bracelet", U"腕輪" },
			{ U"Ring", U"指輪" },
			{ U"Necklace", U"首飾り" },
			{ U"Crown", U"冠" }
		}}
	};


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
