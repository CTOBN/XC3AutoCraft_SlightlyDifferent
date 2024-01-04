# include "Setting.hpp"


Setting::Setting(const InitData& init)
	: IScene{ init }

{
	Array<String> DescriptionDetailJapaneseList = Accessory::getDescriptionJapaneseList();

	for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		openableListBoxesAccessory.push_back(OpenableListBox{ ACCSESSORIE_FONT, accPulldownTablePos.movedBy(0, 31 * (i+1)), ACCESSORIES_CELL_WIDTH + 1, 30, 5 });
	}

	for (auto& openableListBoxAccessory : openableListBoxesAccessory)
	{
		openableListBoxAccessory.setItems(DescriptionDetailJapaneseList);
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
		openableListBoxStatusType.setItems(StatusTypeStringList[U"Japanese"]);
	}

	for (const auto& info : System::EnumerateWebcams())
	{
		webcams.push_back(info.name);
	};

	openableListBoxCamera.setItems(webcams);

	for (const auto& info : getData().infos)
	{
		// Console << U"{} {}"_fmt(info.hardwareID, info.description);

		options.push_back(U"{} {}"_fmt(info.hardwareID, info.description));
	}

	openableListBoxSerial.setItems(options);

	desiredAccessories_to_pullDowns();

	accPulldownTable.push_back_row({ U"特殊効果", U"ｽﾃｰﾀｽ１", U"ｽﾃｰﾀｽ2", U"ｽﾃｰﾀｽ3", U"ｽﾃｰﾀｽ4" }, { -1, 0, 0, 0, 0 });

	probabilityTable.push_back_row({ U"互換", U"腕輪", U"指輪", U"首飾", U"冠" }, { 0, 0, 0, 0, 0 });
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	}
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

