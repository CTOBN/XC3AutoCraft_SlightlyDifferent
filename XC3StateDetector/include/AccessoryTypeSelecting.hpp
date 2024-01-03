#pragma once

namespace xc3
{
	class AccessoryTypeSelecting : public State {
	public:
		AccessoryTypeSelecting();
		void handle(Context& context) override;
	};

}
