#pragma once

namespace xc3
{
	class GotDesiredAcc : public State {
	public:
		GotDesiredAcc();
		void handle(Context& context) override;
	};
}



