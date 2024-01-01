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
		StatusType judgedStatusType = StatusType::Unselected;
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
	for (size_t i = 0; i < getData().binarizedAbilities.size(); i++)
	{
		Image binarizedAbility = getData().binarizedAbilities[i];

		double similarity = calculateSimilarity(clippedImage, binarizedAbility);
		if (similarity > similarityMax)
		{
			similarityMax = similarity;
			judgedIndex = i + 1; // 0は未選択なので+１
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

void Recording::judgeAccessory()
{
	currentAccessory = recognizeAccessory();
	addAccessory(currentAccessory);
	context.wasJudged = true;

	if (completeMission())
	{
		context.gotDesiredAccessory = true;
		String toastTitle = U"アクセサリが完成しました";
		String toastMessage = Accessory::getDescriptionDetailJP(currentAccessory.getIndex());
		toastMessage += U"\n";
		for (int j = 0; j < 4; j++)
		{
			toastMessage += StatusTypeToString[U"JP"][currentAccessory.getStatusBoosts()[j].type];
			toastMessage += U" ";
		}

		const ToastNotificationItem toast{
			.title = toastTitle, // 通知のタイトル
			.message = toastMessage, // 通知のメッセージ
			.actions = { U"通知を消す" } // アクションボタン
		};
		Platform::Windows::ToastNotification::Show(toast);
		Console << toastTitle << toastMessage;
	}
}


void Recording::updateContext()
{

	if (context.getCurrentStateName() != U"Undefined")
	{
		context.request();
	}

	if (context.getCurrentStateName() != U"Judge" || context.wasJudged)
	{
		return;
	}

	judgeAccessory();
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
	lastSerialByte = currentSerialBytes.front();
	currentSerialBytes.clear();
}

void Recording::drawSerialBytesLog()
{
	if (lastSerialByte == 0) return;
	if (commandByteToString.contains(lastSerialByte))
	{
		String commandName = commandByteToString.at(lastSerialByte);
		Console << U"{} が実行されました"_fmt(commandName);
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
		FontAsset(U"TextFont")(Accessory::getDescriptionJP(acc.getIndex())).draw(recognizedAccessoriesPos.x, recognizedAccessoriesPos.y + i * 30);
		for (int j = 0; j < 4; j++)
		{
			FontAsset(U"TextFont")(StatusTypeToString[U"JP"][acc.getStatusBoosts()[j].type]).drawAt(recognizedAccessoriesPos.x + 600 + j * 90, recognizedAccessoriesPos.y + 10 + i * 30);
		}
	}
}


// 目的のアクセサリを表示
void Recording::drawDesiredAccessories() const
{
	for (int i = 0; i < getData().desiredAccessories.size(); i++)
	{
		Accessory& acc = getData().desiredAccessories[i];
		FontAsset(U"TextFont")(Accessory::getDescriptionJP(acc.getIndex())).draw(desiredAccessoriesPos.x, desiredAccessoriesPos.y + i * 30);
		for (int j = 0; j < 4; j++)
		{
			FontAsset(U"TextFont")(StatusTypeToString[U"JP"][acc.getStatusBoosts()[j].type]).drawAt(desiredAccessoriesPos.x + 600 + j * 70, desiredAccessoriesPos.y + 10 + i * 30);
		}
	}
}

void Recording::drawButtons()
{
	if (SimpleGUI::Button(U"\U000F065C シリアルポートを開く", Vec2{ buttonPos.x, buttonPos.y }))
	{
		openSerialPort();
	}

	if (SimpleGUI::Button(U"\U000F0199 ｱﾝﾉｳﾝﾏﾀｰの数を認識", Vec2{ buttonPos.x, buttonPos.y + 50 }))
	{
		recognizeUnknownMatterCount();
		uint8 setAccType = accessoryTypeIndexToCommandByte[getData().accessoryTypeIndex - 1];
		getData().serial.writeByte(setAccType);
	}

	if (SimpleGUI::Button(U"\U000F040A 自動クラフト開始", Vec2{ buttonPos.x, buttonPos.y + 100 }))
	{
		context.init();

		context.setState(std::make_unique<xc3::AccessorySelected>());
	}

	if (webcam && SimpleGUI::Button(U"\U000F0E51 PCにスクショを保存", Vec2{ buttonPos.x, buttonPos.y + 200 }))
	{
		webcam.getFrame(image);
		String path = U"XC3AutoCraft_{}.png"_fmt(DateTime::Now()).replace(U":", U".");
		image.save(path);
		Console << U"スクリーンショットを保存しました ファイル名 : {}"_fmt(path);
	}

	if (SimpleGUI::Button(U"\U000F0493 設定に戻る", Vec2{ buttonPos.x, buttonPos.y + 250 }))
	{
		// 設定に遷移
		changeScene(U"Setting");
	}

	if (SimpleGUI::Button(U"\U000F0544 Tweetする", Vec2{ buttonPos.x, buttonPos.y + 300 }))
	{
		// ツイート投稿画面を開く
		Twitter::OpenTweetWindow(U"#XC3AutoCraft");
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

	drawRecognizedAccessories();
	drawDesiredAccessories();

	virtualJoyCon.draw();

	// 現在の状態を表示
	FontAsset(U"TextFont")(U"現在の状態 : {}"_fmt(context.getCurrentStateName())).draw(1000, 300);
	FontAsset(U"TextFont")(U"現在ｱﾝﾉｳﾝﾏﾀｰ : {} 個"_fmt(context.currentUnknownMatterCount)).draw(1000, 330);

}
