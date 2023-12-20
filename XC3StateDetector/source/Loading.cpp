# include "Loading.hpp"

Loading::Loading(const InitData& init)
		: IScene{ init }
{
	if (not csv) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `accessories.csv`" };
	}

	for (size_t row = 1; row < csv.rows(); ++row) // 1行目はヘッダなので飛ばす
	{
		Accessory::pushBackID(Parse<uint16>(csv[row][0]));
		Accessory::pushBackDescriptionEN(csv[row][1]);
		Accessory::pushBackDescriptionJP(csv[row][2]);
		Accessory::pushBackDescriptionDetailJP(csv[row][3]);
		Accessory::pushBackAlready(csv[row][4]);
		Accessory::pushBackWrist(csv[row][5]);
		Accessory::pushBackFinger(csv[row][6]);
		Accessory::pushBackNecklaces(csv[row][7]);
		Accessory::pushBackCrowns(csv[row][8]);
	}

	for (uint16 i = 3428; i <= 3913; i += 5)
	{
		String path = discriptionJPTemplateFolderPath + U"/" + Format(i) + U".jpg";
		discriptionsImagesPathList.push_back(path);
		getData().binarizedAbilities.push_back(Image{ path }.thresholded(128));
		Console << path << U" を読み込みました";
	}

	for (String statusIconFileName : getData().statusTypeFileName)
	{
		String path = statusIconsFolderPath + U"/" + statusIconFileName + U".jpg";
		statusIconsPathList.push_back(path);
		Image icon = Image{ path };
		getData().icons.push_back(icon);
		getData().binarizedIcons.push_back(icon.thresholded(128));
		Console << path << U" を読み込みました";
	}

	for (int i = 0; i < 10; i++)
	{
		String path = UnkonwnMatterNumbersPath + U"/" + Format(i) + U".jpg";
		getData().binarizedUnkownMatterNumbers.push_back(Image{ path }.thresholded(128));
		Console << path << U" を読み込みました";
	}
	getData().binarizedUnkownMatterNumbers.push_back(Image{ U"images/UnknownMatterNumbers/null.jpg" }.thresholded(128));
	Console << U"images/UnknownMatterNumbers/null.jpg" << U" を読み込みました";
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
