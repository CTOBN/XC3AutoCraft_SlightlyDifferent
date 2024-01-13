#include "Recording.hpp"


Recording::Recording(const InitData& init)
	: IScene{ init }
{
	// 非同期タスクを開始
	const uint32 CameraIndex = getData().cameraIndex - 1;

	task = AsyncTask<Webcam>{ [CameraIndex, this]() {
		Webcam webcam{ CameraIndex, Size{ this->CAMERA_RESOLUTION }, StartImmediately::No };
		webcam.start();
		return webcam;
	} };
}


double Recording::calculateSimilarity(const Image& img1, const Image& img2) {
	// 画像のサイズが一致しない場合はエラーを返す
	if (img1.size() != img2.size()) {
		throw Error(U"Images must be the same size {} and {}"_fmt(img1.size(), img2.size()));
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

size_t Recording::findMostSimilarNumber(const Point pos)
{
	size_t judgedNumber = 0;
	double similarityMax = 0;
	webcam.getFrame(image);

	Image clippedImage = image.clipped(pos, UNKOWN_MATTER_NUMBER_SIZE).thresholded(128);
	for (size_t i = 0; i < getData().binarizedEnigmatterNumbers.size(); i++)
	{
		Image binarizedNumber = getData().binarizedEnigmatterNumbers[i];

		double similarity = calculateSimilarity(clippedImage, binarizedNumber);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedNumber = i;
		}
	}
	if (judgedNumber == 10) judgedNumber = 0; // null.jpgの場合0として扱う
	return judgedNumber;
}

Array<StatusBoost> Recording::findMostSimilarStatusBoost()
{
	Array<StatusBoost> result;
	webcam.getFrame(image);

	for (int i = 0; i < 4; i++)
	{
		StatusType judgedStatusType = StatusType::Anything;
		double similarityMax = 0;
		Image clippedImage = image.clipped(clipStatusPosList[i], STATUS_ICON_SIZE).thresholded(128);
		for (int8 j = 0; j < getData().icons.size(); j++)
		{
			Image binarizedStatusTypeImage = getData().binarizedIcons[j];

			double similarity = calculateSimilarity(clippedImage, binarizedStatusTypeImage);
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

size_t Recording::findMostSimilarAbility() {
	webcam.getFrame(image);

	size_t judgedIndex = 0;
	String judgedAbilityName = U"認識不可";
	double similarityMax = 0;
	Image clippedImage = image.clipped(ABILITY_TEXT_AREA_POS, ABILITY_TEXI_AREA_SIZE).thresholded(128);
	for (size_t i = 0; i < getData().binarizedSpecialEffects.size(); i++)
	{
		Image binarizedAbility = getData().binarizedSpecialEffects[i];

		double similarity = calculateSimilarity(clippedImage, binarizedAbility);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedIndex = i + 2; // 0は未選択, 1は任意のアクセサリ
		}
	}
	if (0 <= judgedIndex && judgedIndex < Accessory::getSpecialEffectList(AppLanguage).size())
	{
		currentSpecialEffect = Accessory::getSpecialEffectList(AppLanguage)[judgedIndex];
		// Console << Format(judgedIndex) << currentSpecialEffect;
	}
	return judgedIndex;
}

String Recording::findMostSimilarGameScene()
{
	webcam.getFrame(image);
	Point clipPos = { 1665, 1030 };
	Point clipSize = { 233, 30 };
	uint8 threshold = 216;
	String judgedGameSceneName = U"";
	double similarityMax = 0;
	Image clippedImage = image.clipped(clipPos, clipSize).thresholded(threshold);
	for (size_t i = 0; i < getData().binarizedGameScenes.size(); i++)
	{
		Image binarizedGameScene = getData().binarizedGameScenes[i];

		double similarity = calculateSimilarity(clippedImage, binarizedGameScene);
		// Console << U"{} : {}"_fmt(getData().GameSceneNames[i], similarity);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedGameSceneName = getData().GameSceneNames[i];
		}
	}
	return judgedGameSceneName;
}

AccessoryType Recording::recognizeSelectingAccessoryType()
{
	webcam.getFrame(image);
	Point clipPos = { 1152, 174 };
	Point clipSize = { 56, 56 };
	uint8 threshold = 128;
	AccessoryType judgedAccessoryType = AccessoryType::Undefined;
	double similarityMax = 0;
	Image clippedImage = image.clipped(clipPos, clipSize).thresholded(threshold);
	for (size_t i = 0; i < 4; i++)
	{
		double similarity = calculateSimilarity(clippedImage, getData().binarizedAccessoryTypes[i]);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedAccessoryType = static_cast<AccessoryType>(i + 1);
		}
	}
	return judgedAccessoryType;
}

int8 Recording::recognizeSelectingRestSpotMenu()
{
	webcam.getFrame(image);
	Point clipPos = { 936, 881 };
	Point clipSize = { 48, 48 };
	uint8 threshold = 128;
	int8 judgedRestSpotMenu = 0;
	double similarityMax = 0;
	Image clippedImage = image.clipped(clipPos, clipSize).thresholded(threshold);
	for (size_t i = 0; i < 7; i++)
	{
		double similarity = calculateSimilarity(clippedImage, getData().binarizedRestSpotMenus[i]);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedRestSpotMenu = i;
		}
	}
	return judgedRestSpotMenu;
}

