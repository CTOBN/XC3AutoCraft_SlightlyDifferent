# pragma once
# include "Common.hpp"

class Loading : public App::Scene
{
public:
	Loading(const InitData& init);
	void update() override;
	void draw() const override;
	void loadConfigINI();
	void loadSpecialEffectImages(const String gameLanguage);
	void loadGameScenes(const String gameLanguage);
	void loadRestSpotMenus(const String gameLanguage);
private:
	const CSV AccessoryCSV{ Resource(U"csv/accessory.csv") };
	const CSV TranslationCSV{ Resource(U"csv/translation.csv") };
	const String SpecialEffectFolderPath = U"images/SpecialEffect";
	const String statusIconsFolderPath = U"images/StatusType";
	const String AccessoryTypeFolderPath = U"images/AccessoryType";
	const String EnigmatterNumbersPath = U"images/EnigmatterNumbers";
	const String GameScenesFolderPath = U"images/GameScenes";
	const String RestSpotMenuFolderPath = U"images/RestSpotMenu";
	Array<String> statusIconsPathList;
	const Polygon diamond{ Vec2{ 10, 0 }, Vec2{ 55, 0 }, Vec2{ 65, 15 },
	Vec2{ 50, 35 }, Vec2{ 15, 35 }, Vec2{ 0, 15 } };
	const Circle ring{ {Scene::Center().x, Scene::Center().y - 200}, 50 };

	void loadAccessoryCSV();
	void loadTranslationCSV();
	void loadStatusIcons();
	void loadAccessoryTypeImages();
	void loadEnigmatterNumbers();
};
