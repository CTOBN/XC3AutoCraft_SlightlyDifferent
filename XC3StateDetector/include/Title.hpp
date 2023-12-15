#pragma once

namespace xc3
{
	class Title : public State {
	public:
		Title();
		void handle(Context& context) override;
	private:
		bool hasTransitioned;
	};

}