Accessory Recording::recognizeAccessory()
{
	size_t judgedAccIndex = findMostSimilarAbility();
	Array<StatusBoost> judgedStatusBoosts = findMostSimilarStatusBoost();
	Accessory recognizedAccessory = Accessory{ judgedAccIndex };
	recognizedAccessory.setStatusBoosts(judgedStatusBoosts);
	return recognizedAccessory;
}

void Recording::addAccessory(const Accessory& accessory)
{
	if (RecognizedAccessories.size() == 15) {
		RecognizedAccessories.pop_front();
	}
	RecognizedAccessories.push_back(accessory);
}

void Recording::recognizeEnigmatterCount()
{
	size_t tensPlace = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_TENS_PLACE_POS);
	size_t onesPlace = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_ONES_PLACE_POS);
	size_t EnigmatterCount = tensPlace * 10 + onesPlace;
	context.currentEnigmatterCount = EnigmatterCount;
}


// 1.所望のアクセサリーのリストをループします。
// 2.各所望のアクセサリーについて、そのインデックスが1（任意のアクセサリーを示す）かどうかを確認します。もしインデックスが1なら、現在のアクセサリーが同じステータスタイプを持つかどうかを確認します。もし同じステータスタイプを持つなら、trueを返します。
// 3.もし所望のアクセサリーのインデックスが1でない場合、現在のアクセサリーのインデックスが所望のアクセサリーのインデックスと一致するかどうかを確認します。もし一致し、さらに現在のアクセサリーが同じステータスタイプを持つなら、trueを返します。
// 4.ループが終了し、一致するアクセサリーが見つからない場合は、falseを返します。
bool Recording::compareAccessories()
{
	for (const auto& desiredAcc : getData().desiredAccessories)
	{
		bool isAnyAccessory = (desiredAcc.getIndex() == 1);
		bool isSameAccessory = (currentAccessory.getIndex() == desiredAcc.getIndex());

		if ((isAnyAccessory || isSameAccessory) && currentAccessory.hasSameStatusTypeOrMore(desiredAcc))
		{
			return true;
		}
	}
	return false;
}

bool Recording::completeMission()
{
	// 全て同じ能力上昇値のアクセサリを希望して　かつ　現在のアクセサリが同じ能力上昇値のアクセサリである
	if (getData().desireConsecutiveStatus && currentAccessory.hasConsecutiveStatus())
	{
		return true;
	}
	// 目的のアクセサリの中に現在のアクセサリがある
	return compareAccessories();
}

