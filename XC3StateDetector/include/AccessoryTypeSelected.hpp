#pragma once

namespace xc3
{
	class AccessoryTypeSelected : public State {
	public:
		AccessoryTypeSelected(GameData config);
		void handle(Context& context) override;
	};

	using App = SceneManager<String, GameData>;

}









