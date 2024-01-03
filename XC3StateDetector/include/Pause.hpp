#pragma once

namespace xc3
{
	class Pause : public State {
	public:
		Pause();
		void handle(Context& context) override;
	};

}

