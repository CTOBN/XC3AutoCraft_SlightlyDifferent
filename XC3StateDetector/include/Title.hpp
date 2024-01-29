#pragma once

namespace xc3
{
	class Title : public State {
	public:
		Title(GameData config);
		void handle(Context& context) override;
	};

}

