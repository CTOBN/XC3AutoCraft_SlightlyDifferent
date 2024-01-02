﻿# include "Setting.hpp"


Setting::Setting(const InitData& init)
	: IScene{ init }

{
	Array<String> DescriptionDetailJPList = Accessory::getDescriptionJPList();

	for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		openableListBoxesAccessory.push_back(OpenableListBox{ ACCSESSORIE_FONT, accPulldownTablePos.movedBy(0, 31 * (i+1)), ACCESSORIES_CELL_WIDTH + 1, 30, 5 });
	}

	for (auto& openableListBoxAccessory : openableListBoxesAccessory)
	{
		openableListBoxAccessory.setItems(DescriptionDetailJPList);
	}

	for (int y = 0; y < TARGET_ACCSESORIES_COUNT_MAX; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			openableListBoxesStatusType.push_back(OpenableListBox{ ACCSESSORIE_FONT, accPulldownTablePos.movedBy(x * (STATUS_CELL_WIDTH + 1) + ACCESSORIES_CELL_WIDTH, (y+1) * 31), STATUS_CELL_WIDTH + 2, 20, 8});
		}
	}
	for (auto& openableListBoxStatusType : openableListBoxesStatusType)
	{
		openableListBoxStatusType.setItems({ U"任意", U"HP", U"攻撃力", U"回復力", U"器用さ", U"素早さ", U"ガード率", U"ｸﾘﾃｨｶﾙ率" });
	}

	for (const auto& info : System::EnumerateWebcams())
	{
		webcams.push_back(info.name);
	};

	openableListBoxCamera.setItems(webcams);

	for (const auto& info : getData().infos)
	{
		Console << U"{} {}"_fmt(info.hardwareID, info.description);
		options.push_back(U"{} {}"_fmt(info.hardwareID, info.description));
	}

	openableListBoxSerial.setItems(options);

	desiredAccessories_to_pullDowns();

	accPulldownTable.push_back_row({ U"特殊効果", U"ｽﾃｰﾀｽ１", U"ｽﾃｰﾀｽ2", U"ｽﾃｰﾀｽ3", U"ｽﾃｰﾀｽ4" }, { -1, 0, 0, 0, 0 });
	//accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
	//accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
	//accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
	//accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });
	//accPulldownTable.push_back_row({ U"", U"", U"", U"", U"" }, { 0, 0, 0, 0, 0 });

	probabilityTable.push_back_row({ U"互換", U"腕輪", U"指輪", U"首飾", U"冠" }, { 0, 0, 0, 0, 0 });
	probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"合計", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });

}

void Setting::assignDesiredAccessories() const
{
	getData().desiredAccessories.clear();
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		if (openableListBoxesAccessory[i].getSelectedIndex() == 0)
		{
			continue;
		}
		Accessory acc{ openableListBoxesAccessory[i].getSelectedIndex() };

		for (size_t j = 0; j < 4; j++)
		{
			StatusType statusType = static_cast<StatusType>(openableListBoxesStatusType[i * 4 + j].getSelectedIndex());
			acc.setStatusBoost(StatusBoost{ statusType }, j);
		}

		getData().desiredAccessories.push_back(acc);
	}
}

void Setting::desiredAccessories_to_pullDowns()
{
	for (size_t i = 0; i < getData().desiredAccessories.size(); i++)
	{
		Accessory& acc = getData().desiredAccessories[i];
		openableListBoxesAccessory[i].listBoxState.selectedItemIndex = acc.getIndex();

		Array<StatusBoost> statusBoosts = acc.getStatusBoosts();
		for (size_t j = 0; j < 4; j++)
		{
			int index = static_cast<int>(statusBoosts[j].type);
			openableListBoxesStatusType[i * 4 + j].listBoxState.selectedItemIndex = index;
		}
	}
}

void Setting::setProbability()
{
	for (size_t k = 0; k < 4; k++)
	{
		sumProbabilityList[k] = 0;
		probabilityTable.setText(6, k+1, U"{:.5f}"_fmt(sumProbabilityList[k]));
	}

	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		size_t index = openableListBoxesAccessory[i].getSelectedIndex();
		probabilityTable.setText(i + 1, 0, Accessory::getAlready(index));

		// 各アクセサリの種類
		for (size_t j = 1; j < 5; j++)
		{
			AccessoryType accessoryType = static_cast<AccessoryType>(j);
			double probability = Accessory::getProbability(index, accessoryType);

			// 各アクセサリのステータス
			for (size_t k = 0; k < 4; k++)
			{
				StatusType statusType = static_cast<StatusType>(openableListBoxesStatusType[i * 4 + k].getSelectedIndex());
				probability *= statusTypeLotteryRateTable[std::make_pair(statusType, accessoryType)];
			}
			probabilityTable.setText(i + 1, j, U"{:.5f}"_fmt(probability));
			sumProbabilityList[j - 1] += probability;
			sumProbabilityList[j - 1] = Min(sumProbabilityList[j - 1], 100.0);
			probabilityTable.setText(6, j, U"{:.5f}"_fmt(sumProbabilityList[j - 1]));
		}
	}
}


