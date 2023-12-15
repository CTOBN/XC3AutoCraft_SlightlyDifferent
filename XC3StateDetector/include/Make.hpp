#pragma once

namespace xc3
{
	class Make : public State {
	public:
		Make();
		void handle(Context& context) override;
	private:
		bool hasTransitioned;
	};
}
