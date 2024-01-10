#pragma once

namespace xc3
{
	class RestSpot : public State {
	public:
		RestSpot();
		void handle(Context& context) override;
	};
}

