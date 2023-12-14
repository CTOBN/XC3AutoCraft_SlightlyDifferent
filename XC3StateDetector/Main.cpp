# include <Siv3D.hpp>
# include "Pulldown.hpp"
# include "Accsessorie.hpp"
# include "JoyConGUI.hpp"

constexpr int STATUS_ICON_NUM = 7;
constexpr Point STATUS_ICON_SIZE = { 30, 30 };

struct ButtonByte
{
	static const uint8 A = 1;
	static const uint8 B = 2;
	static const uint8 X = 3;
	static const uint8 Y = 4;
	static const uint8 L = 5;
	static const uint8 R = 6;
	static const uint8 ZL = 7;
	static const uint8 ZR = 8;
	static const uint8 Plus = 9;
	static const uint8 Minus = 10;
	static const uint8 Home = 11;
	static const uint8 Capture = 12;
	static const uint8 LStickClick = 13;
	static const uint8 LStickUp = 14;
	static const uint8 LStickDown = 15;
	static const uint8 LStickLeft = 16;
	static const uint8 LStickRight = 17;
	static const uint8 RStickClick = 18;
	static const uint8 RStickUp = 19;
	static const uint8 RStickDown = 20;
	static const uint8 RStickLeft = 21;
	static const uint8 RStickRight = 22;
	static const uint8 Up = 23;
	static const uint8 Down = 24;
	static const uint8 Left = 25;
	static const uint8 Right = 26;
};


double calculateSimilarity(const Image& img1, const Image& img2) {
	// 画像のサイズが一致しない場合はエラーを返す
	if (img1.size() != img2.size()) {
		throw Error(U"Images must be the same size");
	}

	// 類似度を計算
	size_t matchCount = 0;
	for (size_t y = 0; y < img1.height(); ++y) {
		for (size_t x = 0; x < img1.width(); ++x) {
			if (img1[y][x] == img2[y][x]) {
				++matchCount;
			}
		}
	}

	// 類似度をパーセンテージで返す
	return static_cast<double>(matchCount) / (img1.height() * img1.width());
}

// 共有するデータ
struct GameData
{
	size_t cameraIndex = 0;
	String cameraName = U"未選択";

	Array<uint16> accessoriesID = { 0 };
	Array<String> accessoriesDiscription = { U"-" };
	Array<String> accessoriesDiscriptionModdified = { U"未選択" };
	Array<String> accessoriesAlready = { U"-" };
	Array<String> accessoriesWristProbability = { U"0.00" };
	Array<String> accessoriesFingerProbability = { U"0.00" };
	Array<String> accessoriesNecklacesProbability = { U"0.00" };
	Array<String> accessoriesCrownsProbability = { U"0.00" };

	Array<Accsessorie> sortedAccessories = { };
	const Array<String> statusIconsFileName = { U"agility", U"attack", U"critical", U"dexterity", U"guard", U"healing", U"hp" };
	const Array<String> statusIconsFileNameJP = { U"素早さ", U"攻撃力", U"ｸﾘﾃｨｶﾙ率", U"器用さ", U"ガード率", U"回復力", U"HP" };
	Array<Accsessorie> desiredAccessories = { };

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
	int accsessoryTypeIndex = -1; // 0:腕輪, 1:指輪, 2:首飾, 3:冠

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


class Loading : public App::Scene
{
public:
	const CSV csv{ U"csv/accessories.csv" };
	const CSV csvSorted{ U"csv/accessoriesSorted.csv" };
	const String discriptionJPTemplateFolderPath = U"images/Xenoblade3_Temp_Jp";
	const String statusIconsFolderPath = U"images/statusIcons";
	const String UnkonwnMatterNumbersPath = U"images/UnknownMatterNumbers";
	
	Array<String> discriptionsImagesPathList;
	Array<String> statusIconsPathList;
	

