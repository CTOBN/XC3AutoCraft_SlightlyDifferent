# pragma once
# include "Common.hpp"
# include "Pulldown.hpp"
# include "OpenableListBox.hpp"

// 設定シーン
class Setting : public App::Scene
{

public:
	const Font font{ font_size };

	//Array<Pulldown> accessoryPulldowns;
	//Array<Pulldown> statusTypePulldowns;
	Array<String> webcams = { U"未選択" };

	
	// const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
	Array<String> options = { U"未選択" };

	Setting(const InitData& init);

	void assignDesiredAccessories() const;

	void desiredAccessories_to_pullDowns();

	void setProbability();

	void selectAccTypeButtonUpdate();

	void drawMouseOver() const;

	bool canMake() const;

	bool isSelectedSerialPort() const;

	bool isSelectedCamera() const;

	bool canGoRecording() const;

	void serialUpdate();

	void drawSerialStatus() const;

	void drawNotion() const;

	void update() override;

	void draw() const override;

	void csvFileToDesiredAccessories(FilePathView path);

	void openDesiredAccessories();

	void saveDesiredAccessories();

private:
	const int DESIRE_CONSENCUTIVE_STATUS_Y = 530;
	const double ACCSESSORIE_TEXT_Y = 50;

	const double MENU_X = 10;
	const int font_size = 15;
	const int ACCESSORIES_FONT_SIZE = 20;
	const int CAMERA_FONT_SIZE = 18;
	const int line_padding = 20;

	const Vec2 CameraTextPos = { MENU_X, 600 };
	const Vec2 SerialTextPos = { MENU_X, 750 };
	const Vec2 GoRecordingButtonPos = { 1650, 850 };

	const Font ACCSESSORIE_FONT{ ACCESSORIES_FONT_SIZE };
	const Font CAMERA_FONT{ CAMERA_FONT_SIZE };
	const Font SERIAL_FONT{ CAMERA_FONT_SIZE };

	const int ABILITY_VALUE_TEXT_X = 820;
	const int INFO_TEXT_X = 1200;
	const int INFO_TEXT_WIDTH = 80;

	const size_t TARGET_ACCSESORIES_COUNT_MAX = 10;

	Array<double> sumProbabilityList = { 0, 0, 0, 0 };

	const double ACCESSORIES_CELL_WIDTH = 800;
	const double STATUS_CELL_WIDTH = 115;
	const double AlreadyCellWidth = 80;
	const double PROBABILITY_CELL_WIDTH = 130;

	const Font tableFont{ FontMethod::MSDF, 36 };

	String serialSelectionStatus = U"シリアルポートを選択してください";
	ColorF serialSelectionStatusColor = Palette::Red;

	const double outlineScale = 0.5;
	const ColorF outlineColor = Palette::White;

	mutable String serialConnectionStatus = U"";
	mutable ColorF serialConnectionStatusColor = Palette::Red;
	mutable bool goRecording = false;

	SimpleTable accPulldownTable{ { ACCESSORIES_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH, STATUS_CELL_WIDTH}, {
		.cellHeight = 30,
		.variableWidth = true,
		.font = tableFont,

		}
	};

	Vec2 accPulldownTablePos = { MENU_X, ACCSESSORIE_TEXT_Y + 50 };

	SimpleTable probabilityTable{ {AlreadyCellWidth, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH}, {
		.cellHeight = 30,
		.variableWidth = true,
		.font = tableFont,
		}
	};

	Vec2 probabilityTablePos = { MENU_X + ACCESSORIES_CELL_WIDTH + STATUS_CELL_WIDTH * 4 + 5, ACCSESSORIE_TEXT_Y + 50};

	Array<OpenableListBox> openableListBoxesAccessory;
	Array<OpenableListBox> openableListBoxesStatusType;
	OpenableListBox	openableListBoxCamera{ CAMERA_FONT, CameraTextPos.movedBy(0, 80), 300, 30, 3};
	OpenableListBox openableListBoxSerial{ SERIAL_FONT, SerialTextPos.movedBy(0, 80), 700, 30, 3 };

