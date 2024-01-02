# pragma once
# include "Common.hpp"
# include "Pulldown.hpp"
# include "OpenableListBox.hpp"

// 設定シーン
class Setting : public App::Scene
{

public:
	const Font font{ font_size };

	Array<Pulldown> accessoryPulldowns;
	Array<Pulldown> statusTypePulldowns;
	Array<String> webcams = { U"未選択" };
	Pulldown cameraPulldown;
	Pulldown serialPulldown;

	
	// const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
	Array<String> options = { U"未選択" };

	Setting(const InitData& init);

	void assignDesiredAccessories() const;

	void desiredAccessories_to_pullDowns();

	void placePulldowns();

	void placeAbilityValuesPulldowns();

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

	Array<double> sumProbabilityList = { 0, 0, 0, 0 };

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

	Vec2 accPulldownTablePos = { MENU_X, ACCSESSORIE_TEXT_Y + 50 };

	SimpleTable probabilityTable{ {PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH, PROBABILITY_CELL_WIDTH}, {
		.cellHeight = 30,
		.variableWidth = true,
		.font = tableFont,
		}
	};

	Point probabilityTablePos = { MENU_X + int(ACCESSORIES_CELL_WIDTH + STATUS_CELL_WIDTH + PROBABILITY_CELL_WIDTH * 4),  ACCSESSORIE_TEXT_Y + 50 };

	Array<OpenableListBox> openableListBoxesAccessory;

	HashTable<std::pair<StatusType, AccessoryType>, double> statusTypeLotteryRateTable = {
		{{StatusType::Unselected, AccessoryType::Undefined}, 1},
		{{StatusType::Unselected, AccessoryType::Blacelet}, 1},
		{{StatusType::Unselected, AccessoryType::Ring}, 1},
		{{StatusType::Unselected, AccessoryType::Necklace}, 1},
		{{StatusType::Unselected, AccessoryType::Crown}, 1},
		{{StatusType::HP, AccessoryType::Undefined}, 1},
		{{StatusType::HP, AccessoryType::Blacelet}, 0.25},
		{{StatusType::HP, AccessoryType::Ring}, 0.125},
		{{StatusType::HP, AccessoryType::Necklace}, 0.1875},
		{{StatusType::HP, AccessoryType::Crown}, 0.1429},
		{{StatusType::Attack, AccessoryType::Undefined}, 1},
		{{StatusType::Attack, AccessoryType::Blacelet}, 0.125},
		{{StatusType::Attack, AccessoryType::Ring}, 0.25},
		{{StatusType::Attack, AccessoryType::Necklace}, 0.1875},
		{{StatusType::Attack, AccessoryType::Crown}, 0.1429},
		{{StatusType::HealingPower, AccessoryType::Undefined}, 1},
		{{StatusType::HealingPower, AccessoryType::Blacelet}, 0.0625},
		{{StatusType::HealingPower, AccessoryType::Ring}, 0.0625},
		{{StatusType::HealingPower, AccessoryType::Necklace}, 0.25},
		{{StatusType::HealingPower, AccessoryType::Crown}, 0.1429},
		{{StatusType::Dexterity, AccessoryType::Undefined}, 1},
		{{StatusType::Dexterity, AccessoryType::Blacelet}, 0.125},
		{{StatusType::Dexterity, AccessoryType::Ring}, 0.1875},
		{{StatusType::Dexterity, AccessoryType::Necklace}, 0.125},
		{{StatusType::Dexterity, AccessoryType::Crown}, 0.1429},
		{{StatusType::Agility, AccessoryType::Undefined}, 1},
		{{StatusType::Agility, AccessoryType::Blacelet}, 0.1875},
		{{StatusType::Agility, AccessoryType::Ring}, 0.125},
		{{StatusType::Agility, AccessoryType::Necklace}, 0.0625},
		{{StatusType::Agility, AccessoryType::Crown}, 0.1429},
		{{StatusType::BlockRate, AccessoryType::Undefined}, 1},
		{{StatusType::BlockRate, AccessoryType::Blacelet}, 0.1875},
		{{StatusType::BlockRate, AccessoryType::Ring}, 0.0625},
		{{StatusType::BlockRate, AccessoryType::Necklace}, 0.0625},
		{{StatusType::BlockRate, AccessoryType::Crown}, 0.1429},
		{{StatusType::CriticalRate, AccessoryType::Undefined}, 1},
		{{StatusType::CriticalRate, AccessoryType::Blacelet}, 0.0625},
		{{StatusType::CriticalRate, AccessoryType::Ring}, 0.1875},
		{{StatusType::CriticalRate, AccessoryType::Necklace}, 0.125},
		{{StatusType::CriticalRate, AccessoryType::Crown}, 0.1429},
	};
};