	Loading(const InitData& init)
		: IScene{ init }
	{
		if (not csv) // もし読み込みに失敗したら
		{
			throw Error{ U"Failed to load `accessories.csv`" };
		}

		for (size_t row = 0; row < csv.rows(); ++row)
		{
			getData().accessoriesID.push_back(Parse<uint16>(csv[row][0]));
			getData().accessoriesDiscription.push_back(csv[row][1]);
			getData().accessoriesDiscriptionModdified.push_back(csv[row][2]);
			getData().accessoriesAlready.push_back(csv[row][3]);
			getData().accessoriesWristProbability.push_back(csv[row][4]);
			getData().accessoriesFingerProbability.push_back(csv[row][5]);
			getData().accessoriesNecklacesProbability.push_back(csv[row][6]);
			getData().accessoriesCrownsProbability.push_back(csv[row][7]);
		}

		if (not csvSorted) // もし読み込みに失敗したら
		{
			throw Error{ U"Failed to load `accessoriesSorted.csv`" };
		}

		for (int row = 1; row < csvSorted.rows(); ++row) // 1行目はヘッダなので飛ばす
		{
			uint16 ID = Parse<uint16>(csvSorted[row][0]);
			String discriptionEN = csvSorted[row][1];
			String discriptionJP = csvSorted[row][2];

			getData().sortedAccessories.push_back(Accsessorie{ uint16(row-1), ID, discriptionEN, discriptionJP});
		}

		for (uint16 i = 3428; i <= 3913; i += 5)
		{
			String path = discriptionJPTemplateFolderPath + U"/" + Format(i) + U".jpg";
			discriptionsImagesPathList.push_back(path);
			getData().binarizedAbilities.push_back(Image{ path }.thresholded(128));
			Console << path << U" を読み込みました";
		}

		for (String statusIconFileName : getData().statusIconsFileName)
		{
			String path = statusIconsFolderPath + U"/" + statusIconFileName + U".jpg";
			statusIconsPathList.push_back(path);
			Image icon = Image{ path };
			getData().icons.push_back(icon);
			getData().binarizedIcons.push_back(icon.thresholded(128));
			Console << path << U" を読み込みました";
		}

		for (int i = 0; i < 10; i++)
		{
			String path = UnkonwnMatterNumbersPath + U"/" + Format(i) + U".jpg";
			getData().binarizedUnkownMatterNumbers.push_back(Image{ path }.thresholded(128));
			Console << path << U" を読み込みました";
		}
		getData().binarizedUnkownMatterNumbers.push_back(Image{ U"images/UnknownMatterNumbers/null.jpg" }.thresholded(128));
		Console << U"images/UnknownMatterNumbers/null.jpg" << U" を読み込みました";
	}

	void update() override
	{
		changeScene(U"Setting");
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });
		FontAsset(U"TitleFont")(U"Now Loading").drawAt(Scene::Center());
	}

private:


};

// 設定シーン
class Setting : public App::Scene
{
private:

	const int DESIRE_CONSENCUTIVE_STATUS_Y = 10;
	const int ACCSESSORIE_TEXT_Y = 60;
	const int CAMERA_TEXT_X = 20;
	const int CAMERA_TEXT_Y = 340;
	const int SERIAL_TEXT_X = 240;
	const int SERIAL_TEXT_Y = 340;
	const int CONFIRM_BUTTON_Y = 480;
	const int MENU_X = 20;
	const int font_size = 15;
	const int ACCESSORIES_FONT_SIZE = 18;
	const int CAMERA_FONT_SIZE = 18;
	const int line_padding = 20;


	const Font ACCSESSORIE_FONT{ ACCESSORIES_FONT_SIZE };
	const Font CAMERA_FONT{ CAMERA_FONT_SIZE };
	const Font SERIAL_FONT{ CAMERA_FONT_SIZE };
	
	const int ABILITY_VALUE_TEXT_X = 820;
	const int INFO_TEXT_X = 1200;
	const int INFO_TEXT_WIDTH = 80;

	const size_t TARGET_ACCSESORIES_COUNT_MAX = 5;

	double sumProbabilityWrists = 0;
	double sumProbabilityFingers = 0;
	double sumProbabilityNecklaces = 0;
	double sumProbabilityCrowns = 0;

	const double ACCESSORIES_CELL_WIDTH = 946;
	const double STATUS_CELL_WIDTH = 100;
	const double PROBABILITY_CELL_WIDTH = 80;

	const Font tableFont{ FontMethod::MSDF, 36 };

	SimpleTable accPulldownTable{ { ACCESSORIES_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH}, {
		.cellHeight = 30,
		.variableWidth = true,
		.font = tableFont,

		}
	};

	Point accPulldownTablePos = { MENU_X, ACCSESSORIE_TEXT_Y + 50 };

	SimpleTable probabilityTable{ {PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH}, {
		.cellHeight = 30,
		.variableWidth = true,
		.font = tableFont,
		}
	};

