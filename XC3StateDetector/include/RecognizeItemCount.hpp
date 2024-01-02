#pragma once

namespace xc3
{
	class RecognizeItemCount : public State {
	public:
		RecognizeItemCount();
		void handle(Context& context) override;
	};

}