// RestSpotメニューからアクセサリクラフトを選択する
void Recording::selectAccessoryCraft()
{
	if (context.getCurrentStateName() != U"RestSpot" || context.isAccessoryCraftSelected)
	{
		return;
	}
	int8 currentSelectingRestSpotMenu = recognizeSelectingRestSpotMenu();
	int8 RightCount = (4 - currentSelectingRestSpotMenu + 7) % 7;
	int8 LeftCount = (currentSelectingRestSpotMenu - 4 + 7) % 7;
	if (not (0 <= currentSelectingRestSpotMenu && currentSelectingRestSpotMenu < 7))
	{
		// Console << U"認識できませんでした";
		return;
	}
	// Console << U"R L = {} vs {}"_fmt(RightCount, LeftCount);
	if (RightCount < LeftCount)
	{
		for (int8 i = 0; i < RightCount; i++)
		{
			getData().serial.writeByte(ButtonByte::Right);
		}
	}
	else
	{
		for (int8 i = 0; i < LeftCount; i++)
		{
			getData().serial.writeByte(ButtonByte::Left);
		}
	}
	getData().serial.writeByte(ButtonByte::A);
	context.isAccessoryCraftSelected = true;
}

void Recording::countEnigmatter()
{
	if (context.getCurrentStateName() != U"RecognizeItemCount" || context.isEnigmatterCountUpdated)
	{
		return;
	}
	recognizeEnigmatterCount();
	context.isEnigmatterCountUpdated = true;
}

void Recording::selectAccessoryType()
{
	if (context.getCurrentStateName() != U"AccessoryTypeSelecting" || context.isAccessoryTypeSelected)
	{
		return;
	}
	AccessoryType currentSelectingAccessoryType = recognizeSelectingAccessoryType();
	AccessoryType desiredAccessoryType = getData().selectedAccessoryType;
	int8 UpCount = (static_cast<int8>(currentSelectingAccessoryType) - static_cast<int8>(desiredAccessoryType) + 4) % 4;
	int8 DownCount = (static_cast<int8>(desiredAccessoryType) - static_cast<int8>(currentSelectingAccessoryType) + 4) % 4;
	
	if (UpCount < DownCount)
	{
		for (int i = 0; i < UpCount; i++)
		{
			getData().serial.writeByte(ButtonByte::LStickUp);
		}
	}
	else
	{
		for (int i = 0; i < DownCount; i++)
		{
			getData().serial.writeByte(ButtonByte::LStickDown);
		}
	}
	context.isAccessoryTypeSelected = true;
}

void Recording::judgeAccessory()
{
	if (context.getCurrentStateName() != U"Judge" || context.isJudged)
	{
		return;
	}

	currentAccessory = recognizeAccessory();
	addAccessory(currentAccessory);
	context.isJudged = true;

	if (completeMission())
	{
		context.gotDesiredAccessory = true;
		String toastTitle = Translate[AppLanguage][U"You got desired Accessory"];
		String toastMessage = Accessory::getSpecialEffectList(AppLanguage)[currentAccessory.getIndex()];
		toastMessage += U"\n";
		for (int j = 0; j < 4; j++)
		{
			toastMessage += StatusTypeToString[AppLanguage][currentAccessory.getStatusBoosts()[j].type];
			toastMessage += U" ";
		}

		const ToastNotificationItem toast{
			.title = toastTitle, // 通知のタイトル
			.message = toastMessage, // 通知のメッセージ
			.actions = { U"通知を消す" } // アクションボタン
		};
		Platform::Windows::ToastNotification::Show(toast);
		// Console << toastTitle << toastMessage;
	}
}


void Recording::updateContext()
{

	if (context.getCurrentStateName() != U"Undefined")
	{
		context.request();
	}
	selectAccessoryCraft();
	countEnigmatter();
	selectAccessoryType();
	judgeAccessory();
}

bool Recording::openSerialPort() const
{
	if (getData().serial.open(getData().infos[getData().serialIndex - 1].port))
	{
		// Console << U"シリアルポートを開きました";
		return true;
	}
	else
	{
		// Console << U"シリアルポートを開けませんでした";
		return false;
	}
}

void Recording::receiveSerialBytes()
{
	if (getData().serial.available())
	{
		currentSerialBytes = getData().serial.readBytes();
	}
	if (currentSerialBytes.size() == 0)
	{
		return;
	}
	// Console << U"受信 : {}"_fmt(currentSerialBytes);
	lastSerialByte = currentSerialBytes.front();
	currentSerialBytes.clear();
}