	Point probabilityTablePos = { MENU_X + int(ACCESSORIES_CELL_WIDTH + STATUS_CELL_WIDTH + PROBABILITY_CELL_WIDTH * 4),  ACCSESSORIE_TEXT_Y + 50};

public:
	const Font font{ font_size };

	Array<Pulldown> accsessoriesPulldowns;
	Array<Pulldown> abilityValuesPulldowns;
	Array<String> webcams = { U"未選択" };
	Pulldown cameraPulldown;
	Pulldown serialPulldown;
	// const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
	Array<String> options = { U"未選択" };

	Setting(const InitData& init)
		: IScene{ init }

	{
		for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			accsessoriesPulldowns.push_back(Pulldown{ getData().accessoriesDiscriptionModdified, ACCSESSORIE_FONT, Point{ MENU_X, ACCSESSORIE_TEXT_Y + 50 + (ACCESSORIES_FONT_SIZE + 15) * (i)} });
		}
		desiredAccessories_to_pullDowns();

		for (const auto& info : System::EnumerateWebcams())
		{
			webcams.push_back(info.name);
		};

		cameraPulldown = { webcams, CAMERA_FONT, Point{ CAMERA_TEXT_X, CAMERA_TEXT_Y + font_size * 3 + line_padding } };

		for (const auto& info : getData().infos)
		{
			Console << info.hardwareID << U" " << info.port << U" " << info.description;
			options.push_back(info.port);
		}
		serialPulldown = { options, SERIAL_FONT, Point{ SERIAL_TEXT_X, SERIAL_TEXT_Y + font_size * 3 + line_padding } };

