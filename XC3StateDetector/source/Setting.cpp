# include "Setting.hpp"

static bool DentButton(const String& str, const RoundRect& roundRect, const Font font, bool flag = true, const ColorF& buttonColor = Scene::GetBackground(), const ColorF& fontColor = Palette::White) {

	RoundRect button = roundRect;

	if (flag) {

		if (button.mouseOver())Cursor::RequestStyle(CursorStyle::Hand);

		if (button.leftPressed()) {
			button.drawShadow({ 0,0 }, 2, 1, ColorF(0.3, 0.5));
			button.drawShadow({ 2,2 }, 2, 1, ColorF(1, 0.5));
			button.moveBy(1, 1);
		}
		else {
			button.drawShadow({ -4,-4 }, 8, 1, ColorF(1, 0.5));
			button.drawShadow({ 4,4 }, 8, 1, ColorF(0, 0.3));
		}
	}
	else {
		button.drawFrame();
	}

	button.draw(buttonColor);

	font(str).drawAt(button.center(), fontColor);

	return flag && button.leftClicked();
}


Setting::Setting(const InitData& init)
	: IScene{ init }

{
	for (int i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		openableListBoxesAccessory.push_back(OpenableListBox{ ACCSESSORIE_FONT, accPulldownTablePos.movedBy(0, 31 * (i+1)), ACCESSORIES_CELL_WIDTH + 1, 30, 5 });
	}

	for (auto& openableListBoxAccessory : openableListBoxesAccessory)
	{
		openableListBoxAccessory.setItems(Accessory::getSpecialEffectList(AppLanguage));
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
		openableListBoxStatusType.setItems(StatusTypeStringList[AppLanguage]);
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

	accPulldownTable.push_back_row(
		{
			Translate[AppLanguage][U"Special Effects"],
			Translate[AppLanguage][U"Status"] + U"1",
			Translate[AppLanguage][U"Status"] + U"2",
			Translate[AppLanguage][U"Status"] + U"3",
			Translate[AppLanguage][U"Status"] + U"4"
		}, { -1, 0, 0, 0, 0 });

	probabilityTable.push_back_row(
		{
			Translate[AppLanguage][U"Compatibility"],
			Translate[AppLanguage][U"Bangle"],
			Translate[AppLanguage][U"Ring"],
			Translate[AppLanguage][U"Collar"],
			Translate[AppLanguage][U"Tiara"]
		}, { 0, 0, 0, 0, 0 });
	for (size_t i = 0; i < TARGET_ACCSESORIES_COUNT_MAX; i++)
	{
		probabilityTable.push_back_row({ U"-", U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });
	}
	probabilityTable.push_back_row({ Translate[AppLanguage][U"Sum"], U"0", U"0", U"0", U"0" }, { 0, 1, 1, 1, 1 });

	menuBar.setItemChecked(MenuBarItemIndex{ 1, 0 }, getData().enableToastNotification);
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
		for (size_t j = 0; j < Accessory::getSpecialEffectList(U"en-US").size(); j++)
		{
			if ((Accessory::getSpecialEffectList(U"en-US")[j] == csv.get<String>(i, 0))
			 || (Accessory::getSpecialEffectList(U"en-US")[j] == csv.get<String>(i, 0)))
			{
				index = j;
				break;
			}
		}
		// csvファイルのアクセサリの名前が不正な場合
		if (index == Accessory::getSpecialEffectList(U"en-US").size())
		{
			continue;
		}

		Accessory acc{ index };
		for (size_t j = 0; j < 4; j++)
		{
			String statusTypeString = csv.get<String>(i, j + 1);
			// csvファイルのステータスの名前が不正な場合、任意として扱う
			StatusType statusType = StatusType::Anything;
			if (StatusTypeStringListEnglish.contains(statusTypeString)
			||  StatusTypeStringListJapanese.contains(statusTypeString))
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
		probabilityTable.setText(i + 1, 0, Accessory::getCompatibilityList(AppLanguage)[index]);

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
	return getData().serialIndex != 0;
}

bool Setting::isSelectedCamera() const
{
	return getData().cameraIndex != 0;
}

bool Setting::canGoRecording() const
{
	return getData().selectedAccessoryType != AccessoryType::Undefined && isSelectedCamera() && isSelectedSerialPort() && canMake();
}

void Setting::serialUpdate()
{
	if (isSelectedSerialPort())
	{
		serialSelectionStatus = Translate[AppLanguage][U"Connection test available(Option)→"];
		serialSelectionStatusColor = Palette::Green;
	}
	else
	{
		getData().serial = Serial{};
		serialSelectionStatus = Translate[AppLanguage][U"Please select a Serial Port"];
		serialSelectionStatusColor = Palette::Red;
	}
}

void Setting::drawSerialStatus() const
{
	FontAsset(U"TextFont")(serialSelectionStatus).draw(TextStyle::Outline(outlineScale, outlineColor), SerialTextPos.movedBy(0, 40), serialSelectionStatusColor);
	FontAsset(U"TextFont")(serialConnectionStatus).draw(TextStyle::Outline(outlineScale, outlineColor), SerialTextPos.movedBy(720, 40), serialConnectionStatusColor);
}

void Setting::drawNotion() const
{
	if (not canMake())
	{
		FontAsset(U"TextFont")(Translate[AppLanguage][U"There is a 0% chance that the desired accessory will be available"]).draw(TextStyle::Outline(outlineScale, outlineColor), probabilityTablePos.movedBy(70, -50), Palette::Red);
	}

	if (getData().selectedAccessoryType == AccessoryType::Undefined)
	{
		FontAsset(U"TextFont")(Translate[AppLanguage][U"Select the type of accessory you want to make"]).draw(TextStyle::Outline(outlineScale, outlineColor), probabilityTablePos.movedBy(70, -50), Palette::Red);
	}
	else
	{
		probabilityTable.cellRegion(probabilityTablePos, 0, static_cast<size_t>(getData().selectedAccessoryType)).drawFrame(4, 0, Palette::Red);
	}

	if (getData().cameraIndex == 0)
	{
		FontAsset(U"TextFont")(Translate[AppLanguage][U"Please select a HDMI Capture"]).draw(TextStyle::Outline(outlineScale, outlineColor), CameraTextPos.movedBy(0, 40), Palette::Red);
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
			csv.write(Accessory::getSpecialEffectList(AppLanguage)[acc.getIndex()]);
			for (size_t i = 0; i < 4; i++)
			{
				csv.write(StatusTypeToString[AppLanguage][acc.getStatusBoosts()[i].type]);
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
	

	getData().cameraIndex = openableListBoxCamera.getSelectedIndex();
	getData().cameraName = openableListBoxCamera.getSelectedItem();

	getData().serialIndex = openableListBoxSerial.getSelectedIndex();
	getData().serialName = openableListBoxSerial.getSelectedItem();

	setProbability();
	selectAccTypeButtonUpdate();

	if (const auto& item = menuBar.update())
	{
		if (item == MenuBarItemIndex{ 1, 0 })
		{
			// チェック状態を反転する
			menuBar.setItemChecked(*item, (not menuBar.getItemChecked(*item)));
			getData().enableToastNotification = menuBar.getItemChecked(MenuBarItemIndex{ 1, 0 });
			getData().ini[U"Notification.enableToastNotification"] = getData().enableToastNotification;
			getData().ini.save(U"config.ini");
		}

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

		// 「スクリーンショットの保存先を変更」が押されたら
		if (item == MenuBarItemIndex{ 1, 1 })
		{
			const auto result = Dialog::SelectFolder(getData().ScreenshotFolderPath);
			if (result)
			{
				getData().ScreenshotFolderPath = result.value();
				getData().ini[U"Screenshot.FolderPath"] = getData().ScreenshotFolderPath;
				getData().ini.save(U"config.ini");
			}
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

		// 「ライセンス情報」が押されたら
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

	// 下のリストボックスから更新することで選択時のクリックで別のリストボックスが開かないようにする
	for (auto it = std::rbegin(openableListBoxesAccessory); it != std::rend(openableListBoxesAccessory); ++it)
	{
		auto& openableListBoxAccessory = *it;
		openableListBoxAccessory.update();
	}

	// 下のリストボックスから更新することで選択時のクリックで別のリストボックスが開かないようにする
	for (auto it = std::rbegin(openableListBoxesStatusType); it != std::rend(openableListBoxesStatusType); ++it)
	{
		auto& openableListBoxStatusType = *it;
		openableListBoxStatusType.update();
	}

	openableListBoxCamera.update();
	openableListBoxSerial.update();
}

void Setting::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });

	SimpleGUI::CheckBox
	(
		getData().desireConsecutiveStatus,
		Translate[AppLanguage][U"Desire all accessories to have the same type of status regardless of special effects (Option)"],
		Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y }
	);

	probabilityTable.draw(probabilityTablePos);
	drawNotion();

	if (isSelectedSerialPort() && SimpleGUI::Button(Translate[AppLanguage][U"Serial connection test"], SerialTextPos.movedBy(720, 80)))
	{
		if (getData().serial.open(getData().infos[getData().serialIndex - 1].port))
		{
			serialConnectionStatus = Translate[AppLanguage][U"Serial Port connection succeeded"];
			serialConnectionStatusColor = Palette::Green;
		}
		else
		{
			serialConnectionStatus = Translate[AppLanguage][U"Serial Port connection succeeded"],
			serialConnectionStatusColor = Palette::Red;
		}
	}

	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Serial Port"]).draw(SerialTextPos);
	drawSerialStatus();

	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"HDMI Capture"]).draw(CameraTextPos);

	// 候補が下に展開されるので逆順に描画
	for (auto it = std::rbegin(openableListBoxesAccessory); it != std::rend(openableListBoxesAccessory); ++it)
	{
		auto& openableListBoxAccessory = *it;
		openableListBoxAccessory.draw();
	}

	// 候補が下に展開されるので逆順に描画
	for (auto it = std::rbegin(openableListBoxesStatusType); it != std::rend(openableListBoxesStatusType); ++it)
	{
		auto& openableListBoxStatusType = *it;
		openableListBoxStatusType.draw();
	}

	openableListBoxCamera.draw();
	openableListBoxSerial.draw();

	accPulldownTable.draw(accPulldownTablePos);

	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Desired accessories"]).draw(20, ACCSESSORIE_TEXT_Y);

	drawMouseOver();

	menuBar.draw();

	if (GoRecordingRect.mouseOver())
	{
		// 目的のアクセサリが出る可能性が０%の場合
		if (not canMake())
		{
			Line{ GoRecordingRect.topCenter(), openableListBoxesAccessory.back().getDisplayRegion().br() }.drawArrow(10, SizeF{20, 20}, Palette::Orange);
			Line{ GoRecordingRect.topCenter(), Vec2{ MENU_X, DESIRE_CONSENCUTIVE_STATUS_Y }.movedBy(20, 20)}.drawArrow(10, SizeF{20, 20}, Palette::Orange);
		}

		// 作るアクセサリが選択されていない場合
		if (getData().selectedAccessoryType == AccessoryType::Undefined)
		{
			Line{ GoRecordingRect.topCenter(), probabilityTable.cellRegion(probabilityTablePos, 0, 2).br() }.drawArrow(10, SizeF{ 20, 20 }, Palette::Orange);
		}

		// カメラが選択されていない場合
		if (not isSelectedCamera())
		{
			Line{ GoRecordingRect.leftCenter(), openableListBoxCamera.getDisplayRegion().rightCenter()}.drawArrow(10, SizeF{20, 20}, Palette::Orange);
		}

		// シリアルポートが選択されていない場合
		if (not isSelectedSerialPort())
		{
			Line{ GoRecordingRect.leftCenter(), openableListBoxSerial.getDisplayRegion().rightCenter() }.drawArrow(10, SizeF{20, 20}, Palette::Orange);
		}
	}
	if (DentButton( canGoRecording() ? Translate[AppLanguage][U"Next"] : Translate[AppLanguage][U"Unable to proceed"],
					GoRecordingRect,
					GoRecordingButtonFont,
					canGoRecording(),
					canGoRecording() ? Color{ 144, 238, 144, 255 }   : Color{ 144, 238, 144, 100 },
					canGoRecording() ? Palette::White : Color{ 127, 127, 127, 255 }))
	{
		assignDesiredAccessories();
		goRecording = true;
	}
}


