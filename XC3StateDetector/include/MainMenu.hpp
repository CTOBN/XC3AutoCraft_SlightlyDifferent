#pragma once

namespace xc3
{
	class MainMenu : public State {
	public:
		MainMenu(GameData config);
		void handle(Context& context) override;
	};
}

