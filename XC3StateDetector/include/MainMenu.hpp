#pragma once

namespace xc3
{
	class MainMenu : public State {
	public:
		MainMenu();
		void handle(Context& context) override;
	};
}