		for (int y = 0; y < TARGET_ACCSESORIES_COUNT_MAX; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				abilityValuesPulldowns.push_back(Pulldown{ {U"", U"HP", U"攻撃力", U"回復力", U"器用さ", U"素早さ", U"ガード率", U"ｸﾘﾃｨｶﾙ率"}, ACCSESSORIE_FONT, Point{ABILITY_VALUE_TEXT_X + font_size * x * 6, ACCSESSORIE_TEXT_Y + font_size *(y + 1) * 2 + line_padding}});
			}
		}


		accPulldownTable.push_back_row({ U"特殊効果", U"ｽﾃｰﾀｽ１", U"ｽﾃｰﾀｽ2", U"ｽﾃｰﾀｽ3", U"ｽﾃｰﾀｽ4" });
		accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
		accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
		accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
		accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
		accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });

		placePulldowns();

		probabilityTable.push_back_row({ U"互換", U"腕輪", U"指輪", U"首飾", U"冠" }, {0, 0, 0, 0, 0});
		probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});
		probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});
		probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});
		probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});
		probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});
		probabilityTable.push_back_row({ U"合計", U"0.00", U"0.00", U"0.00", U"0.00" }, {0, 1, 1, 1, 1});

		placeAbilityValuesPulldowns();

	}
	void assignDesiredAccessories()
	{
		getData().desiredAccessories.clear();
		for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			if (accsessoriesPulldowns[i].getIndex() == 0)
			{
				continue;
			}
			Accsessorie acc = getData().sortedAccessories[accsessoriesPulldowns[i].getIndex() - 1];
			acc.setStatusList({ abilityValuesPulldowns[i * 4 + 0].getItem(),
								abilityValuesPulldowns[i * 4 + 1].getItem(),
								abilityValuesPulldowns[i * 4 + 2].getItem(),
								abilityValuesPulldowns[i * 4 + 3].getItem() });
			getData().desiredAccessories.push_back(acc);
		}
	}

	void desiredAccessories_to_pullDowns()
	{
		for (size_t i = 0; i < getData().desiredAccessories.size(); i++)
		{
			Accsessorie &acc = getData().desiredAccessories[i];
			accsessoriesPulldowns[i].setIndex(acc.getIndex() + 1);
		}
	}

	void calculateSumProbability()
	{
		sumProbabilityWrists = 0;
		sumProbabilityFingers = 0;
		sumProbabilityNecklaces = 0;
		sumProbabilityCrowns = 0;
		for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			size_t index = accsessoriesPulldowns[i].getIndex();
			sumProbabilityWrists += Parse<double>(getData().accessoriesWristProbability[index]);
			sumProbabilityFingers += Parse<double>(getData().accessoriesFingerProbability[index]);
			sumProbabilityNecklaces += Parse<double>(getData().accessoriesNecklacesProbability[index]);
			sumProbabilityCrowns += Parse<double>(getData().accessoriesCrownsProbability[index]);
		}
	}


	void placePulldowns()
	{
		for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			RectF region = accPulldownTable.cellRegion(accPulldownTablePos, i+1, 0);
			Point pos = region.pos.asPoint();
			accsessoriesPulldowns[i].setPos(pos);
		}
	}

	void placeAbilityValuesPulldowns()
	{
		for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				RectF region = accPulldownTable.cellRegion(accPulldownTablePos, i + 1, j + 1);
				Point pos = region.pos.asPoint();
				abilityValuesPulldowns[i * 4 + j].setPos(pos);
			}
		}
	}	

	void setProbability()
	{
		for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			probabilityTable.setText(i + 1, 0, getData().accessoriesAlready[accsessoriesPulldowns[i].getIndex()]);
			probabilityTable.setText(i + 1, 1, getData().accessoriesWristProbability[accsessoriesPulldowns[i].getIndex()]);
			probabilityTable.setText(i + 1, 2, getData().accessoriesFingerProbability[accsessoriesPulldowns[i].getIndex()]);
			probabilityTable.setText(i + 1, 3, getData().accessoriesNecklacesProbability[accsessoriesPulldowns[i].getIndex()]);
			probabilityTable.setText(i + 1, 4, getData().accessoriesCrownsProbability[accsessoriesPulldowns[i].getIndex()]);
		}

		probabilityTable.setText(6, 1, U"{:.2f}"_fmt(sumProbabilityWrists));
		probabilityTable.setText(6, 2, U"{:.2f}"_fmt(sumProbabilityFingers));
		probabilityTable.setText(6, 3, U"{:.2f}"_fmt(sumProbabilityNecklaces));
		probabilityTable.setText(6, 4, U"{:.2f}"_fmt(sumProbabilityCrowns));
	}


	void selectAccTypeButtonUpdate()
	{
		for (size_t i = 0; i < 4; i++)
		{
			RectF r = probabilityTable.cellRegion(probabilityTablePos, 0, i + 1);
			if (r.leftClicked())
			{
				getData().accsessoryTypeIndex = i;
			}
		}
	}
	void drawMouseOver() const
	{
		for (size_t i = 0; i < 4; i++)
		{
			RectF r = probabilityTable.cellRegion(probabilityTablePos, 0, i+1);
			if (r.mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
				r.draw(ColorF{ 0.5, 0.5, 0.5, 0.5 });
			}
		}
	}

	bool canMake() const
	{
		return getData().desireConsencutiveStatus || (sumProbabilityWrists > 0 && sumProbabilityFingers > 0 && sumProbabilityNecklaces > 0 && sumProbabilityCrowns > 0);
	}


	bool canGoRecording() const
	{
		return getData().accsessoryTypeIndex != -1 && getData().cameraName != U"未選択" && getData().serialName != U"未選択" && canMake();
	}



	void serialUpdate()
	{
		if (getData().serialName == U"未選択")
		{
			getData().serial = Serial{};
			FontAsset(U"TextFont")(U"シリアルポートを選択してください").draw(SERIAL_TEXT_X, SERIAL_TEXT_Y + 40, Palette::Red);
		}
		else
		{
			// シリアルポートをオープン
			if (getData().serial.open(getData().infos[getData().serialIndex].port))
			{
				FontAsset(U"TextFont")(U"シリアルポートを開きました").draw(SERIAL_TEXT_X, SERIAL_TEXT_Y + 40, Palette::Green);
			}
			else
			{
				FontAsset(U"TextFont")(U"シリアルポートを開けませんでした").draw(SERIAL_TEXT_X, SERIAL_TEXT_Y + 40, Palette::Red);
			}
		}
	}

	void drawNotion() const
	{
		if (not canMake())
		{
			FontAsset(U"TextFont")(U"目的のアクセサリが出る可能性は０です").draw(probabilityTablePos.x + PROBABILITY_CELL_WIDTH, probabilityTablePos.y + 30 * 7 + 30, Palette::Red);
		}

		if (getData().accsessoryTypeIndex == -1)
		{
			FontAsset(U"TextFont")(U"作るアクセサリを選択してください↓").draw(probabilityTablePos.x + PROBABILITY_CELL_WIDTH, probabilityTablePos.y - 50, Palette::Red);
		}
		else
		{
			probabilityTable.cellRegion(probabilityTablePos, 0, getData().accsessoryTypeIndex + 1).drawFrame(4, Palette::Red);
		}


		if (getData().cameraName == U"未選択")
		{
			FontAsset(U"TextFont")(U"カメラを選択してください").draw(CAMERA_TEXT_X, CAMERA_TEXT_Y + 40, Palette::Red);
		}


	}


	void update() override
	{
		SimpleGUI::CheckBox(getData().desireConsencutiveStatus, U"特殊効果にかかわらず全て同じ種類のステータス増加のアクセサリも希望する", Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y });
		serialUpdate();
		drawNotion();

		if (canGoRecording() && SimpleGUI::Button(U"決定", Vec2{1740, 800}))
		{
			assignDesiredAccessories();
			// Recordingシーンに遷移
			changeScene(U"Recording");
		}

		// プルダウンメニューを更新
		// 逆順に更新することで、選択時のクリックによって別のメニューが開いてしまうのを防ぐ
		for (auto it = std::rbegin(accsessoriesPulldowns); it != std::rend(accsessoriesPulldowns); ++it) {
			auto& accessoriesPulldown = *it;
			// 他のすべてのメニューが閉じている場合にのみ、このメニューを更新
			if (std::all_of(accsessoriesPulldowns.begin(), accsessoriesPulldowns.end(), [&](const Pulldown& m) { return &m == &accessoriesPulldown || !m.getIsOpen(); }))
			{
				accessoriesPulldown.update();
			}
		}
		

		cameraPulldown.update();
		getData().cameraIndex = cameraPulldown.getIndex() - 1;
		getData().cameraName = cameraPulldown.getItem();

		serialPulldown.update();
		getData().serialIndex = serialPulldown.getIndex() - 1;
		getData().serialName = serialPulldown.getItem();

		for (auto it = std::rbegin(abilityValuesPulldowns); it != std::rend(abilityValuesPulldowns); ++it) {
			auto& abilityValuesPulldown = *it;
			// 他のすべてのメニューが閉じている場合にのみ、このメニューを更新
			if (std::all_of(abilityValuesPulldowns.begin(), abilityValuesPulldowns.end(), [&](const Pulldown& m) { return &m == &abilityValuesPulldown || !m.getIsOpen(); }))
			{
				abilityValuesPulldown.update();
			}
		}

		calculateSumProbability();
		setProbability();
		selectAccTypeButtonUpdate();


	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

		probabilityTable.draw(probabilityTablePos);
		drawNotion();

		FontAsset(U"SubtitleFont")(U"シリアルポート").draw(SERIAL_TEXT_X, SERIAL_TEXT_Y);
		serialPulldown.draw();

		FontAsset(U"SubtitleFont")(U"カメラ").draw(CAMERA_TEXT_X, CAMERA_TEXT_Y);
		cameraPulldown.draw();

		accPulldownTable.draw(accPulldownTablePos);

		// 逆順に描画することで、展開されたメニューが手前に来るようにする
		for (int y = 0; y < TARGET_ACCSESORIES_COUNT_MAX; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				abilityValuesPulldowns[(TARGET_ACCSESORIES_COUNT_MAX - y - 1) * 4 + x].draw();
			}
		}

		FontAsset(U"SubtitleFont")(U"希望のアクセサリ").draw(20, ACCSESSORIE_TEXT_Y);

		// 逆順に描画することで、展開されたメニューが手前に来るようにする
		for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			accsessoriesPulldowns[TARGET_ACCSESORIES_COUNT_MAX - i - 1].draw();
		}

		Print << Cursor::Pos();

		

		
		drawMouseOver();

	}
};


