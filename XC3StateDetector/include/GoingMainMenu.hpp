#pragma once

namespace xc3
{
	class GoingMainMenu : public State {
	public:
		GoingMainMenu(GameData config);
		void handle(Context& context) override;
	};
}

