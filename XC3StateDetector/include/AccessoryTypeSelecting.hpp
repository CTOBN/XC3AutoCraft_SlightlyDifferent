#pragma once

namespace xc3
{
	class AccessoryTypeSelecting : public State {
	public:
		AccessoryTypeSelecting(GameData config);
		void handle(Context& context) override;
	};
}
