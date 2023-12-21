#pragma once

namespace xc3
{
	class Camp : public State {
	public:
		Camp();
		void handle(Context& context) override;
	};
}

