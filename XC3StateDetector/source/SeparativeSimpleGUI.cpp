#include "SeparativeSimpleGUI.hpp"

namespace SeparativeSimpleGUI
{
	constexpr int32 UnitSize = 36;
	constexpr ColorF BackgroundColor{ 1.0 };
	constexpr ColorF DisabledBackgroundColor{ 0.92 };
	constexpr ColorF ButtonMouseOverColor{ 0.9, 0.95, 1.0 };
	constexpr ColorF ActiveTextColor{ 0.11 };
	constexpr ColorF DisabledTextColor{ 0.67 };
	constexpr int32 FontYOffset = -1;
	constexpr double RoundSize = 4.8;
	constexpr ColorF FrameColor{ 0.67 };

	[[nodiscard]]
	inline constexpr ColorF GetTextColor(bool enabled) noexcept
	{
		return (enabled ? ActiveTextColor : DisabledTextColor);
	}

	Button::Button(const String label, const int32 fontSize, const Vec2& pos, const Optional<double>& _width, const bool enabled)
		: m_label(label), m_pos(pos), m_enabled(enabled), m_mouseOver(false), m_pushed(false)
	{
		m_font = Font{ FontMethod::MSDF, fontSize, Typeface::CJK_Regular_JP };
		const auto dtext = m_font(m_label);
		const double labelWidth = Math::Ceil(dtext.region().w);
		m_width = _width.value_or_eval([&]() { return (labelWidth + 40); });
		m_center = { m_pos.x + m_width / 2.0, m_pos.y + m_font.height() / 2.0 };
		m_rect = { Arg::center = m_center, m_width, (double)m_font.height() };
		m_labelPos = { (m_rect.x + (m_width - labelWidth) / 2.0), (m_center.y - m_font.height() / 2.0 + FontYOffset) };
		m_rrect = m_rect.rounded(RoundSize);
	}

	void Button::updateValues()
	{
		const auto dtext = m_font(m_label);
		const double labelWidth = Math::Ceil(dtext.region().w);
		m_width = labelWidth + 40;
		m_center = { m_pos.x + m_width / 2.0, m_pos.y + m_font.height() / 2.0 };
		m_rect = { Arg::center = m_center, m_width, (double)m_font.height() };
		m_labelPos = { (m_rect.x + (m_width - labelWidth) / 2.0), (m_center.y - m_font.height() / 2.0 + FontYOffset) };
		m_rrect = m_rect.rounded(RoundSize);
	}

	bool Button::isPushed() const
	{
		return m_pushed;
	}

	void Button::update(const Optional<String> label)
	{
		if (label.has_value())
		{
			m_label = label.value();
			updateValues();
		}
		m_mouseOver = (m_enabled && m_rect.mouseOver());
		m_pushed = (m_mouseOver && Cursor::OnClientRect() && MouseL.down());
	}

	void Button::draw() const
	{
		if (m_enabled)
		{
			m_rrect.draw(m_mouseOver ? ButtonMouseOverColor : BackgroundColor);
		}
		else
		{
			m_rrect.draw(DisabledBackgroundColor);
		}

		if (!m_pushed)
		{
			m_rrect.drawFrame(1, 0, FrameColor);
		}

		m_font(m_label).draw(m_labelPos, GetTextColor(m_enabled));

		if (m_mouseOver)
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}


} // namespace SeparativeSimpleGUI
