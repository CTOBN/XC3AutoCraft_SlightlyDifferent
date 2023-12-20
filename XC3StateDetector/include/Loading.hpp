﻿# pragma once
# include "Common.hpp"

class Loading : public App::Scene
{
public:
	Loading(const InitData& init);

	void update() override;

	void draw() const override;

private:
	const CSV csv{ U"csv/accessory.csv" };
	const String discriptionJPTemplateFolderPath = U"images/Xenoblade3_Temp_Jp";
	const String statusIconsFolderPath = U"images/StatusType";
	const String UnkonwnMatterNumbersPath = U"images/UnknownMatterNumbers";
	Array<String> discriptionsImagesPathList;
	Array<String> statusIconsPathList;
	const Polygon diamond{ Vec2{ 10, 0 }, Vec2{ 55, 0 }, Vec2{ 65, 15 },
	Vec2{ 50, 35 }, Vec2{ 15, 35 }, Vec2{ 0, 15 } };
	const Circle ring{ {Scene::Center().x, Scene::Center().y - 200}, 50 };
};
