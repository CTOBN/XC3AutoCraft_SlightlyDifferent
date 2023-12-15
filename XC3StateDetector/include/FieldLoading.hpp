#pragma once

namespace xc3
{
	class FieldLoading : public State {
	public:
		FieldLoading();
		void handle(Context& context) override;
	private:
		bool hasTransitioned;
	};
}

