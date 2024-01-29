#pragma once

namespace xc3
{
	class TitleLoading : public State {
	public:
		TitleLoading(GameData config);
		void handle(Context& context) override;
	};
}

