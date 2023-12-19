# include <Siv3D.hpp>
# include "Pulldown.hpp"
# include "Accessory.hpp"
# include "StatusType.hpp"
# include "StatusBoost.hpp"
# include "Context.hpp"
# include "State.hpp"
# include "Title.hpp"
# include "Make.hpp"
# include "AccessorySelection.hpp"
# include "Judge.hpp"

# include "VirtualJoyCon.hpp"
# include "ButtonByte.hpp"

constexpr int STATUS_ICON_NUM = 7;
constexpr Point STATUS_ICON_SIZE = { 30, 30 };

/// @brief 斜めのストライプの背景を描画します。
/// @param width ストライプの幅
/// @param angle ストライプの角度
/// @param color ストライプの色
void DrawStripedBackground(int32 width, double angle, const ColorF& color)
{
	for (int32 x = -Scene::Height(); x < (Scene::Width() + Scene::Height()); x += (width * 2))
	{
		Rect{ x, 0, width, Scene::Height() }.skewedX(angle).draw(color);
	}
}

/// @brief 上下方向のグラデーションの背景を描画します。
/// @param topColor 上部の色
/// @param bottomColor 下部の色
void DrawVerticalGradientBackground(const ColorF& topColor, const ColorF& bottomColor)
{
	Scene::Rect()
		.draw(Arg::top = topColor, Arg::bottom = bottomColor);
}


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

	const Array<String> statusTypeFileName = {U"HP", U"Attack", U"HealingPower", U"Dexterity", U"Agility", U"BlockRate",  U"CriticalRate"};
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
	int8 accsessoryTypeIndex = -1; // 0:腕輪, 1:指輪, 2:首飾, 3:冠

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
	const CSV csv{ U"csv/accessory.csv" };
	const String discriptionJPTemplateFolderPath = U"images/Xenoblade3_Temp_Jp";
	const String statusIconsFolderPath = U"images/StatusType";
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

		for (size_t row = 1; row < csv.rows(); ++row) // 1行目はヘッダなので飛ばす
		{
			Accessory::pushBackID(Parse<uint16>(csv[row][0]));
			Accessory::pushBackDiscriptionEN(csv[row][1]);
			Accessory::pushBackDiscriptionJP(csv[row][2]);
			Accessory::pushBackDiscriptionDetailJP(csv[row][3]);
			Accessory::pushBackAlready(csv[row][4]);
			Accessory::pushBackWrist(csv[row][5]);
			Accessory::pushBackFinger(csv[row][6]);
			Accessory::pushBackNecklaces(csv[row][7]);
			Accessory::pushBackCrowns(csv[row][8]);
		}

		for (uint16 i = 3428; i <= 3913; i += 5)
		{
			String path = discriptionJPTemplateFolderPath + U"/" + Format(i) + U".jpg";
			discriptionsImagesPathList.push_back(path);
			getData().binarizedAbilities.push_back(Image{ path }.thresholded(128));
			Console << path << U" を読み込みました";
		}

		for (String statusIconFileName : getData().statusTypeFileName)
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
		DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
		Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);

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

	String serialSelectionStatus = U"シリアルポートを選択してください";
	ColorF serialSelectionStatusColor = Palette::Red;

	mutable String serialConnectionStatus = U"";
	mutable ColorF serialConnectionStatusColor = Palette::Red;
	mutable bool goRecording = false;

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

	Array<Pulldown> accessoryPulldowns;
	Array<Pulldown> abilityValuesPulldowns;
	Array<String> webcams = { U"未選択" };
	Pulldown cameraPulldown;
	Pulldown serialPulldown;
	// const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
	Array<String> options = { U"未選択" };

	Setting(const InitData& init)
		: IScene{ init }

	{
		Array<String> DiscriptionDetailJPList = {U"未選択"};
		DiscriptionDetailJPList.append(Accessory::getDiscriptionDetailJPList());

		for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			accessoryPulldowns.push_back(Pulldown{ DiscriptionDetailJPList, ACCSESSORIE_FONT, Point{MENU_X, ACCSESSORIE_TEXT_Y + 50 + (ACCESSORIES_FONT_SIZE + 15) * (i)}});
		}
		

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
		desiredAccessories_to_pullDowns();

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
	void assignDesiredAccessories() const
	{
		getData().desiredAccessories.clear();
		for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			if (accessoryPulldowns[i].getIndex() == 0)
			{
				continue;
			}
			Accessory acc{ accessoryPulldowns[i].getIndex() - 1 };

			for (int j = 0; j < 4; j++)
			{
				StatusType statusType1 = static_cast<StatusType>(abilityValuesPulldowns[i * 4 + j].getIndex());
				acc.setStatusBoost(StatusBoost{ statusType1 }, j);
			}
			
			getData().desiredAccessories.push_back(acc);
		}
	}

	void desiredAccessories_to_pullDowns()
	{
		for (size_t i = 0; i < getData().desiredAccessories.size(); i++)
		{
			Accessory &acc = getData().desiredAccessories[i];
			accessoryPulldowns[i].setIndex(acc.getIndex() + 1);

			Array<StatusBoost> statusBoosts = acc.getStatusBoosts();
			for (size_t j = 0; j < 4; j++)
			{
				int index = static_cast<int>(statusBoosts[j].type);
				abilityValuesPulldowns[i * 4 + j].setIndex(index);
			}
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
			if (accessoryPulldowns[i].getIndex() == 0)
			{
				continue;
			}
			int index = accessoryPulldowns[i].getIndex() - 1;
			sumProbabilityWrists += Parse<double>(Accessory::getProbabilityWrist(index));
			sumProbabilityFingers += Parse<double>(Accessory::getProbabilityFinger(index));
			sumProbabilityNecklaces += Parse<double>(Accessory::getProbabilityNecklaces(index));
			sumProbabilityCrowns += Parse<double>(Accessory::getProbabilityCrowns(index));
		}
	}


	void placePulldowns()
	{
		for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
		{
			RectF region = accPulldownTable.cellRegion(accPulldownTablePos, i+1, 0);
			Point pos = region.pos.asPoint();
			accessoryPulldowns[i].setPos(pos);
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
			if (accessoryPulldowns[i].getIndex() == 0)
			{
				continue;
			}
			size_t index = accessoryPulldowns[i].getIndex() - 1;
			probabilityTable.setText(i + 1, 0, Accessory::getAlready(index));
			probabilityTable.setText(i + 1, 1, Accessory::getProbabilityWrist(index));
			probabilityTable.setText(i + 1, 2, Accessory::getProbabilityFinger(index));
			probabilityTable.setText(i + 1, 3, Accessory::getProbabilityNecklaces(index));
			probabilityTable.setText(i + 1, 4, Accessory::getProbabilityCrowns(index));
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

	bool isSelectedSerialPort() const
	{
		return getData().serialName != U"未選択";
	}

	bool isSelectedCamera() const
	{
		return getData().cameraName != U"未選択";
	}

	bool canGoRecording() const
	{
		return getData().accsessoryTypeIndex != -1 && isSelectedCamera() && isSelectedSerialPort() && canMake();
	}

	void serialUpdate()
	{
		if (isSelectedSerialPort())
		{
			serialSelectionStatus = U"接続テストが可能です(任意)";
			serialSelectionStatusColor = Palette::Green;
		}
		else
		{
			getData().serial = Serial{};
			serialSelectionStatus = U"シリアルポートを選択してください";
			serialSelectionStatusColor = Palette::Red;

			serialConnectionStatus = U"";
		}


	}

	void drawSerialStatus() const
	{
		FontAsset(U"TextFont")(serialSelectionStatus).draw(SERIAL_TEXT_X, SERIAL_TEXT_Y + 40, serialSelectionStatusColor);
		FontAsset(U"TextFont")(serialConnectionStatus).draw(SERIAL_TEXT_X + 200, SERIAL_TEXT_Y + 40, serialConnectionStatusColor);
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
		if (goRecording)
		{
			changeScene(U"Recording");
		}
		// DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
		serialUpdate();
		drawNotion();



		// プルダウンメニューを更新
		// 逆順に更新することで、選択時のクリックによって別のメニューが開いてしまうのを防ぐ
		for (auto it = std::rbegin(accessoryPulldowns); it != std::rend(accessoryPulldowns); ++it) {
			auto& accessoriesPulldown = *it;
			// 他のすべてのメニューが閉じている場合にのみ、このメニューを更新
			if (std::all_of(accessoryPulldowns.begin(), accessoryPulldowns.end(), [&](const Pulldown& m) { return &m == &accessoriesPulldown || !m.getIsOpen(); }))
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
		DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
		Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);


		SimpleGUI::CheckBox(getData().desireConsencutiveStatus, U"特殊効果にかかわらず全て同じ種類のステータス増加のアクセサリも希望する", Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y });

		probabilityTable.draw(probabilityTablePos);
		drawNotion();

		if (isSelectedSerialPort() && SimpleGUI::Button(U"接続テスト", Vec2{ SERIAL_TEXT_X + 200, SERIAL_TEXT_Y + font_size * 3 + line_padding }))
		{
			if (getData().serial.open(getData().infos[getData().serialIndex].port))
			{
				serialConnectionStatus = U"シリアルポートの接続に成功しました";
				serialConnectionStatusColor = Palette::Green;
			}
			else
			{
				serialConnectionStatus = U"シリアルポートの接続に失敗しました";
				serialConnectionStatusColor = Palette::Red;
			}
		}

		FontAsset(U"SubtitleFont")(U"シリアルポート").draw(SERIAL_TEXT_X, SERIAL_TEXT_Y);
		drawSerialStatus();
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
			accessoryPulldowns[TARGET_ACCSESORIES_COUNT_MAX - i - 1].draw();
		}

		Print << Cursor::Pos();

		if (canGoRecording() && SimpleGUI::Button(U"決定", Scene::Center()))
		{
			assignDesiredAccessories();
			goRecording = true;
		}
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
	Accessory currentAccessory{ 0 }; // todo アクセサリの特殊効果もOptionalにする?
	Array<Accessory> RecognizedAccessories;

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
	VirtualJoyCon virtualJoyCon{ getData().serial, JOYCON_GUI_POS_Left, JOYCON_GUI_POS_Right };
	Array<uint8> currentSerialBytes;
	Array<uint8> SerialBytesLog;

	HashTable<int8, uint8> accsessoryTypeIndexToCommandByte = {
		{0, xc3::Context::CommandByte::SetAccTypeAsWrist},
		{1, xc3::Context::CommandByte::SetAccTypeAsFinger},
		{2, xc3::Context::CommandByte::SetAccTypeAsNecklaces},
		{3, xc3::Context::CommandByte::SetAccTypeAsCrowns}
	};

	HashTable<uint8, String> commandByteToString = {
		{xc3::Context::CommandByte::Title_to_FieldLoading, U"Title_to_FieldLoading"},
		{xc3::Context::CommandByte::Field_to_Camp, U"Field_to_Camp"},
		{xc3::Context::CommandByte::Camp_to_AccessorySelected, U"Camp_to_AccessorySelected"},
		{xc3::Context::CommandByte::AccessorySelected_to_Judge, U"AccessorySelected_to_Judge"},
		{xc3::Context::CommandByte::Judge_to_AccessorySelected, U"Judge_to_AccessorySelected"},
		{xc3::Context::CommandByte::Judge_to_MainMenu, U"Judge_to_MainMenu"},
		{xc3::Context::CommandByte::MainMenu_to_SystemMenu, U"MainMenu_to_SystemMenu"},
		{xc3::Context::CommandByte::SystemMenu_to_TitleLoading, U"SystemMenu_to_TitleLoading"},
		{xc3::Context::CommandByte::SetAccTypeAsWrist, U"SetAccTypeAsWrist"},
		{xc3::Context::CommandByte::SetAccTypeAsFinger, U"SetAccTypeAsFinger"},
		{xc3::Context::CommandByte::SetAccTypeAsNecklaces, U"SetAccTypeAsNecklaces"},
		{xc3::Context::CommandByte::SetAccTypeAsCrowns, U"SetAccTypeAsCrowns"},
	};

	xc3::Context context{ getData().serial };

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

	Array<StatusBoost> findMostSimilarStatusBoost()
	{
		Array<StatusBoost> result;
		webcam.getFrame(image);

		for (int i = 0; i < 4; i++)
		{
			StatusType judgedStatusType = StatusType::Undefined;
			double similarityMax = 0;
			Image clipedImage = image.clipped(clipStatusPosList[i], STATUS_ICON_SIZE).thresholded(128);
			for (int8 j = 0; j < getData().icons.size(); j++)
			{
				Image binarizedStatusTypeImage = getData().binarizedIcons[j];

				double similarity = calculateSimilarity(clipedImage, binarizedStatusTypeImage);
				Console << Format(similarity);
				if (similarity > similarityMax)
				{
					similarityMax = similarity;
					judgedStatusType = static_cast<StatusType>(j + 1);
				}
			}
			result.push_back(StatusBoost{ judgedStatusType });
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
		if (0 <= judgedIndex && judgedIndex < Accessory::getDiscriptionDetailJPList().size())
		{
			currentAccAbilityJP = Accessory::getDiscriptionDetailJP(judgedIndex);
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
		Array<StatusBoost> judgedStatusBoosts = findMostSimilarStatusBoost();

		if (judgedAccIndex == -1)
		{
			return;
		}
		currentAccessory = Accessory{ judgedAccIndex };
		currentAccessory.setStatusBoosts(judgedStatusBoosts);
		RecognizedAccessories.push_back(currentAccessory);
	}

	bool compareAccessories()
	{
		for (int i = 0; i < getData().desiredAccessories.size(); i++)
		{
			Accessory &desiredAcc = getData().desiredAccessories[i];
			if (currentAccessory.getIndex() == desiredAcc.getIndex() && currentAccessory.hasSameStatusTypeOrMore(desiredAcc))
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

	void receiveSerialBytes()
	{
		if (getData().serial.available())
		{
			currentSerialBytes = getData().serial.readBytes();
		}
		if (currentSerialBytes.size() == 0)
		{
			return;
		}
		SerialBytesLog.append(currentSerialBytes);
		currentSerialBytes.clear();
	}

	void drawSerialBytesLog() const
	{
		size_t logSize = SerialBytesLog.size();
		for (size_t i = 0; i < logSize; i++)
		{
			FontAsset(U"TextFont")(SerialBytesLog[logSize - i - 1]).draw(1000, 300 + i * 30);
			uint8 commandByte = SerialBytesLog[logSize - i - 1];
			if (commandByteToString.contains(commandByte))
			{
				String commandName = commandByteToString.at(commandByte);
				FontAsset(U"TextFont")(commandName).draw(1100, 300 + i * 30);
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
		if (context.getCurrentStateName() != U"Undefined")
		{
			context.request();
		}

		if (context.getCurrentStateName() == U"Judge")
		{
			if (not context.wasJudged)
			{
				recognizeAccessory();
				context.wasJudged = true;

				if (completeMission())
				{
					context.gotDesiredAccesory = true;
					Console << U"目的のアクセサリが完成しました";
					Console << Accessory::getDiscriptionDetailJP(currentAccessory.getIndex());
				}
			}
		}

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

		if (SimpleGUI::Button(U"シリアルポートを開く", Vec2{ buttonPosX, buttonPosY }))
		{
			openSerialPort();
		}

		if (SimpleGUI::Button(U"開始前に押す", Vec2{ buttonPosX, buttonPosY +50}))
		{
			size_t unknownMatterCount = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_TENS_PLACE_POS) * 10 + findMostSimilarNumber(UNKOWN_MATTER_NUMBER_ONES_PLACE_POS);
			Console << U"初期ｱﾝﾉｳﾝﾏﾀｰの数は" << unknownMatterCount << U"個です";
			context.initialUnkownMatterCount = unknownMatterCount;
			context.currentUnknownMatterCount = context.initialUnkownMatterCount;

			uint8 setAccType = accsessoryTypeIndexToCommandByte[getData().accsessoryTypeIndex];
			getData().serial.writeByte(setAccType);
		}

		if (SimpleGUI::Button(U"自動クラフト開始", Vec2{ buttonPosX, buttonPosY + 100 }))
		{
			context.init();
			context.setState(std::make_unique<xc3::AccessorySelection>());
		}
		if (webcam && SimpleGUI::Button(U"アクセサリ認識テスト", Vec2{ buttonPosX, buttonPosY + 150 }))
		{
			recognizeAccessory();
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
		if (SimpleGUI::Button(U"debug", Vec2{ buttonPosX, buttonPosY + 300 }))
		{
			context.debugPrint();
		}
		if (SimpleGUI::Button(U"タイトルから", Vec2{ buttonPosX, buttonPosY + 350 }))
		{
			context.initialUnkownMatterCount = 99;
			context.currentUnknownMatterCount = context.initialUnkownMatterCount;
			uint8 setAccType = accsessoryTypeIndexToCommandByte[getData().accsessoryTypeIndex];
			getData().serial.writeByte(setAccType);
			context.init();
			context.setState(std::make_unique<xc3::Title>());
		}
		if (SimpleGUI::Button(U"Judge後、リセット前", Vec2{ buttonPosX, buttonPosY + 400 }))
		{
			context.wasJudged = true;
			context.gotDesiredAccesory = false;
			context.initialUnkownMatterCount = 99;
			context.currentUnknownMatterCount = 0;
			uint8 setAccType = accsessoryTypeIndexToCommandByte[getData().accsessoryTypeIndex];
			getData().serial.writeByte(setAccType);
			context.setState(std::make_unique<xc3::Judge>());
		}

		virtualJoyCon.update();
		receiveSerialBytes();
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
			Accessory &acc = getData().desiredAccessories[i];
			FontAsset(U"TextFont")(Accessory::getDiscriptionJP(acc.getIndex())).draw(1000, 80 + i * 30);
			for (int j = 0; j < 4; j++)
			{
				FontAsset(U"TextFont")(StatusTypeToString[U"JP"][acc.getStatusBoosts()[j].type]).drawAt(1600 + j * 70, 90 + i * 30);
			}
		}

		// 認識したアクセサリを表示

		const size_t recognizedAccessoriesSize = RecognizedAccessories.size();
		for (int i = 0; i < recognizedAccessoriesSize; i++)
		{
			const Accessory& acc = RecognizedAccessories[recognizedAccessoriesSize - i - 1];
			FontAsset(U"TextFont")(Accessory::getDiscriptionJP(acc.getIndex())).draw(30, 550 + i * 30);
			for (int j = 0; j < 4; j++)
			{
				FontAsset(U"TextFont")(StatusTypeToString[U"JP"][acc.getStatusBoosts()[j].type]).drawAt(630 + j * 70, 560 + i * 30);
			}
		}
		virtualJoyCon.draw();

		// 現在の状態を表示
		FontAsset(U"TextFont")(U"現在の状態" + context.getCurrentStateName()).draw(1000, 200);
		FontAsset(U"TextFont")(context.currentUnknownMatterCount).draw(1000, 230);
		drawSerialBytesLog();


	}
};

void Main()
{
	Window::SetTitle(U"XC3StateDetector");
	Window::SetStyle(WindowStyle::Sizable);
	Window::Maximize();
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