void Setting::selectAccTypeButtonUpdate()
{
	for (size_t i = 0; i < 4; i++)
	{
		RectF r = probabilityTable.cellRegion(probabilityTablePos, 0, i + 1);
		if (r.leftClicked())
		{
			getData().accessoryTypeIndex = static_cast<int8>(i + 1);
		}
	}
}
	void Setting::drawMouseOver() const
	{
		for (size_t i = 0; i < 4; i++)
		{
			RectF r = probabilityTable.cellRegion(probabilityTablePos, 0, i + 1);
			if (r.mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
				r.draw(ColorF{ 0.5, 0.5, 0.5, 0.5 });
			}
		}
	}

bool Setting::canMake() const
{
	double epsilon = 1e-9;
	if (getData().desireConsecutiveStatus) {
		return true;
	}
	else if (getData().accessoryTypeIndex != 0 && sumProbabilityList[getData().accessoryTypeIndex - 1] < epsilon)
	{
		return false;
	}
	return true;
}

bool Setting::isSelectedSerialPort() const
{
	return getData().serialName != U"未選択";
}

bool Setting::isSelectedCamera() const
{
	return getData().cameraName != U"未選択";
}

bool Setting::canGoRecording() const
{
	return getData().accessoryTypeIndex != 0 && isSelectedCamera() && isSelectedSerialPort() && canMake();
}

void Setting::serialUpdate()
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

void Setting::drawSerialStatus() const
{
	FontAsset(U"TextFont")(serialSelectionStatus).draw(SerialTextPos.movedBy(0, 40), serialSelectionStatusColor);
	FontAsset(U"TextFont")(serialConnectionStatus).draw(SerialTextPos.movedBy(200, 40), serialConnectionStatusColor);
}


void Setting::drawNotion() const
{
	if (not canMake())
	{
		FontAsset(U"TextFont")(U"目的のアクセサリが出る可能性は０です").draw(probabilityTablePos.x + PROBABILITY_CELL_WIDTH, probabilityTablePos.y + 30 * 7 + 30, Palette::Red);
	}

	if (getData().accessoryTypeIndex == 0)
	{
		FontAsset(U"TextFont")(U"作るアクセサリを選択してください↓").draw(probabilityTablePos.x + PROBABILITY_CELL_WIDTH, probabilityTablePos.y - 50, Palette::Red);
	}
	else
	{
		probabilityTable.cellRegion(probabilityTablePos, 0, getData().accessoryTypeIndex).drawFrame(4, 0, Palette::Red);
	}

	if (getData().cameraName == U"未選択")
	{
		FontAsset(U"TextFont")(U"カメラを選択してください").draw(CameraTextPos.movedBy(0, 40), Palette::Red);
	}
}


void Setting::update()
{
	if (goRecording)
	{
		changeScene(U"Recording");
	}
	// DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	serialUpdate();
	drawNotion();

	

	getData().cameraIndex = static_cast<uint32>(openableListBoxCamera.getSelectedIndex() - 1);
	getData().cameraName = openableListBoxCamera.getSelectedItem();

	getData().serialIndex = openableListBoxSerial.getSelectedIndex() - 1;
	getData().serialName = openableListBoxSerial.getSelectedItem();

	setProbability();
	selectAccTypeButtonUpdate();
}

void Setting::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);
	

	SimpleGUI::CheckBox(getData().desireConsecutiveStatus, U"特殊効果にかかわらず全て同じ種類のステータス増加のアクセサリも希望する", Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y });

	probabilityTable.draw(probabilityTablePos);
	drawNotion();

	if (isSelectedSerialPort() && SimpleGUI::Button(U"接続テスト", SerialTextPos.movedBy(350, 0)))
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

	FontAsset(U"SubtitleFont")(U"シリアルポート").draw(SerialTextPos);
	drawSerialStatus();

	FontAsset(U"SubtitleFont")(U"カメラ").draw(CameraTextPos);
	openableListBoxCamera.update();
	openableListBoxCamera.draw();

	openableListBoxSerial.update();
	openableListBoxSerial.draw();

	accPulldownTable.draw(accPulldownTablePos);


	for (auto it = std::rbegin(openableListBoxesStatusType); it != std::rend(openableListBoxesStatusType); ++it) {
		auto& openableListBoxStatusType = *it;
		// 他のすべてのメニューが閉じている場合にのみ、このメニューを更新
		if (std::all_of(openableListBoxesStatusType.begin(), openableListBoxesStatusType.end(), [&](const OpenableListBox& m) { return &m == &openableListBoxStatusType || !m.getIsOpen(); }))
		{
			openableListBoxStatusType.update();
		}
		openableListBoxStatusType.draw();
	}

	FontAsset(U"SubtitleFont")(U"希望のアクセサリ").draw(20, ACCSESSORIE_TEXT_Y);


	for (auto it = std::rbegin(openableListBoxesAccessory); it != std::rend(openableListBoxesAccessory); ++it) {
		auto& openableListBoxAccessory = *it;
		// 他のすべてのメニューが閉じている場合にのみ、このメニューを更新
		if (std::all_of(openableListBoxesAccessory.begin(), openableListBoxesAccessory.end(), [&](const OpenableListBox& m) { return &m == &openableListBoxAccessory || !m.getIsOpen(); }))
		{
			openableListBoxAccessory.update();
		}
		openableListBoxAccessory.draw();
	}




	if (canGoRecording() && SimpleGUI::Button(U"決定", Scene::Center()))
	{
		assignDesiredAccessories();
		goRecording = true;
	}
	drawMouseOver();
}


