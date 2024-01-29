#pragma	once

namespace xc3
{
	class SystemMenu : public State {
	public:
		SystemMenu(GameData config);
		void handle(Context& context) override;
	};
}

