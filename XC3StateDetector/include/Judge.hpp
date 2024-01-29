#pragma once

namespace xc3
{
	class Judge : public State {
	public:
		Judge(GameData config);
		void handle(Context& context) override;
	};
}