void Setting::csvFileToDesiredAccessories(FilePathView path)
{
	getData().desiredAccessories.clear();
	const CSV csv(path);
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		size_t index = 0;
		for (size_t j = 0; j < Accessory::getDescriptionEnglishList().size(); j++)
		{
			if (Accessory::getDescriptionEnglishList()[j] == csv.get<String>(i, 0))
			{
				index = j;
				break;
			}
		}
		// csvファイルのアクセサリの名前が不正な場合
		if (index == Accessory::getDescriptionEnglishList().size())
		{
			continue;
		}

		Accessory acc{ index };
		for (size_t j = 0; j < 4; j++)
		{
			String statusTypeString = csv.get<String>(i, j + 1);
			// csvファイルのステータスの名前が不正な場合、任意として扱う
			StatusType statusType = StatusType::Anything;
			if (StringToStatusType.contains(statusTypeString))
			{
				statusType = StringToStatusType[statusTypeString];
			}
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
			probabilityTable.setText(TARGET_ACCSESORIES_COUNT_MAX + 1, j, U"{:.5f}"_fmt(sumProbabilityList[j - 1]));
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
			getData().selectedAccessoryType = static_cast<AccessoryType>(i + 1);
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
	else if (getData().selectedAccessoryType != AccessoryType::Undefined && sumProbabilityList[static_cast<size_t>(getData().selectedAccessoryType) - 1] < epsilon)
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
	return getData().selectedAccessoryType != AccessoryType::Undefined && isSelectedCamera() && isSelectedSerialPort() && canMake();
}

void Setting::serialUpdate()
{
	if (isSelectedSerialPort())
	{
		serialSelectionStatus = U"接続テストが可能です(任意)→";
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
		FontAsset(U"TextFont")(U"目的のアクセサリが出る可能性は０です").draw(probabilityTablePos.movedBy(70, -50), Palette::Red);
	}

	if (getData().selectedAccessoryType == AccessoryType::Undefined)
	{
		FontAsset(U"TextFont")(U"作るアクセサリを選択してください↓").draw(probabilityTablePos.movedBy(70, -50), Palette::Red);
	}
	else
	{
		probabilityTable.cellRegion(probabilityTablePos, 0, static_cast<size_t>(getData().selectedAccessoryType)).drawFrame(4, 0, Palette::Red);
	}

	if (getData().cameraName == U"未選択")
	{
		FontAsset(U"TextFont")(U"カメラを選択してください").draw(CameraTextPos.movedBy(0, 40), Palette::Red);
	}
}

void Setting::openDesiredAccessories()
{
	desiredAccessoryOpenPath = Dialog::OpenFile({ FileFilter::CSV() });
	if (desiredAccessoryOpenPath)
	{
		csvFileToDesiredAccessories(desiredAccessoryOpenPath.value());
		desiredAccessories_to_pullDowns();
	}
}

void Setting::saveDesiredAccessories()
{
	desiredAccessorySavePath = Dialog::SaveFile({ FileFilter::CSV() });
	if (desiredAccessorySavePath)
	{
		assignDesiredAccessories();
		CSV csv;

		for (const auto& acc : getData().desiredAccessories)
		{
			csv.write(acc.getDescriptionEnglish(acc.getIndex()));
			for (size_t i = 0; i < 4; i++)
			{
				csv.write(StatusTypeToString_EN[acc.getStatusBoosts()[i].type]);
			}
			csv.newLine();
		}
		csv.save(desiredAccessorySavePath.value());
	}
}

void Setting::update()
{
	if (goRecording)
	{
		changeScene(U"Recording");
	}
	serialUpdate();
	

	getData().cameraIndex = static_cast<uint32>(openableListBoxCamera.getSelectedIndex() - 1);
	getData().cameraName = openableListBoxCamera.getSelectedItem();

	getData().serialIndex = openableListBoxSerial.getSelectedIndex() - 1;
	getData().serialName = openableListBoxSerial.getSelectedItem();

	setProbability();
	selectAccTypeButtonUpdate();

	if (const auto& item = menuBar.update())
	{
		// 「開く」が押されたら
		if (item == MenuBarItemIndex{ 0, 0 })
		{
			openDesiredAccessories();
		}

		// 「名前を付けて保存」が押されたら
		if (item == MenuBarItemIndex{ 0, 1 })
		{
			saveDesiredAccessories();
		}

		// 「終了」が押されたら
		if (item == MenuBarItemIndex{ 0, 2 })
		{
			System::Exit();
		}

		// Webマニュアルが押されたら
		if (item == MenuBarItemIndex{ 2, 0 })
		{
			System::LaunchBrowser(getData().WebManualURL);
		}

		// 「ライセンス」が押されたら
		if (item == MenuBarItemIndex{ 2, 2 })
		{
			LicenseManager::ShowInBrowser();
		}
	}

	// ファイルがドロップされたら
	if (DragDrop::HasNewFilePaths())
	{
		for (const auto& dropped : DragDrop::GetDroppedFilePaths())
		{
			csvFileToDesiredAccessories(dropped.path);
			desiredAccessories_to_pullDowns();
		}
	}

	if (canGoRecording())
	{
		if (GoRecordingButtonRect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
		if (GoRecordingButtonRect.leftClicked())
		{
			assignDesiredAccessories();
			goRecording = true;
		}
	}
}

void Setting::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	// Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);
	

	SimpleGUI::CheckBox(getData().desireConsecutiveStatus, U"(オプション)特殊効果にかかわらず全て同じ種類のステータス増加のアクセサリを希望する", Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y });

	probabilityTable.draw(probabilityTablePos);
	drawNotion();

	if (isSelectedSerialPort() && SimpleGUI::Button(U"シリアル接続テスト", SerialTextPos.movedBy(280, 35)))
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


	drawMouseOver();

	if (canGoRecording())
	{
		const double angle = (Scene::Time() * 10_deg);

		// 影を作る図形を shadowTexture に描く
		{
			GoRecordingButtonRect.draw(ColorF{ 0.5, 0.5, 0.5, 0.5 });
		}
		{
			Shader::GaussianBlur(shadowTexture, internalTexture, shadowTexture);
			Shader::GaussianBlur(shadowTexture, internalTexture, shadowTexture);
		}
		shadowTexture.draw(ColorF{ 0.0, 0.5 });

		GoRecordingButtonRect.draw(Palette::Springgreen);
		GoRecordingButtonFont(U"次へ >>>").drawAt(GoRecordingButtonRect.center(), Palette::Black);
	}

	menuBar.draw();
}


