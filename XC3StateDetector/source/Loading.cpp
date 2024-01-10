# include "Loading.hpp"

Loading::Loading(const InitData& init)
		: IScene{ init }
{
	if (not getData().ini)
	{
		throw Error { U"Failed to load `config.ini`" };
	}
	getData().AppLanguage = Parse<String>(getData().ini[U"Language.AppLanguage"]);
	getData().ScreenshotFolderPath = Parse<String>(getData().ini[U"Screenshot.FolderPath"]);
	getData().ScreenshotDateFormat = Parse<String>(getData().ini[U"Screenshot.DateFormat"]);
	getData().ScreenshotFileFormat = Parse<String>(getData().ini[U"Screenshot.FileFormat"]);
	getData().enableToastNotification = Parse<bool>(getData().ini[U"Notification.enableToastNotification"]);


	if (not TranslationCSV) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `translate.csv`" };
	}

	const size_t languageCount = TranslationCSV.columns(0) - 1;

	for (size_t column = 1; column <= languageCount; ++column)
	{
		const String language = TranslationCSV[0][column];
		for (size_t row = 1; row < TranslationCSV.rows(); ++row) // 1行目はヘッダなので飛ばす
		{
			if (column >= TranslationCSV[row].size())
			{
				throw Error{ U"column out of range in `translation.csv` row, column = {}, {}"_fmt(row, column)};
			}
			const String key = TranslationCSV[row][0];
			const String value = TranslationCSV[row][column];
			getData().Translate[language][key] = value;
		}
	}


	if (not AccessoryCSV) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `accessory.csv`" };
	}

	Array<String> SpecialEffectListEnglish;
	Array<String> SpecialEffectListJapanese;
	Array<String> SpecialEffectDetailListJapanese;
	Array<String> CompatibilityEnglish;
	Array<String> CompatibilityJapanese;

	for (size_t row = 1; row < AccessoryCSV.rows(); ++row) // 1行目はヘッダなので飛ばす
	{
		Accessory::pushBackID(Parse<uint16>(AccessoryCSV[row][0]));
		SpecialEffectListEnglish.push_back(AccessoryCSV[row][1]);
		SpecialEffectListJapanese.push_back(AccessoryCSV[row][2]);
		Accessory::pushBackSpecialEffectDetailJapanese(AccessoryCSV[row][3]);
		CompatibilityEnglish.push_back(AccessoryCSV[row][4]);
		CompatibilityJapanese.push_back(AccessoryCSV[row][5]);
		Accessory::pushBackBracelet(Parse<double>(AccessoryCSV[row][6]));
		Accessory::pushBackRing(Parse<double>(AccessoryCSV[row][7]));
		Accessory::pushBackNecklace(Parse<double>(AccessoryCSV[row][8]));
		Accessory::pushBackCrown(Parse<double>(AccessoryCSV[row][9]));

	}
	Accessory::emplaceSpecialEffectList(U"en-US", SpecialEffectListEnglish);
	Accessory::emplaceSpecialEffectList(U"ja-JP", SpecialEffectListJapanese);
	Accessory::emplaceCompatibilityList(U"en-US", CompatibilityEnglish);
	Accessory::emplaceCompatibilityList(U"ja-JP", CompatibilityJapanese);


	for (uint16 i = 3428; i <= 3913; i += 5)
	{
		String path = descriptionJapaneseTemplateFolderPath + U"/" + Format(i) + U".jpg";
		descriptionsImagesPathList.push_back(path);
		getData().binarizedAbilities.push_back(Image{ path }.thresholded(128));
		// Console << path << U" を読み込みました";
	}

	for (String statusIconFileName : StatusTypeStringListEnglish)
	{
		if (statusIconFileName == U"Anything") continue;
		String path = statusIconsFolderPath + U"/" + statusIconFileName + U".jpg";
		statusIconsPathList.push_back(path);
		Image icon = Image{ Resource(path) };
		getData().icons.push_back(icon);
		getData().binarizedIcons.push_back(icon.thresholded(128));
		// Console << path << U" を読み込みました";
	}

	for (size_t i = 0; i < 4; i++)
	{
		String accessoryTypeFileName = AccessoryTypeToNameEN.at(static_cast<AccessoryType>(i + 1));
		String path = AccessoryTypeFolderPath + U"/" + accessoryTypeFileName + U".jpg";
		getData().binarizedAccessoryTypes.push_back(Image{ Resource(path) }.thresholded(128));
		// Console << path << U" を読み込みました";
	}

	for (int i = 0; i < 10; i++)
	{
		String path = EnigmatterNumbersPath + U"/" + Format(i) + U".jpg";
		getData().binarizedEnigmatterNumbers.push_back(Image{ Resource(path) }.thresholded(128));
		// Console << path << U" を読み込みました";
	}
	getData().binarizedEnigmatterNumbers.push_back(Image{ Resource(U"images/EnigmatterNumbers/null.jpg") }.thresholded(216));
	// Console << U"images/EnigmatterNumbers/null.jpg" << U" を読み込みました";

	for (const auto& GameSceneName : getData().GameSceneNames)
	{
		String path = GameScenesFolderPath + U"/" + GameSceneName + U".jpg";
		getData().binarizedGameScenes.push_back(Image{ Resource(path) }.thresholded(216));
		// Console << path << U" を読み込みました";
	}
}

void Loading::update()
{
	changeScene(U"Setting");
}

void Loading::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	Circle{ {Scene::Center().x, Scene::Center().y - 3200}, 3500 }.drawArc(135_deg, 90_deg, 0, 500, Palette::Springgreen);
	diamond.draw(Scene::Center().x - 33, Scene::Center().y - 315, Palette::Springgreen);
	ring.drawFrame(0, 20, Palette::Springgreen);
	FontAsset(U"TitleFont")(U"Now Loading").drawAt(Scene::Center());
}
