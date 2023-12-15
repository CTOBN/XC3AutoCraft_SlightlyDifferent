#include "Pulldown.hpp"

Pulldown::Pulldown()
	: m_font{ Font{ 20, Typeface::Bold } }
	, m_rect{ 0, 0, 0, 0 }
{
}

Pulldown::Pulldown(const Array<String>& items, const Font& font, const Point& pos)
	: m_font{ font }
	, m_items{ items }
	, m_rect{ pos, 0, (m_font.height() + m_padding.y * 2) }
{
	for (const auto& item : m_items)
	{
		m_rect.w = Max(m_rect.w, static_cast<int32>(m_font(item).region().w));
	}

	m_rect.w += (m_padding.x * 2 + m_downButtonSize);
	m_maxDisplayItems = Min(m_maxDisplayItems, int32(m_items.size()));
}

bool Pulldown::isEmpty() const
{
	return m_items.empty();
}

const Rect Pulldown::getRect() const
{
	return m_rect;
}

void Pulldown::update()
{
	if (isEmpty())
	{
		return;
	}

	if (m_rect.leftClicked())
	{
		m_isOpen = !m_isOpen;
	}

	if (m_isOpen)
	{
		Point pos = m_rect.pos.movedBy(0, m_rect.h);
		int32 displayedItems = Min(static_cast<int32>(m_items.size()), m_maxDisplayItems);

		for (int32 i = m_displayStartIndex; i < m_displayStartIndex + displayedItems; ++i)
		{
			Rect itemRect{ pos, m_rect.w, m_rect.h };

			if (itemRect.leftClicked())
			{
				m_index = i;
				m_isOpen = false;
				break;
			}

			pos.y += m_rect.h;
		}

		// マウスホイールやスクロールバーによる操作で表示するアイテムの位置を更新
		if (Mouse::Wheel() < 0 && m_displayStartIndex > 0)
		{
			--m_displayStartIndex;
		}
		else if (Mouse::Wheel() > 0 && m_displayStartIndex < m_items.size() - m_maxDisplayItems)
		{
			++m_displayStartIndex;
		}
	}
}

void Pulldown::draw() const
{
	m_rect.draw();

	if (isEmpty())
	{
		return;
	}

	m_rect.drawFrame(1, 0, m_isOpen ? Palette::Orange : Palette::Gray);

	Point pos = m_rect.pos;

	m_font(m_items[m_index]).draw(pos + m_padding, Palette::Black);

	Triangle{ (m_rect.x + m_rect.w - m_downButtonSize / 2.0 - m_padding.x), (m_rect.y + m_rect.h / 2.0),
		(m_downButtonSize * 0.5), 180_deg }.draw(Palette::Black);

	pos.y += m_rect.h;


	if (m_isOpen)
	{
		const Rect backRect{ m_rect.pos.movedBy(0, m_rect.h), m_rect.w, (m_rect.h * m_maxDisplayItems) };

		backRect.drawShadow({ 1, 1 }, 4, 1).draw();

		for (size_t i = m_displayStartIndex; i < m_displayStartIndex + m_maxDisplayItems; ++i)
		{
			pos = m_rect.pos.movedBy(0, m_rect.h * (i - m_displayStartIndex) + m_font.height() + m_padding.y * 2);

			if (const Rect rect{ pos, m_rect.size }; rect.mouseOver())
			{
				rect.draw(Palette::Skyblue);
			}

			m_font(m_items[i]).draw((pos + m_padding), Palette::Black);
		}

		backRect.drawFrame(1, 0, Palette::Gray);
	}
}

void Pulldown::setPos(const Point& pos)
{
	m_rect.setPos(pos);
}

size_t Pulldown::getIndex() const
{
	return m_index;
}

void Pulldown::setIndex(const size_t& index)
{
	m_index = index;
}

String Pulldown::getItem() const
{
	if (isEmpty())
	{
		return{};
	}

	return m_items[m_index];
}

bool Pulldown::getIsOpen() const
{
	return m_isOpen;
}
