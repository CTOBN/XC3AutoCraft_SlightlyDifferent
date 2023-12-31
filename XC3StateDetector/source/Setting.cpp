# include "Setting.hpp"


Setting::Setting(const InitData& init)
	: IScene{ init }

{
	Array<String> DescriptionDetailJPList = { U"未選択" };
	DescriptionDetailJPList.append(Accessory::getDescriptionDetailJPList());

	for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		accessoryPulldowns.push_back(Pulldown{ DescriptionDetailJPList, ACCSESSORIE_FONT, Point{MENU_X, ACCSESSORIE_TEXT_Y + 50 + (ACCESSORIES_FONT_SIZE + 15) * (i)} });
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
			abilityValuesPulldowns.push_back(Pulldown{ {U"", U"HP", U"攻撃力", U"回復力", U"器用さ", U"素早さ", U"ガード率", U"ｸﾘﾃｨｶﾙ率"}, ACCSESSORIE_FONT, Point{ABILITY_VALUE_TEXT_X + font_size * x * 6, ACCSESSORIE_TEXT_Y + font_size * (y + 1) * 2 + line_padding} });
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

	probabilityTable.push_back_row({ U"互換", U"腕輪", U"指輪", U"首飾", U"冠" }, { 0, 0, 0, 0, 0 });
	probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"-", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });
	probabilityTable.push_back_row({ U"合計", U"0.00", U"0.00", U"0.00", U"0.00" }, { 0, 1, 1, 1, 1 });

	placeAbilityValuesPulldowns();

}

void Setting::assignDesiredAccessories() const
{
	getData().desiredAccessories.clear();
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		if (accessoryPulldowns[i].getIndex() == 0)
		{
			continue;
		}
		Accessory acc{ accessoryPulldowns[i].getIndex() - 1 };

		for (size_t j = 0; j < 4; j++)
		{
			StatusType statusType1 = static_cast<StatusType>(abilityValuesPulldowns[i * 4 + j].getIndex());
			acc.setStatusBoost(StatusBoost{ statusType1 }, j);
		}
	
		getData().desiredAccessories.push_back(acc);
	}
}

void Setting::desiredAccessories_to_pullDowns()
{
	for (size_t i = 0; i < getData().desiredAccessories.size(); i++)
	{
		Accessory& acc = getData().desiredAccessories[i];
		accessoryPulldowns[i].setIndex(acc.getIndex() + 1);

		Array<StatusBoost> statusBoosts = acc.getStatusBoosts();
		for (size_t j = 0; j < 4; j++)
		{
			int index = static_cast<int>(statusBoosts[j].type);
			abilityValuesPulldowns[i * 4 + j].setIndex(index);
		}
	}
}

void Setting::calculateSumProbability()
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
		size_t index = accessoryPulldowns[i].getIndex() - 1;
		sumProbabilityWrists += Parse<double>(Accessory::getProbabilityWrist(index));
		sumProbabilityFingers += Parse<double>(Accessory::getProbabilityFinger(index));
		sumProbabilityNecklaces += Parse<double>(Accessory::getProbabilityNecklaces(index));
		sumProbabilityCrowns += Parse<double>(Accessory::getProbabilityCrowns(index));
	}
}


void Setting::placePulldowns()
{
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		RectF region = accPulldownTable.cellRegion(accPulldownTablePos, i + 1, 0);
		Point pos = region.pos.asPoint();
		accessoryPulldowns[i].setPos(pos);
	}
}

void Setting::placeAbilityValuesPulldowns()
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

void Setting::setProbability()
{
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		if (accessoryPulldowns[i].getIndex() == 0)
		{
			probabilityTable.setText(i + 1, 0, U"-");
			probabilityTable.setText(i + 1, 1, U"0.00");
			probabilityTable.setText(i + 1, 2, U"0.00");
			probabilityTable.setText(i + 1, 3, U"0.00");
			probabilityTable.setText(i + 1, 4, U"0.00");
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
	return getData().desireConsecutiveStatus || (sumProbabilityWrists > 0 && sumProbabilityFingers > 0 && sumProbabilityNecklaces > 0 && sumProbabilityCrowns > 0);
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
	FontAsset(U"TextFont")(serialSelectionStatus).draw(SERIAL_TEXT_X, SERIAL_TEXT_Y + 40, serialSelectionStatusColor);
	FontAsset(U"TextFont")(serialConnectionStatus).draw(SERIAL_TEXT_X + 200, SERIAL_TEXT_Y + 40, serialConnectionStatusColor);
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
		probabilityTable.cellRegion(probabilityTablePos, 0, getData().accessoryTypeIndex).drawFrame(4, Palette::Red);
	}

	if (getData().cameraName == U"未選択")
	{
		FontAsset(U"TextFont")(U"カメラを選択してください").draw(CAMERA_TEXT_X, CAMERA_TEXT_Y + 40, Palette::Red);
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
	getData().cameraIndex = static_cast<uint32>(cameraPulldown.getIndex() - 1);
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

void Setting::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);


	SimpleGUI::CheckBox(getData().desireConsecutiveStatus, U"特殊効果にかかわらず全て同じ種類のステータス増加のアクセサリも希望する", Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y });

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

	if (canGoRecording() && SimpleGUI::Button(U"決定", Scene::Center()))
	{
		assignDesiredAccessories();
		goRecording = true;
	}
	drawMouseOver();
}


