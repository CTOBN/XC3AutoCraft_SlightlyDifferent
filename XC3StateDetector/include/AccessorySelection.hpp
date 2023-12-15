#pragma once

namespace xc3
{
	class AccessorySelection : public State {
	public:
		AccessorySelection ();
		void handle(Context& context) override;
	private:
		bool hasTransitioned;
	};

}