	HashTable<std::pair<StatusType, AccessoryType>, double> statusTypeLotteryRateTable = {
		{{StatusType::Anything, AccessoryType::Undefined}, 1},
		{{StatusType::Anything, AccessoryType::Bracelet}, 1},
		{{StatusType::Anything, AccessoryType::Ring}, 1},
		{{StatusType::Anything, AccessoryType::Necklace}, 1},
		{{StatusType::Anything, AccessoryType::Crown}, 1},
		{{StatusType::HP, AccessoryType::Undefined}, 1},
		{{StatusType::HP, AccessoryType::Bracelet}, 0.25},
		{{StatusType::HP, AccessoryType::Ring}, 0.125},
		{{StatusType::HP, AccessoryType::Necklace}, 0.1875},
		{{StatusType::HP, AccessoryType::Crown}, 0.1429},
		{{StatusType::Attack, AccessoryType::Undefined}, 1},
		{{StatusType::Attack, AccessoryType::Bracelet}, 0.125},
		{{StatusType::Attack, AccessoryType::Ring}, 0.25},
		{{StatusType::Attack, AccessoryType::Necklace}, 0.1875},
		{{StatusType::Attack, AccessoryType::Crown}, 0.1429},
		{{StatusType::HealingPower, AccessoryType::Undefined}, 1},
		{{StatusType::HealingPower, AccessoryType::Bracelet}, 0.0625},
		{{StatusType::HealingPower, AccessoryType::Ring}, 0.0625},
		{{StatusType::HealingPower, AccessoryType::Necklace}, 0.25},
		{{StatusType::HealingPower, AccessoryType::Crown}, 0.1429},
		{{StatusType::Dexterity, AccessoryType::Undefined}, 1},
		{{StatusType::Dexterity, AccessoryType::Bracelet}, 0.125},
		{{StatusType::Dexterity, AccessoryType::Ring}, 0.1875},
		{{StatusType::Dexterity, AccessoryType::Necklace}, 0.125},
		{{StatusType::Dexterity, AccessoryType::Crown}, 0.1429},
		{{StatusType::Agility, AccessoryType::Undefined}, 1},
		{{StatusType::Agility, AccessoryType::Bracelet}, 0.1875},
		{{StatusType::Agility, AccessoryType::Ring}, 0.125},
		{{StatusType::Agility, AccessoryType::Necklace}, 0.0625},
		{{StatusType::Agility, AccessoryType::Crown}, 0.1429},
		{{StatusType::BlockRate, AccessoryType::Undefined}, 1},
		{{StatusType::BlockRate, AccessoryType::Bracelet}, 0.1875},
		{{StatusType::BlockRate, AccessoryType::Ring}, 0.0625},
		{{StatusType::BlockRate, AccessoryType::Necklace}, 0.0625},
		{{StatusType::BlockRate, AccessoryType::Crown}, 0.1429},
		{{StatusType::CriticalRate, AccessoryType::Undefined}, 1},
		{{StatusType::CriticalRate, AccessoryType::Bracelet}, 0.0625},
		{{StatusType::CriticalRate, AccessoryType::Ring}, 0.1875},
		{{StatusType::CriticalRate, AccessoryType::Necklace}, 0.125},
		{{StatusType::CriticalRate, AccessoryType::Crown}, 0.1429},
	};

	const Array<std::pair<String, Array<String>>> menus
	{
		{ U"ファイル", { U"開く", U"名前を付けて保存", U"終了" }},
		{ U"設定", { U"完成時にトースト通知する"}},
		{ U"ヘルプ", { U"\U000F0625 Webマニュアル", U"\U000F14F7 リリースノート", U"\U000F0FC3 ライセンス情報" } },
	};
	SimpleMenuBar menuBar{ menus };

	Optional<FilePath> desiredAccessoryOpenPath;
	Optional<FilePath> desiredAccessorySavePath;
	
	Font GoRecordingButtonFont{ 50 };
};
