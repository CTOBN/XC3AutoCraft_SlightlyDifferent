#pragma once

namespace xc3
{
	class GotDesiredAcc : public State {
	public:
		GotDesiredAcc(GameData config);
		void handle(Context& context) override;
	};
}