class Recording : public App::Scene
{
private:
	const Point ABILITY_TEXT_AREA_POS{ 679, 516 };
	const Array<Point> clipStatusPosList{ {698, 659}, {841, 659}, {985, 659}, {1128, 659} };
	const Point ABILITY_TEXI_AREA_SIZE{ 550, 76 };
	const Point UNKOWN_MATTER_NUMBER_TENS_PLACE_POS{ 1646, 522 };
	const Point UNKOWN_MATTER_NUMBER_SIZE{ 18, 23 };
	const Point UNKOWN_MATTER_NUMBER_ONES_PLACE_POS = { UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.x + UNKOWN_MATTER_NUMBER_SIZE.x, UNKOWN_MATTER_NUMBER_TENS_PLACE_POS.y };
	Accsessorie currentAccessory;
	Array<Accsessorie> RecognizedAccessories;

	AsyncTask<Webcam> task;
	Webcam webcam;
	DynamicTexture texture;
	Image image;
	const Point CAMERA_RESOLUTION = { 1920, 1080 };
	const Point VIDEO_DISPLAY_SIZE = CAMERA_RESOLUTION / 2;
	String currentAccAbilityJP = U"未解析";
	String currentAccAbilityEN = U"Unredorded";

	const int buttonPosX = 1500;
	const int buttonPosY = 570;


