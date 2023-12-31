# pragma once
# include "Common.hpp"
# include "Pulldown.hpp"

// 設定シーン
class Setting : public App::Scene
{

public:
	const Font font{ font_size };

	Array<Pulldown> accessoryPulldowns;
	Array<Pulldown> abilityValuesPulldowns;
	Array<String> webcams = { U"未選択" };
	Pulldown cameraPulldown;
	Pulldown serialPulldown;
	// const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
	Array<String> options = { U"未選択" };

	Setting(const InitData& init);

	void assignDesiredAccessories() const;

	void desiredAccessories_to_pullDowns();

	void calculateSumProbability();

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

	Point probabilityTablePos = { MENU_X + int(ACCESSORIES_CELL_WIDTH + STATUS_CELL_WIDTH + PROBABILITY_CELL_WIDTH * 4),  ACCSESSORIE_TEXT_Y + 50 };

	HashTable<std::pair<StatusType, int8>, double> probabilityTableValues = {
		{{StatusType::Undefined, -1}, 1},
		{{StatusType::Undefined, 0}, 1},
		{{StatusType::Undefined, 1}, 1},
		{{StatusType::Undefined, 2}, 1},
		{{StatusType::Undefined, 3}, 1},
		{{StatusType::HP, -1}, 0},
		{{StatusType::HP, 0}, 0},
		{{StatusType::HP, 1}, 0},
		{{StatusType::HP, 2}, 0},
		{{StatusType::HP, 3}, 0},
		{{StatusType::Attack, 0}, 0},
		{{StatusType::Attack, 1}, 0},
		{{StatusType::Attack, 2}, 0},
		{{StatusType::Attack, 3}, 0},
		{{StatusType::Attack, 4}, 0},
		{{StatusType::HealingPower, 0}, 0},
		{{StatusType::HealingPower, 1}, 0},
		{{StatusType::HealingPower, 2}, 0},
		{{StatusType::HealingPower, 3}, 0},
		{{StatusType::HealingPower, 4}, 0},
		{{StatusType::Dexterity, 0}, 0},
		{{StatusType::Dexterity, 1}, 0},
		{{StatusType::Dexterity, 2}, 0},
		{{StatusType::Dexterity, 3}, 0},
		{{StatusType::Dexterity, 4}, 0},
		{{StatusType::Agility, 0}, 0},
		{{StatusType::Agility, 1}, 0},
		{{StatusType::Agility, 2}, 0},
		{{StatusType::Agility, 3}, 0},
		{{StatusType::Agility, 4}, 0},
		{{StatusType::BlockRate, 0}, 0},
		{{StatusType::BlockRate, 1}, 0},
		{{StatusType::BlockRate, 2}, 0},
		{{StatusType::BlockRate, 3}, 0},
		{{StatusType::BlockRate, 4}, 0},
		{{StatusType::CriticalRate, 0}, 0},
		{{StatusType::CriticalRate, 1}, 0},
		{{StatusType::CriticalRate, 2}, 0},
		{{StatusType::CriticalRate, 3}, 0},
		{{StatusType::CriticalRate, 4}, 0},
	};
};
