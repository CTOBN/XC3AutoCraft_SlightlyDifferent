#pragma once

namespace xc3
{
	class GoingMainMenu : public State {
	public:
		GoingMainMenu();
		void handle(Context& context) override;
	};
}