void Recording::drawSerialBytesLog()
{
	if (lastSerialByte == 0) return;
	if (commandByteToString.contains(lastSerialByte))
	{
		String commandName = commandByteToString.at(lastSerialByte);
		// Console << U"{} が実行されました"_fmt(commandName);
		lastSerialByte = 0;
	}
}


// 認識したアクセサリを表示
void Recording::drawRecognizedAccessories() const
{
	const size_t recognizedAccessoriesSize = RecognizedAccessories.size();
	for (int i = 0; i < recognizedAccessoriesSize; i++)
	{
		const Accessory& acc = RecognizedAccessories[recognizedAccessoriesSize - i - 1];
		fontAccessory(Accessory::getSpecialEffectList(AppLanguage)[acc.getIndex()]).draw(recognizedAccessoriesPos.movedBy(0, i * 30));
		for (int j = 0; j < 4; j++)
		{
			fontAccessory(StatusTypeToString[getData().AppLanguage][acc.getStatusBoosts()[j].type]).drawAt(recognizedAccessoriesPos.movedBy(600 + j * 90, 10 + i * 30));
		}
	}
}


// 目的のアクセサリを表示
void Recording::drawDesiredAccessories() const
{
	for (int i = 0; i < getData().desiredAccessories.size(); i++)
	{
		Accessory& acc = getData().desiredAccessories[i];
		fontAccessory(Accessory::getSpecialEffectList(AppLanguage)[acc.getIndex()]).draw(desiredAccessoriesPos.movedBy(0, i * 30));
		for (int j = 0; j < 4; j++)
		{
			fontAccessory(StatusTypeToString[getData().AppLanguage][acc.getStatusBoosts()[j].type]).drawAt(desiredAccessoriesPos.movedBy(600 + j * 90, 10 + i * 30));
		}
	}
}

void Recording::drawButtons()
{
	if (SimpleGUI::Button(U"\U000F02B4 {}"_fmt(Translate[AppLanguage][U"Connect Virtual JoyCon"]), Vec2{ buttonPos.x, buttonPos.y }))
	{
		openSerialPort();
	}

	if (SimpleGUI::Button(Translate[AppLanguage][U"3 times A Button"], Vec2{buttonPos.movedBy(300, 0)}))
	{
		getData().serial.writeByte(ButtonByte::A);
		getData().serial.writeByte(ButtonByte::A);
		getData().serial.writeByte(ButtonByte::A);
	}

	if (SimpleGUI::Button(U"\U000F040A {}"_fmt(Translate[AppLanguage][U"Start Auto Craft"]), Vec2{ buttonPos.movedBy(0, 50) }))
	{
		context.init();
		String gameSceneName = findMostSimilarGameScene();
		if (gameSceneName == U"Title")
		{
			context.setState(std::make_unique<xc3::Title>());
		}
		else if (gameSceneName == U"Field")
		{
			context.setState(std::make_unique<xc3::Field>());
		}
		else if (gameSceneName == U"RestSpot")
		{
			context.setState(std::make_unique<xc3::RestSpot>());
		}
		else if (gameSceneName == U"CraftAccessories")
		{
			context.setState(std::make_unique<xc3::RecognizeItemCount>());
		}
		else
		{
			context.message = U"Game scene could not be recognized";
		}
	}

	if (SimpleGUI::Button(U"\U000F04DB {}"_fmt(Translate[AppLanguage][U"Stop Auto Craft"]), Vec2{ buttonPos.movedBy(0, 100) }))
	{
		context.deleteState();
	}
	
	if (SimpleGUI::Button(U"\U000F0493 {}"_fmt(Translate[AppLanguage][U"Go to Setting"]), Vec2{ buttonPos.movedBy(0, 200) }))
	{
		// 設定に遷移
		OpenableListBox::closeCurrentOpeningListBox();
		changeScene(U"Setting", 0.5s);
	}

	if (webcam && SimpleGUI::Button(U"\U000F0E51 {}"_fmt(Translate[AppLanguage][U"Take Screenshot"]), Vec2{ buttonPos.movedBy(0, 250) }))
	{
		webcam.getFrame(image);
		if (FileSystem::Exists(getData().ScreenshotFolderPath))
		{
			// String now = U"{}"_fmt(DateTime::Now()).replace(U":", U".");
			String now = FormatDateTime(DateTime::Now(), getData().ScreenshotDateFormat);
			String path = U"{}{}{}{}"_fmt(getData().ScreenshotFolderPath, getData().ScreenshotFileName, now, getData().ScreenshotFileFormat);
			image.save(path);
			// Console << U"スクリーンショットを保存しました ファイル名 : {}"_fmt(path);
		}
	}
	// スクリーンショットの保存先を開く
	if (SimpleGUI::Button(Translate[AppLanguage][U"Folder"], Vec2{ buttonPos.movedBy(250, 250) }))
	{
		System::LaunchFile(getData().ScreenshotFolderPath);
		// Console << getData().ScreenshotFolderPath;
	}

	if (SimpleGUI::Button(U"\U000F0544 {}"_fmt(Translate[AppLanguage][U"Tweet"]), Vec2{buttonPos.movedBy(0, 300)}))
	{
		// ツイート投稿画面を開く
		Twitter::OpenTweetWindow(U"#XC3AutoCraft");
	}
}