	const Vec2 JOYCON_GUI_POS_Left = { 1500, 300 };
	const Vec2 JOYCON_GUI_POS_Right = { 1600, 300 };

	JoyConGUI joyconGUI{ JOYCON_GUI_POS_Left, JOYCON_GUI_POS_Right };

	size_t findMostSimilarNumber(const Point pos)
	{
		size_t judgedNumber = 0;
		double similarityMax = 0;
		webcam.getFrame(image);

		Image clipedImage = image.clipped(pos, UNKOWN_MATTER_NUMBER_SIZE).thresholded(128);
		for (size_t i = 0; i < getData().binarizedUnkownMatterNumbers.size(); i++)
		{
			Image binarizedNumber = getData().binarizedUnkownMatterNumbers[i];

			double similarity = calculateSimilarity(clipedImage, binarizedNumber);
			if (similarity > similarityMax)
			{
				similarityMax = similarity;
				judgedNumber = i;
			}
		}
		if (judgedNumber == 10) judgedNumber = 0; // null.jpgの場合0として扱う
		return judgedNumber;
	}

	Array<String> findMostSimilarIcons()
	{
		Array<String> result = { U"認識不可", U"認識不可", U"認識不可", U"認識不可" };
		webcam.getFrame(image);

		for (int i = 0; i < 4; i++)
		{
			int8 judgedIndex = -1;
			double similarityMax = 0;
			Image clipedImage = image.clipped(clipStatusPosList[i], STATUS_ICON_SIZE).thresholded(128);
			for (int8 j = 0; j < getData().icons.size(); j++)
			{
				Image binarizedIcon = getData().binarizedIcons[j];

				double similarity = calculateSimilarity(clipedImage, binarizedIcon);
				Console << Format(similarity);
				if (similarity > similarityMax)
				{
					similarityMax = similarity;
					judgedIndex = j;
				}
			}
			if (0 <= judgedIndex && judgedIndex < getData().statusIconsFileNameJP.size())
			{	
				result[i] = getData().statusIconsFileNameJP[judgedIndex];
			}
			else
			{
				Console << U"アクセサリの能力上昇の認識に失敗しました";
			}
		}
		return result;
		
	}

	size_t findMostSimilarAbility() {
		webcam.getFrame(image);

		size_t judgedIndex = -1;
		String judgedAbilityName = U"認識不可";
		double similarityMax = 0;
		Image clipedImage = image.clipped(ABILITY_TEXT_AREA_POS, ABILITY_TEXI_AREA_SIZE).thresholded(128);
		for (size_t i = 0; i < getData().binarizedAbilities.size(); i++)
		{
			Image binarizedAbility = getData().binarizedAbilities[i];

			double similarity = calculateSimilarity(clipedImage, binarizedAbility);
			if (similarity > similarityMax)
			{
				similarityMax = similarity;
				judgedIndex = i;
			}
		}
		if (0 <= judgedIndex && judgedIndex < getData().sortedAccessories.size())
		{
			currentAccAbilityJP = getData().sortedAccessories[judgedIndex].getDiscriptionJP();
			currentAccAbilityEN = getData().sortedAccessories[judgedIndex].getDiscriptionEN();


			Console << Format(judgedIndex) << currentAccAbilityJP;
		}
		else
		{
			Console << U"アクセサリの特殊効果の認識に失敗しました";
		}
		return judgedIndex;
	}
	void recognizeAccessory()
	{
		size_t judgedAccIndex = findMostSimilarAbility();
		Array<String> judgedIcons = findMostSimilarIcons();

		if (judgedAccIndex == -1)
		{
			return;
		}

		size_t judgedAccID = getData().sortedAccessories[judgedAccIndex].getID();
		currentAccessory = { judgedAccIndex, judgedAccID, currentAccAbilityEN, currentAccAbilityJP };
		currentAccessory.setStatusList(judgedIcons);
		RecognizedAccessories.push_back(currentAccessory);
	}

	// todo 順不同で十分条件で判定
	bool compareAccessories()
	{
		for (int i = 0; i < getData().desiredAccessories.size(); i++)
		{
			Accsessorie &desiredAcc = getData().desiredAccessories[i];
			if (currentAccessory == desiredAcc)
			{
				return true;
			}
		}
		return false;
	}

