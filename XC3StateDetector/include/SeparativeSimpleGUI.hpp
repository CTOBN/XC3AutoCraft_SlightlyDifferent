#pragma once

namespace SeparativeSimpleGUI
{
	class Button
	{
	public:
		Button(const String label, const int32 font_size, const Vec2& pos, const Optional<double>& _width = unspecified, const bool enabled = true);
		bool isPushed() const;
		void update();
		void draw() const;
	private:
		String m_label;
		Font m_font;
		const Vec2 m_pos;
		double m_width;
		bool m_enabled;
		Vec2 m_center;
		RectF m_rect;
		Vec2 m_labelPos;
		bool m_mouseOver;
		bool m_pushed;
		RoundRect m_rrect;
	};

	[[nodiscard]]
	inline constexpr ColorF GetTextColor(bool enabled) noexcept;
}
