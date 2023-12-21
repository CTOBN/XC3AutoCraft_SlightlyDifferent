#pragma	once

namespace xc3
{
	class SystemMenu : public State {
	public:
		SystemMenu();
		void handle(Context& context) override;
	};
}