	bool completeMission()
	{
		// 全て同じ能力上昇値のアクセサリを希望して　かつ　現在のアクセサリが同じ能力上昇値のアクセサリである
		if (getData().desireConsencutiveStatus && currentAccessory.hasConsencutiveStatus())
		{
			return true;
		}
		// 目的のアクセサリの中に現在のアクセサリがある
		else if (compareAccessories())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void judge()
	{
		if (completeMission())
		{
			Console << U"目的のアクセサリが完成しました";
			Console << currentAccessory.getDiscriptionJP();
		}
		else
		{
			Console << currentAccessory.getDiscriptionJP();
		}
	}

	bool openSerialPort() const
	{
		if (getData().serial.open(getData().infos[getData().serialIndex].port))
		{
			Console << U"シリアルポートを開きました";
			return true;
		}
		else
		{
			Console << U"シリアルポートを開けませんでした";
			return false;
		}
	}

	void sendJoyConButtonSerial() const
	{
		Array<uint8> buttons =
		{
			ButtonByte::A,
			ButtonByte::B,
			ButtonByte::X,
			ButtonByte::Y,
			ButtonByte::L,
			ButtonByte::R,
			ButtonByte::ZL,
			ButtonByte::ZR,
			ButtonByte::Minus,
			ButtonByte::Plus,
			ButtonByte::Home,
			ButtonByte::Capture,
			ButtonByte::LStickClick,
			ButtonByte::LStickUp,
			ButtonByte::LStickDown,
			ButtonByte::LStickLeft,
			ButtonByte::LStickRight,
			ButtonByte::RStickClick,
			ButtonByte::RStickUp,
			ButtonByte::RStickDown,
			ButtonByte::RStickLeft,
			ButtonByte::RStickRight,
			ButtonByte::Up,
			ButtonByte::Down,
			ButtonByte::Left,
			ButtonByte::Right,
		};
		bool (JoyConGUI:: * funcs[])() const =
		{
			&JoyConGUI::getButtonA,
			&JoyConGUI::getButtonB,
			&JoyConGUI::getButtonX,
			&JoyConGUI::getButtonY,
			&JoyConGUI::getButtonL,
			&JoyConGUI::getButtonR,
			&JoyConGUI::getButtonZL,
			&JoyConGUI::getButtonZR,
			&JoyConGUI::getButtonMinus,
			&JoyConGUI::getButtonPlus,
			&JoyConGUI::getButtonLStickClick,
			&JoyConGUI::getButtonRStickClick,
			&JoyConGUI::getButtonHome,
			&JoyConGUI::getButtonCapture,
			&JoyConGUI::getButtonLStickClick,
			&JoyConGUI::getButtonLStickUp,
			&JoyConGUI::getButtonLStickDown,
			&JoyConGUI::getButtonLStickLeft,
			&JoyConGUI::getButtonLStickRight,
			&JoyConGUI::getButtonRStickClick,
			&JoyConGUI::getButtonRStickUp,
			&JoyConGUI::getButtonRStickDown,
			&JoyConGUI::getButtonRStickLeft,
			&JoyConGUI::getButtonRStickRight,
			&JoyConGUI::getButtonUp,
			&JoyConGUI::getButtonDown,
			&JoyConGUI::getButtonLeft,
			&JoyConGUI::getButtonRight,
		};


		for (size_t i = 0; i < buttons.size(); i++) {
			if ((joyconGUI.*funcs[i])()) {
				getData().serial.writeByte(buttons[i]);
			}
		}
	}

public:
	Recording(const InitData& init)
		: IScene{ init }
	{	
		// 非同期タスクを開始
		const uint32 CameraIndex = getData().cameraIndex;
		
		task = AsyncTask<Webcam>{ [CameraIndex, this]() {
			Webcam webcam{ CameraIndex, Size{ this->CAMERA_RESOLUTION }, StartImmediately::No };
			webcam.start();
			return webcam;
		} };
	}

	void update() override
	{

		// macOS では、ユーザがカメラ使用の権限を許可しないと Webcam の作成に失敗する。再試行の手段を用意する
# if SIV3D_PLATFORM(MACOS)
		if ((not webcam) && (not task.valid()))
		{
			if (SimpleGUI::Button(U"Retry", Vec2{ 20, 20 }))
			{
				task = AsyncTask{ []() { return Webcam{ 0, Size{ 1280, 720 }, StartImmediately::Yes }; } };
			}
		}
# endif
		if (task.isReady())
		{
			// 起動が完了した Webcam をタスクから取得
			webcam = task.get();
		}
		if (webcam.hasNewFrame())
		{
			webcam.getFrame(texture);
		}
		if (SimpleGUI::Button(U"シリアルポートを開く", Vec2{ buttonPosX, buttonPosY + 50 }))
		{
			openSerialPort();
		}

		if (SimpleGUI::Button(U"自動クラフト開始", Vec2{ buttonPosX, buttonPosY + 100 }))
		{
			
		}
		if (webcam && SimpleGUI::Button(U"アクセサリ認識テスト", Vec2{ buttonPosX, buttonPosY + 150 }))
		{
			recognizeAccessory();
			judge();
		}
		if (webcam && SimpleGUI::Button(U"PCにスクショを保存", Vec2{ buttonPosX, buttonPosY + 200}))
		{
			webcam.getFrame(image);
			image.save(U"xenoblade3_screenshot.png");
		}
		if (SimpleGUI::Button(U"設定に戻る", Vec2{ buttonPosX, buttonPosY + 250}))
		{
			// 設定に遷移
			changeScene(U"Setting");
		}



		if (SimpleGUI::Button(U"ｱﾝﾉｳﾝﾏﾀｰを数える", Vec2{ buttonPosX, buttonPosY }))
		{
			size_t number = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_TENS_PLACE_POS) * 10 + findMostSimilarNumber(UNKOWN_MATTER_NUMBER_ONES_PLACE_POS);
			Print << U"uk " << number;
		}

		Console << getData().serial.readBytes();

		joyconGUI.update();
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF(0.2, 0.8, 0.6));
		// Webcam 作成待機中は円を表示
		if (not webcam)
		{
			Rect{ {0, 0}, VIDEO_DISPLAY_SIZE }.draw(Palette::Black);
			Circle{ {VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2}, 100 }
			.drawArc(Scene::Time() * 180_deg, 300_deg, 5, 5, Palette::White);
			FontAsset(U"TextFont")(U"カメラ起動中").drawAt({ VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2 - 20 }, Palette::White);
			FontAsset(U"TextFont")(getData().cameraName).drawAt({ VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2 + 20 }, Palette::White);
		}
		if (texture)
		{
			texture.resized(960, 540).draw();

			Rect{ ABILITY_TEXT_AREA_POS / 2, ABILITY_TEXI_AREA_SIZE / 2 }.drawFrame(1, Palette::Red);

			for (int i = 0; i < 4; i++)
			{
				Rect{ clipStatusPosList[i] / 2, STATUS_ICON_SIZE / 2 }.drawFrame(1, Palette::Green);
			}
		}

