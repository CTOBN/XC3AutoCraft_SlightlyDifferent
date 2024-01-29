#pragma once

namespace xc3
{
	class FieldLoading : public State {
	public:
		FieldLoading(GameData config);
		void handle(Context& context) override;
	};
}

