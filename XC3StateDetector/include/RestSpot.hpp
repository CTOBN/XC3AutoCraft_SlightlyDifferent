#pragma once

namespace xc3
{
	class RestSpot : public State {
	public:
		RestSpot(GameData config);
		void handle(Context& context) override;
	};
}

