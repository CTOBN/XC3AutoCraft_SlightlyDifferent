#pragma	once

class Pulldown
{
public:
	Pulldown();
	Pulldown(const Array<String>& items, const Font& font, const Point& pos = { 0,0 });

	bool isEmpty() const;
	void update();
	void draw() const;
	void setPos(const Point& pos);
	const Rect getRect() const;
	size_t getIndex() const;
	String getItem() const;
	bool getIsOpen() const;
	void setIndex(const size_t& index);
private:
	Font m_font;
	Array<String> m_items;
	size_t m_index = 0;
	Size m_padding{ 6, 2 };
	Rect m_rect;
	int32 m_downButtonSize = 16;
	bool m_isOpen = false;
	int32 m_maxDisplayItems = 5; // Maximum number of display items
	int32 m_displayStartIndex = 0; // Index of the first item to be displayed
};
