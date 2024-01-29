#pragma once

namespace xc3
{
	class Pause : public State {
	public:
		Pause(GameData config);
		void handle(Context& context) override;
	};

}

