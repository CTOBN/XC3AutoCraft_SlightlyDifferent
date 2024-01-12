#pragma once

constexpr int32 FrameThickness = 1;
constexpr int32 ScrollBarWidth = 18;
constexpr int32 DisplayRegionFrameThickness = 2;

class OpenableListBox
{
public:
	OpenableListBox(const Font font, const Vec2& pos, const double width, const double height, const size_t displayCount);
	mutable ListBoxState listBoxState;
	void emplace_back(String item);
	void setItems(const Array<String>& items);
	void setIndex(const size_t index);
	void setIndexFromItem(const String& item);
	size_t getSelectedIndex() const;
	String getSelectedItem() const;
	bool getIsOpen() const;
	void update();
	void draw() const;
	RectF getDisplayRegion() const;
	static void closeCurrentOpeningListBox();
private:
	static OpenableListBox* currentOpeningListBox;
	Font m_font;
	RectF m_displayRegion;
	mutable bool m_isOpen;
	size_t m_displayCount;
};
