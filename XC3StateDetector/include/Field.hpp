#pragma once

namespace xc3
{
	class Field : public State {
	public:
		Field(GameData config);
		void handle(Context& context) override;
	};

}

