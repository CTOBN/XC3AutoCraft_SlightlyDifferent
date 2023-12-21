#pragma once

namespace xc3
{
	class Judge : public State {
	public:
		Judge();
		void handle(Context& context) override;
	};
}

