# include "OpenableListBox.hpp"

OpenableListBox* OpenableListBox::currentOpeningListBox = nullptr;

OpenableListBox::OpenableListBox(const Font font, const Vec2& pos, const double width, const double height, const size_t displayCount)
	: m_isOpen(false)
	, m_font(font)
	, m_displayCount(displayCount)
	, m_displayRegion{ pos, Max(width, 40.0), Max<double>(height, font.height() + (DisplayRegionFrameThickness * 2)) }
{
	listBoxState.selectedItemIndex = 0;
}

void OpenableListBox::emplace_back(String item)
{
	listBoxState.items.emplace_back(item);
}

void OpenableListBox::setItems(const Array<String>& items)
{
	listBoxState.items = items;
}

size_t OpenableListBox::getSelectedIndex() const
{
	return *listBoxState.selectedItemIndex;
}

String OpenableListBox::getSelectedItem() const
{
	if (listBoxState.selectedItemIndex)
	{
		return listBoxState.items[*listBoxState.selectedItemIndex];
	}
	else
	{
		return String();
	}
}

bool OpenableListBox::getIsOpen() const
{
	return m_isOpen;
}

void OpenableListBox::update()
{
	if (m_displayRegion.leftClicked())
	{
		m_isOpen = !m_isOpen;
		if (currentOpeningListBox != nullptr && currentOpeningListBox != this)
		{
			currentOpeningListBox->m_isOpen = false;
		}
		currentOpeningListBox = this;
	}

	if (m_isOpen)
	{
		for (size_t i = 0; i < m_displayCount; ++i)
		{
			Vec2 pos = m_displayRegion.pos.movedBy(0, (SimpleGUI::GetFont().height()) * i + m_displayRegion.h);
			const double RectWidth = (m_displayCount < listBoxState.items.size()) ? (m_displayRegion.size.x - ScrollBarWidth) : m_displayRegion.size.x;
			if (const RectF rect{ pos, { RectWidth, SimpleGUI::GetFont().height() } }; rect.leftClicked())
			{
				listBoxState.selectedItemIndex = i;
				m_isOpen = false;
			}
		}
	}
}

void OpenableListBox::draw() const
{
	m_displayRegion.draw().drawFrame(DisplayRegionFrameThickness, 0, m_isOpen ? Palette::Orange : Palette::Gray);

	m_font(listBoxState.items[*listBoxState.selectedItemIndex]).draw(Arg::leftCenter(m_displayRegion.leftCenter().x + 5, m_displayRegion.leftCenter().y), Palette::Black);

	Triangle{ m_displayRegion.rightCenter().movedBy(-20, 0), (m_displayRegion.h / 3.0), 180_deg }.draw(Palette::Black);

	if (m_isOpen)
	{
		if (SimpleGUI::ListBox(listBoxState, { m_displayRegion.pos.movedBy(0, m_displayRegion.h) }, m_displayRegion.w, (SimpleGUI::GetFont().height() + FrameThickness) * m_displayCount))
		{
			m_isOpen = false;
		}
		for (size_t i = 0; i < m_displayCount; ++i)
		{
			Vec2 pos = m_displayRegion.pos.movedBy(0, (SimpleGUI::GetFont().height()) * i + m_displayRegion.h);
			const double RectWidth = (m_displayCount < listBoxState.items.size()) ? (m_displayRegion.size.x - ScrollBarWidth) : m_displayRegion.size.x;
			if (const RectF rect{ pos, { RectWidth, SimpleGUI::GetFont().height() } }; rect.mouseOver())
			{
				rect.draw(Color{ 135, 206, 235, 128 });
			}
		}
	}
}

RectF OpenableListBox::getDisplayRegion() const
{
	return m_displayRegion;
}
