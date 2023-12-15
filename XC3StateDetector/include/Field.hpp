#pragma once

namespace xc3
{
	class Field : public State {
	public:
		Field();
		void handle(Context& context) override;

	private:
		bool hasTransitioned;
	};

}

