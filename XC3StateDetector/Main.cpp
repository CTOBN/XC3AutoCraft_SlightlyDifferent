# include "Common.hpp"
# include "Loading.hpp"
# include "Config.hpp"
# include "Setting.hpp"
# include "Recording.hpp"


#if USE_TEST
#include "Test.hpp"

class TestRunner
{
public:
	static int run()
	{
		Console.open();

		doctest::Context context;

		// overrides
		context.setOption("no-breaks", true); // don't break in the debugger when assertions fail

		int result = context.run(); // run

		if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
			return result; // propagate the result of the tests

		// テスト実行
		bool testSuccess = result == 0;
		if (!testSuccess)
		{
			// テスト失敗時

			// 失敗に気づきやすいようにキー入力を待つようにする
			// 失敗時にはゲーム画面は表示されず、コンソール画面のみ表示される
			static_cast<void>(std::getchar());
		}

		return testSuccess;
	}
};
#endif

void Main()
{
#if USE_TEST
	TestRunner::run();
#endif
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
	FontAsset::Register(U"AccessoryFont", 12, Typeface::Bold);

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

