# include "Common.hpp"
# include "Loading.hpp"
# include "Config.hpp"
# include "Setting.hpp"
# include "Recording.hpp"



void Main()
{
	const String applicationName = U"XC3StateDetector";
	const String version = U"v1.0.0";
	Window::SetTitle(U"{} {}"_fmt(applicationName, version));
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1009);
	Window::Maximize();
	Scene::SetResizeMode(ResizeMode::Keep);
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);
	FontAsset::Register(U"SubtitleFont", 30, Typeface::Heavy);
	FontAsset::Register(U"TextFont", 20, Typeface::Mplus_Medium);
	FontAsset::Register(U"AccessoryFont", 15, Typeface::Bold);

	// シーンマネージャーを作成
	// ここで GameData が初期化される
	App manager;
	manager
		.add<Loading>(U"Loading")
		.add<Config>(U"Config")
		.add<Setting>(U"Setting")
		.add<Recording>(U"Recording");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

