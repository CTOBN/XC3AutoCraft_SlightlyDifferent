#include "Recording.hpp"

double Recording::calculateSimilarity(const Image& img1, const Image& img2) {
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

size_t Recording::findMostSimilarNumber(const Point pos)
{
	size_t judgedNumber = 0;
	double similarityMax = 0;
	webcam.getFrame(image);

	Image clippedImage = image.clipped(pos, UNKOWN_MATTER_NUMBER_SIZE).thresholded(128);
	for (size_t i = 0; i < getData().binarizedUnkownMatterNumbers.size(); i++)
	{
		Image binarizedNumber = getData().binarizedUnkownMatterNumbers[i];

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
		StatusType judgedStatusType = StatusType::Undefined;
		double similarityMax = 0;
		Image clippedImage = image.clipped(clipStatusPosList[i], STATUS_ICON_SIZE).thresholded(128);
		for (int8 j = 0; j < getData().icons.size(); j++)
		{
			Image binarizedStatusTypeImage = getData().binarizedIcons[j];

			double similarity = calculateSimilarity(clippedImage, binarizedStatusTypeImage);
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

size_t Recording::findMostSimilarAbility() {
	webcam.getFrame(image);

	size_t judgedIndex = 0;
	String judgedAbilityName = U"認識不可";
	double similarityMax = 0;
	Image clippedImage = image.clipped(ABILITY_TEXT_AREA_POS, ABILITY_TEXI_AREA_SIZE).thresholded(128);
	for (size_t i = 0; i < getData().binarizedAbilities.size(); i++)
	{
		Image binarizedAbility = getData().binarizedAbilities[i];

		double similarity = calculateSimilarity(clippedImage, binarizedAbility);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedIndex = i;
		}
	}
	if (0 <= judgedIndex && judgedIndex < Accessory::getDescriptionDetailJPList().size())
	{
		currentAccAbilityJP = Accessory::getDescriptionDetailJP(judgedIndex);
		Console << Format(judgedIndex) << currentAccAbilityJP;
	}
	return judgedIndex;
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

void Recording::recognizeUnknownMatterCount()
{
	size_t tensPlace = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_TENS_PLACE_POS);
	size_t onesPlace = findMostSimilarNumber(UNKOWN_MATTER_NUMBER_ONES_PLACE_POS);
	size_t unknownMatterCount = tensPlace * 10 + onesPlace;
	context.initialUnkownMatterCount = unknownMatterCount;
	context.currentUnknownMatterCount = unknownMatterCount;
}

bool Recording::compareAccessories()
{
	for (int i = 0; i < getData().desiredAccessories.size(); i++)
	{
		Accessory& desiredAcc = getData().desiredAccessories[i];
		if (currentAccessory.getIndex() == desiredAcc.getIndex() && currentAccessory.hasSameStatusTypeOrMore(desiredAcc))
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
	else if (compareAccessories())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Recording::openSerialPort() const
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
	SerialBytesLog.append(currentSerialBytes);
	currentSerialBytes.clear();
}

void Recording::drawSerialBytesLog() const
{
	size_t logSize = SerialBytesLog.size();
	for (size_t i = 0; i < logSize; i++)
	{
		uint8 commandByte = SerialBytesLog[logSize - i - 1];
		if (commandByteToString.contains(commandByte))
		{
			String commandName = commandByteToString.at(commandByte);
			FontAsset(U"TextFont")(U"{} が実行されました"_fmt(commandName)).draw(1000, 350 + i * 30);
		}
	}
}


Recording::Recording(const InitData& init)
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

void Recording::update()
{
	if (context.getCurrentStateName() != U"Undefined")
	{
		context.request();
	}

	if (context.getCurrentStateName() == U"Judge")
	{
		if (not context.wasJudged)
		{
			addAccessory(recognizeAccessory());
			context.wasJudged = true;

			if (completeMission())
			{
				context.gotDesiredAccessory = true;
				const ToastNotificationItem toast{
					.title = U"アクセサリが完成しました", // 通知のタイトル
					.message = Accessory::getDescriptionDetailJP(currentAccessory.getIndex()), // 通知の本文
					.imagePath = U"pizza.png", // 大きい画像だと使われないことがある
					.actions = { U"通知を消す" } // アクションボタン（不要な場合は設定しない）
				};
				Platform::Windows::ToastNotification::Show(toast);
				Console << U"アクセサリが完成しました";
				Console << Accessory::getDescriptionDetailJP(currentAccessory.getIndex());
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

	if (SimpleGUI::Button(U"開始前に押す", Vec2{ buttonPosX, buttonPosY + 50 }))
	{
		recognizeUnknownMatterCount();
		uint8 setAccType = accessoryTypeIndexToCommandByte[getData().accessoryTypeIndex];
		getData().serial.writeByte(setAccType);
	}

	if (SimpleGUI::Button(U"自動クラフト開始", Vec2{ buttonPosX, buttonPosY + 100 }))
	{
		context.init();

		context.setState(std::make_unique<xc3::AccessorySelected>());
	}

	if (webcam && SimpleGUI::Button(U"PCにスクショを保存", Vec2{ buttonPosX, buttonPosY + 200 }))
	{
		webcam.getFrame(image);
		image.save(U"xenoblade3_screenshot.png");
	}

	if (SimpleGUI::Button(U"設定に戻る", Vec2{ buttonPosX, buttonPosY + 250 }))
	{
		// 設定に遷移
		changeScene(U"Setting");
	}
	if (SimpleGUI::Button(U"タイトルから", Vec2{ buttonPosX, buttonPosY + 350 }))
	{
		context.initialUnkownMatterCount = 99;
		context.currentUnknownMatterCount = context.initialUnkownMatterCount;
		uint8 setAccType = accessoryTypeIndexToCommandByte[getData().accessoryTypeIndex];
		getData().serial.writeByte(setAccType);
		context.init();
		context.setState(std::make_unique<xc3::Title>());
	}
	virtualJoyCon.update();
	receiveSerialBytes();
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
		Accessory& acc = getData().desiredAccessories[i];
		FontAsset(U"TextFont")(Accessory::getDescriptionJP(acc.getIndex())).draw(1000, 80 + i * 30);
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
		FontAsset(U"TextFont")(Accessory::getDescriptionJP(acc.getIndex())).draw(30, 550 + i * 30);
		for (int j = 0; j < 4; j++)
		{
			FontAsset(U"TextFont")(StatusTypeToString[U"JP"][acc.getStatusBoosts()[j].type]).drawAt(630 + j * 90, 560 + i * 30);
		}
	}
	virtualJoyCon.draw();

	// 現在の状態を表示
	FontAsset(U"TextFont")(U"現在の状態 : {}"_fmt(context.getCurrentStateName())).draw(1000, 300);
	FontAsset(U"TextFont")(U"現在ｱﾝﾉｳﾝﾏﾀｰ : {} 個"_fmt(context.currentUnknownMatterCount)).draw(1000, 330);
	drawSerialBytesLog();
}
