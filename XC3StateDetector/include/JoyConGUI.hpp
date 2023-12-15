#pragma	once




class JoyConGUI {
public:
	JoyConGUI() = default;
	JoyConGUI(Vec2 L_pos, Vec2 R_pos);
	enum struct Direction
	{
		Up,
		Down,
		Left,
		Right
	};
	bool getButtonA() const;
	bool getButtonB() const;
	bool getButtonX() const;
	bool getButtonY() const;
	bool getButtonL() const;
	bool getButtonR() const;
	bool getButtonZL() const;
	bool getButtonZR() const;
	bool getButtonPlus() const;
	bool getButtonMinus() const;
	bool getButtonLStickClick() const;
	bool getButtonRStickClick() const;

	bool getButtonHome() const;
	bool getButtonCapture() const;
	bool getButtonLStickUp() const;
	bool getButtonLStickDown() const;
	bool getButtonLStickLeft() const;
	bool getButtonLStickRight() const;
	bool getButtonRStickUp() const;
	bool getButtonRStickDown() const;
	bool getButtonRStickLeft() const;
	bool getButtonRStickRight() const;

	bool getButtonUp() const;
	bool getButtonDown() const;
	bool getButtonLeft() const;
	bool getButtonRight() const;

	Vec2 getLStickValue() const;
	Vec2 getRStickValue() const;
	Vec2 calculateStickValue(const Circle& circle, const Circle innerCircle) const;
	Vec2 calculateStickDirectionAsVector(const Circle& circle, const Circle innerCircle) const;
	int8 calculateStickDirectionAsInt(const Circle& circle, const Circle innerCircle) const;
	bool isMouseOverArc(const Circle& circle, const Circle& innerCircle, Direction direction) const;
	void updateLStickDirection();
	void updateRStickDirection();
	void updateJoyConL();
	void updateJoyConR();
	void drawButtonLLetters() const;
	void drawButtonRLetters() const;
	void drawJoyConL() const;
	void drawJoyConR() const;
	void update();
	void draw() const;
private:
	struct Button {
		bool A = false;
		bool B = false;
		bool X = false;
		bool Y = false;
		bool L = false;
		bool R = false;
		bool ZL = false;
		bool ZR = false;
		bool Plus = false;
		bool Minus = false;
		bool Home = false;
		bool Capture = false;
		bool LStickClick = false;
		bool LStickUp = false;
		bool LStickDown = false;
		bool LStickLeft = false;
		bool LStickRight = false;
		bool RStickClick = false;
		bool RStickUp = false;
		bool RStickDown = false;
		bool RStickLeft = false;
		bool RStickRight = false;
		bool Up = false;
		bool Down = false;
		bool Left = false;
		bool Right = false;
	};


	Button button;

	// ボタンの色
	const ColorF m_buttonColor = Palette::Black;

	// ボタンがマウスカーソルの上にあるときの色
	const ColorF m_buttonColorOver = Palette::Gray;

	// ボタンが押されているときの色
	const ColorF m_buttonColorPushed = Palette::Red;

	// const Font m_font{ 5 };
	const Font m_font{ FontMethod::MSDF, 100, Typeface::Bold };

	const ColorF m_fontColor = Palette::Darkgray;

	// 左ジョイコンの色
	const ColorF m_joyConLColor = Palette::Skyblue;

	// 右ジョイコンの色
	const ColorF m_joyConRColor = Palette::Tomato;

	// ジョイコンのサイズ
	const double m_joyConSize = 400.0;

	// ジョイコンの角度
	const double m_joyConAngle = 0.0;

	const Vec2 m_joyConLPos = Vec2{ 0.0, 0.0 };

	// ジョイコンの位置
	const Vec2 m_joyConRPos = Vec2{ 0.0, 0.0 };

	const double m_Stick_radius = 1.2;

	const double m_Stick_click_radius = 0.5;

	Vec2 LStickValue = { 128, 128 };
	Vec2 RStickValue = { 128, 128 };
	Vec2 LStickDirection = { 0, 0 };
	Vec2 RStickDirection = { 0, 0 };

	Circle body_left_1;
	RectF  body_left_2;
	Circle body_left_3;
	RectF  body_left_4;

	Circle m_left_arrow;
	Circle m_right_arrow;
	Circle m_up_arrow;
	Circle m_down_arrow;

	Triangle m_left_arrow_triangle;
	Triangle m_right_arrow_triangle;
	Triangle m_up_arrow_triangle;
	Triangle m_down_arrow_triangle;


	Circle m_left_stick;
	Circle m_left_click;

	RoundRect m_screenshot;

	RectF m_minus;

	Circle m_right_body_1;
	RectF  m_right_body_2;
	Circle m_right_body_3;
	RectF  m_right_body_4;

	Circle m_A;
	Circle m_B;
	Circle m_X;
	Circle m_Y;

	Circle  m_right_stick;
	Circle  m_right_click;

	Circle m_home;

	Shape2D m_plus;

	Polygon m_L;
	Polygon m_ZL;

	Polygon m_R;
	Polygon m_ZR;

	const Glyph glyph_A = m_font.getGlyph(U'A');
	const Glyph glyph_B = m_font.getGlyph(U'B');
	const Glyph glyph_X = m_font.getGlyph(U'X');
	const Glyph glyph_Y = m_font.getGlyph(U'Y');
	const Glyph glyph_L = m_font.getGlyph(U'L');
	const Glyph glyph_R = m_font.getGlyph(U'R');
	const Glyph glyph_Z = m_font.getGlyph(U"Z");

	const Glyph glyph_HOME = m_font.getGlyph(U'⌂');
	const Glyph glyph_CAPTURE = m_font.getGlyph(U'○');

};