void Recording::drawRecognitionArea() const
{
	Rect{ ABILITY_TEXT_AREA_POS / 2, ABILITY_TEXI_AREA_SIZE / 2 }.drawFrame(1, Palette::Red);

	for (int i = 0; i < 4; i++)
	{
		Rect{ clipStatusPosList[i] / 2, STATUS_ICON_SIZE / 2 }.drawFrame(1, Palette::Green);
	}
}



void Recording::update()
{
	updateContext();

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

	drawButtons();
	virtualJoyCon.update();
	receiveSerialBytes();
	drawSerialBytesLog();

	if (const auto& item = menuBar.update())
	{
		// 「終了」が押されたら
		if (item == MenuBarItemIndex{ 0, 1 })
		{
			System::Exit();
		}

		// Webマニュアルが押されたら
		if (item == MenuBarItemIndex{ 2, 0 })
		{
			System::LaunchBrowser(getData().WebManualURL);
		}

		// 「ライセンス情報」が押されたら
		if (item == MenuBarItemIndex{ 2, 1 })
		{
			LicenseManager::ShowInBrowser();
		}
	}
}



void Recording::draw() const
{
	Scene::SetBackground(ColorF(0.2, 0.8, 0.6));
	// Webcam 作成待機中は円を表示
	if (not webcam)
	{
		Rect{ {0, 0}, VIDEO_DISPLAY_SIZE }.draw(Palette::Black);
		Circle{ {VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2}, 100 }
		.drawArc(Scene::Time() * 180_deg, 300_deg, 5, 5, Palette::White);
		FontAsset(U"TextFont")(getData().Translate[AppLanguage][U"HDMI Capture is preparing"]).drawAt({ VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2 - 20 }, Palette::White);
		FontAsset(U"TextFont")(getData().cameraName).drawAt({ VIDEO_DISPLAY_SIZE.x / 2, VIDEO_DISPLAY_SIZE.y / 2 + 20 }, Palette::White);
	}
	if (texture)
	{
		texture.resized(960, 540).draw(Point{ 0, SimpleMenuBar::MenuBarHeight});
	}

	drawRecognizedAccessories();
	drawDesiredAccessories();

	virtualJoyCon.draw();

	// 現在の状態を表示
	FontAsset(U"TextFont")(U"{} : {}"_fmt(getData().Translate[AppLanguage][U"Current State"], getData().Translate[AppLanguage][context.getCurrentStateName()])).draw(StateInformationPos);
	FontAsset(U"TextFont")(U"{} : {}"_fmt(getData().Translate[AppLanguage][U"Enigmatter"], context.currentEnigmatterCount)).draw(StateInformationPos.movedBy(0, 30));
	// FontAsset(U"TextFont")(getData().Translate[AppLanguage][context.message]).draw(1150, 510, Palette::Red);

	menuBar.draw();
}