		// 目的のアクセサリを表示
		for (int i = 0; i < getData().desiredAccessories.size(); i++)
		{
			FontAsset(U"TextFont")(getData().desiredAccessories[i].getDiscriptionJP()).draw(1000, 80 + i * 30);
			for (int j = 0; j < 4; j++)
			{
				FontAsset(U"TextFont")(getData().desiredAccessories[i].getStatusList()[j]).drawAt(1600 + j * 70, 90 + i * 30);
			}
		}

		// 認識したアクセサリを表示

		const size_t recognizedAccessoriesSize = RecognizedAccessories.size();
		for (int i = 0; i < recognizedAccessoriesSize; i++)
		{
			const Accsessorie& acc = RecognizedAccessories[recognizedAccessoriesSize - i - 1];
			FontAsset(U"TextFont")(acc.getDiscriptionJP()).draw(30, 550 + i * 30);
			for (int j = 0; j < 4; j++)
			{
				FontAsset(U"TextFont")(acc.getStatusList()[j]).drawAt(630 + j * 70, 560 + i * 30);
			}
		}

		joyconGUI.draw();
		Print << Cursor::Pos();
	}
};

void Main()
{
	Window::SetTitle(U"METAL GAY SOLID 3");
	Window::Resize(1920, 900);
	Window::Centering();
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);
	FontAsset::Register(U"SubtitleFont", 30, Typeface::Heavy);
	FontAsset::Register(U"TextFont", 15, Typeface::Bold);

	// シーンマネージャーを作成
	// ここで GameData が初期化される
	App manager;
	manager
		.add<Loading>(U"Loading")
		.add<Setting>(U"Setting")
		.add<Recording>(U"Recording");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

