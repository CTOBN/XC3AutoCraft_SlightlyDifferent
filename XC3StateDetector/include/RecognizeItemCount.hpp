#pragma once

namespace xc3
{
	class RecognizeItemCount : public State {
	public:
		RecognizeItemCount(GameData config);
		void handle(Context& context) override;
	};

}

