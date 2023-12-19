#pragma once

namespace xc3
{
	class TitleLoading : public State {
	public:
		TitleLoading();
		void handle(Context& context) override;
	private:
		bool hasTransitioned;
	};
}

