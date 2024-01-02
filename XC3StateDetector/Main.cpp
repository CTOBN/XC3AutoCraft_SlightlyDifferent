# include "Common.hpp"
# include "Loading.hpp"
# include "Setting.hpp"
# include "Recording.hpp"



void Main()
{
	Window::SetTitle(U"XC3StateDetector");
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1009);
	Window::Maximize();
	Scene::SetResizeMode(ResizeMode::Keep);
	FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);
	FontAsset::Register(U"SubtitleFont", 30, Typeface::Heavy);
	FontAsset::Register(U"TextFont", 15, Typeface::Bold);

	// シーンマネージャーを作成
	// ここで GameData が初期化される
	App manager;
	manager
		.add<Loading>(U"Loading")
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

