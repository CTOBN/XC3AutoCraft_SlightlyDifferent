# include "JoyConGUI.hpp"

JoyConGUI::JoyConGUI(Vec2 L_pos, Vec2 R_pos)
	: m_joyConLPos{ L_pos }
	, m_joyConRPos{ R_pos }
{
	const Vec2 size = Vec2{ 5.27, 15.33 };
	RectF base{ Arg::center = m_joyConLPos, size };


	// body
	{
		body_left_1 = Circle{ base.pos.movedBy(3.0, 3.6), 3.0 };
		body_left_2 = RectF{ base.pos.movedBy(0.0, 3.6), Vec2{ 3.0, 8.63 } };
		body_left_3 = Circle{ base.bl().movedBy(3.0, -3.1), 3.0 };
		body_left_4 = RectF{ base.pos.movedBy(3.00, 0.6), Vec2{ 1.82, 14.63 } };
	}

	// arrow
	{
		const double delta = 1.15;
		const Vec2 c = base.pos.movedBy(2.55, 8.33);

		m_left_arrow = Circle{ c.movedBy(-delta, 0), 0.55 }; // left
		m_down_arrow = Circle{ c.movedBy(0, delta), 0.55 }; // down
		m_up_arrow = Circle{ c.movedBy(0, -delta), 0.55 }; // up
		m_right_arrow = Circle{ c.movedBy(delta, 0), 0.55 }; // right

		m_left_arrow_triangle = Triangle{ c.movedBy(-delta, 0),  0.55, -90_deg };
		m_down_arrow_triangle = Triangle{ c.movedBy(0, delta), 0.55, 180_deg };
		m_up_arrow_triangle = Triangle{ c.movedBy(0, -delta), 0.55 };
		m_right_arrow_triangle = Triangle{ c.movedBy(delta, 0), 0.55, 90_deg };

	}

	// stick
	{
		const Vec2 c = base.pos.movedBy(2.55, 4.28);
		m_left_stick = Circle{ c, m_Stick_radius };
		m_left_click = Circle{ c, m_Stick_click_radius };
	}

	// Screenshot
	m_screenshot = RoundRect{ Arg::center = base.tl().movedBy(3.4, 11.13), Vec2{ 1.0, 1.0 }, 0.1 };


	// Minus
	m_minus = RectF{ Arg::center = base.tl().movedBy(4.03, 2.0), Vec2{ 0.8, 0.2 } };

	// L, ZL
	m_L = RectF{ base.tl().movedBy(2.0, -0.1), Vec2{ 2.8, 0.8 } }.rounded(0.3, 0.3, 0, 0);
	m_ZL = RectF{ base.tl().movedBy(2.0, -1.8), Vec2{ 2.8, 1.6 } }.rounded(0.3, 0.3, 0, 0);

	base = { Arg::center = m_joyConRPos, size };

	const Vec2 baseTr = base.tr();

	// body
	{
		const Vec2 tr = baseTr;
		m_right_body_1 = Circle{ tr.movedBy(-3.0, 3.6), 3.0 };
		m_right_body_2 = RectF{ tr.movedBy(-3.0, 3.6), Vec2{ 3.0, 8.63 } };
		m_right_body_3 = Circle{ base.br().movedBy(-3.0, -3.1), 3.0 };
		m_right_body_4 = RectF{ tr.movedBy(-4.82, 0.6), Vec2{ 1.82, 14.63 } };
	}

	// ABXY
	{
		const double delta = 1.15;
		const Vec2 c = baseTr.movedBy(-2.55, 4.28);

		m_A = Circle{ c.movedBy(delta, 0), 0.55 }; // A
		m_X = Circle{ c.movedBy(0, -delta), 0.55 }; // X
		m_B = Circle{ c.movedBy(0, delta), 0.55 }; // B
		m_Y = Circle{ c.movedBy(-delta, 0), 0.55 }; // Y
	}

	// stick
	{
		const Vec2 c = baseTr.movedBy(-2.55, 8.33);
		m_right_stick = Circle{ c, m_Stick_radius };
		m_right_click = Circle{ c, m_Stick_click_radius };
	}

	// Home
	m_home = Circle{ baseTr.movedBy(-3.4, 11.13), 0.55 };

	// Plus
	m_plus = Shape2D::Plus(0.4, 0.2, baseTr.movedBy(-4.03, 2.0));

	// R, ZR
	m_R = RectF{ baseTr.movedBy(-4.8, -0.1), Vec2{ 2.8, 0.8 } }.rounded(0.3, 0.3, 0, 0);
	m_ZR = RectF{ baseTr.movedBy(-4.8, -1.8), Vec2{ 2.8, 1.6 } }.rounded(0.3, 0.3, 0, 0);
}

bool JoyConGUI::getButtonA() const
{
	return button.A;
}

bool JoyConGUI::getButtonB() const
{
	return button.B;
}

bool JoyConGUI::getButtonX() const
{
	return button.X;
}

bool JoyConGUI::getButtonY() const
{
	return button.Y;
}

bool JoyConGUI::getButtonL() const
{
	return button.L;
}

bool JoyConGUI::getButtonZL() const
{
	return button.ZL;
}

bool JoyConGUI::getButtonR() const
{
	return button.R;
}

bool JoyConGUI::getButtonZR() const
{
	return button.ZR;
}

bool JoyConGUI::getButtonUp() const
{
	return button.Up;
}

bool JoyConGUI::getButtonDown() const
{
	return button.Down;
}

bool JoyConGUI::getButtonLeft() const
{
	return button.Left;
}

bool JoyConGUI::getButtonRight() const
{
	return button.Right;
}

bool JoyConGUI::getButtonPlus() const
{
	return button.Plus;
}

bool JoyConGUI::getButtonMinus() const
{
	return button.Minus;
}

bool JoyConGUI::getButtonLStickClick() const
{
	return button.LStickClick;
}

bool JoyConGUI::getButtonRStickClick() const
{
	return button.RStickClick;
}

bool JoyConGUI::getButtonHome() const
{
	return button.Home;
}

bool JoyConGUI::getButtonCapture() const
{
	return button.Capture;
}

bool JoyConGUI::getButtonLStickUp() const
{
	return button.LStickUp;
}

bool JoyConGUI::getButtonLStickDown() const
{
	return button.LStickDown;
}

bool JoyConGUI::getButtonLStickLeft() const
{
	return button.LStickLeft;
}

bool JoyConGUI::getButtonLStickRight() const
{
	return button.LStickRight;
}

bool JoyConGUI::getButtonRStickUp() const
{
	return button.RStickUp;
}

bool JoyConGUI::getButtonRStickDown() const
{
	return button.RStickDown;
}

bool JoyConGUI::getButtonRStickLeft() const
{
	return button.RStickLeft;
}

bool JoyConGUI::getButtonRStickRight() const
{
	return button.RStickRight;
}




Vec2 JoyConGUI::getLStickValue() const
{
	return LStickValue;
}

Vec2 JoyConGUI::getRStickValue() const
{
	return RStickValue;
}

Vec2 JoyConGUI::calculateStickValue(const Circle& circle, const Circle innerCircle) const
{
	const Vec2 center = circle.center;
	const Vec2 mousePos = Cursor::PosF();
	const Vec2 relativePos = mousePos - center;

	// Calculate the angle from the center to the mouse position
	const double angle = Atan2(relativePos.y, relativePos.x);

	// Calculate the distance from the center to the mouse position
	const double distance = Min(relativePos.length(), circle.r);

	if (distance <= innerCircle.r)
	{
		return Vec2{ 128, 128 };
	}

	// Convert polar coordinates(angle, distance) to Cartesian coordinates(x, y)
	const double x = Max(Min(Round(128 * cos(angle) * distance) + 128, 255.0), 0.0);
	const double y = Max(Min(Round(128 * sin(angle) * distance) + 128, 255.0), 0.0);

	return Vec2{ x, y };
}

// 円の中心からのマウスカーソルの相対位置を0から1の範囲に正規化して返す
Vec2 JoyConGUI::calculateStickDirectionAsVector(const Circle& circle, const Circle innerCircle) const
{
	const Vec2 mousePos = Cursor::PosF();
	const Vec2 relativePos = mousePos - circle.center;

	if (!circle.contains(mousePos))
	{
		return Vec2{ 0, 0 };
	}

	const double distance = Min(relativePos.length(), circle.r);
	if (distance <= innerCircle.r)
	{
		return Vec2{ 0, 0 };
	}

	const double angle = Atan2(relativePos.y, relativePos.x);
	const double normalizedAngle = angle < 0 ? angle + (2 * Math::Pi) : angle;

	if (normalizedAngle >= 7 * Math::Pi / 4 || normalizedAngle < Math::Pi / 4)
	{
		return Vec2{ 1, 0 };
	}
	else if (normalizedAngle >= Math::Pi / 4 && normalizedAngle < 3 * Math::Pi / 4)
	{
		return Vec2{ 0, 1 };
	}
	else if (normalizedAngle >= 3 * Math::Pi / 4 && normalizedAngle < 5 * Math::Pi / 4)
	{
		return Vec2{ -1, 0 };
	}
	else if (normalizedAngle >= 5 * Math::Pi / 4 && normalizedAngle < 7 * Math::Pi / 4)
	{
		return Vec2{ 0, -1 };
	}

	return Vec2{ 0, 0 };
}

int8 JoyConGUI::calculateStickDirectionAsInt(const Circle& circle, const Circle innerCircle) const
{
	const Vec2 mousePos = Cursor::PosF();
	const Vec2 relativePos = mousePos - circle.center;

	if (!circle.contains(mousePos))
	{
		return 0;
	}

	const double distance = Min(relativePos.length(), circle.r);
	if (distance <= innerCircle.r)
	{
		return 0;
	}

	const double angle = Atan2(relativePos.y, relativePos.x);
	const double normalizedAngle = angle < 0 ? angle + (2 * Math::Pi) : angle;

	if (normalizedAngle >= 7 * Math::Pi / 4 || normalizedAngle < Math::Pi / 4)
	{
		return 1; // 右
	}
	else if (normalizedAngle >= Math::Pi / 4 && normalizedAngle < 3 * Math::Pi / 4)
	{
		return 2; // 上
	}
	else if (normalizedAngle >= 3 * Math::Pi / 4 && normalizedAngle < 5 * Math::Pi / 4)
	{
		return 3; // 左
	}
	else if (normalizedAngle >= 5 * Math::Pi / 4 && normalizedAngle < 7 * Math::Pi / 4)
	{
		return 4; // 下
	}

	return 0;
}

bool JoyConGUI::isMouseOverArc(const Circle& circle, const Circle& innerCircle, Direction direction) const
{
	const Vec2 mousePos = Cursor::PosF();
	const Vec2 relativePos = mousePos - circle.center;

	if (!circle.contains(mousePos))
	{
		return false;
	}

	const double distance = Min(relativePos.length(), circle.r);
	if (distance <= innerCircle.r)
	{
		return false;
	}
	const double angle = Atan2(relativePos.y, relativePos.x);
	const double normalizedAngle = angle < 0 ? angle + (2 * Math::Pi) : angle;

	if (direction == Direction::Right && (normalizedAngle >= 7 * Math::Pi / 4 || normalizedAngle < Math::Pi / 4))
	{
		return true;
	}
	else if (direction == Direction::Down && (normalizedAngle >= Math::Pi / 4 && normalizedAngle < 3 * Math::Pi / 4))
	{
		return true;
	}
	else if (direction == Direction::Left && (normalizedAngle >= 3 * Math::Pi / 4 && normalizedAngle < 5 * Math::Pi / 4))
	{
		return true;
	}
	else if (direction == Direction::Up && (normalizedAngle >= 5 * Math::Pi / 4 && normalizedAngle < 7 * Math::Pi / 4))
	{
		return true;
	}
	return false;
}

void JoyConGUI::updateLStickDirection()
{
	button.LStickUp    = isMouseOverArc(m_left_stick, m_left_click, Direction::Up)    && m_left_stick.leftClicked();
	button.LStickDown  = isMouseOverArc(m_left_stick, m_left_click, Direction::Down)  && m_left_stick.leftClicked();
	button.LStickLeft  = isMouseOverArc(m_left_stick, m_left_click, Direction::Left)  && m_left_stick.leftClicked();
	button.LStickRight = isMouseOverArc(m_left_stick, m_left_click, Direction::Right) && m_left_stick.leftClicked();
}

void JoyConGUI::updateRStickDirection()
{
	button.RStickUp    = isMouseOverArc(m_right_stick, m_right_click, Direction::Up)    && m_right_stick.leftClicked();
	button.RStickDown  = isMouseOverArc(m_right_stick, m_right_click, Direction::Down)  && m_right_stick.leftClicked();
	button.RStickLeft  = isMouseOverArc(m_right_stick, m_right_click, Direction::Left)  && m_right_stick.leftClicked();
	button.RStickRight = isMouseOverArc(m_right_stick, m_right_click, Direction::Right) && m_right_stick.leftClicked();
}

void JoyConGUI::updateJoyConL()
{
	Transformer2D t{ Mat3x2::Scale(20, m_joyConLPos).rotated(m_joyConAngle, m_joyConLPos), TransformCursor::Yes };

	button.L = m_L.leftClicked();
	button.ZL = m_ZL.leftClicked();
	button.Minus = m_minus.leftClicked();
	button.Left = m_left_arrow.leftClicked();
	button.Right = m_right_arrow.leftClicked();
	button.Up = m_up_arrow.leftClicked();
	button.Down = m_down_arrow.leftClicked();
	button.Capture = m_screenshot.leftClicked();
	button.LStickClick = m_left_click.leftClicked();
	updateLStickDirection();
	LStickValue = calculateStickValue(m_left_stick, m_left_click);
	LStickDirection = calculateStickDirectionAsVector(m_left_stick, m_left_click);
}

void JoyConGUI::updateJoyConR()
{
	Transformer2D t{ Mat3x2::Scale(20, m_joyConRPos).rotated(m_joyConAngle, m_joyConRPos), TransformCursor::Yes };
	button.A = m_A.leftClicked();
	button.B = m_B.leftClicked();
	button.X = m_X.leftClicked();
	button.Y = m_Y.leftClicked();
	button.R = m_R.leftClicked();
	button.ZR = m_ZR.leftClicked();
	button.Plus = m_plus.asPolygon().leftClicked();
	button.Home = m_home.leftClicked();
	button.RStickClick = m_right_click.leftClicked();
	updateRStickDirection();
	RStickValue = calculateStickValue(m_right_stick, m_right_click);
	RStickDirection = calculateStickDirectionAsVector(m_right_stick, m_right_click);
}

void JoyConGUI::drawButtonLLetters() const
{
	const ScopedCustomShader2D shader{ Font::GetPixelShader(m_font.method()) };

	glyph_L.texture.scaled(0.008).drawAt(m_L.centroid(), m_fontColor);
	glyph_Z.texture.scaled(0.008).drawAt({ m_ZL.centroid().x - 0.25, m_ZL.centroid().y }, m_fontColor);
	glyph_L.texture.scaled(0.008).drawAt({ m_ZL.centroid().x + 0.25, m_ZL.centroid().y }, m_fontColor);

	glyph_CAPTURE.texture.scaled(0.008).drawAt(m_screenshot.center(), m_fontColor);
}


void JoyConGUI::drawButtonRLetters() const
{
	const ScopedCustomShader2D shader{ Font::GetPixelShader(m_font.method()) };

	glyph_A.texture.scaled(0.008).drawAt(m_A.center, m_fontColor);
	glyph_B.texture.scaled(0.008).drawAt(m_B.center, m_fontColor);
	glyph_X.texture.scaled(0.008).drawAt(m_X.center, m_fontColor);
	glyph_Y.texture.scaled(0.008).drawAt(m_Y.center, m_fontColor);

	glyph_R.texture.scaled(0.008).drawAt(m_R.centroid(), m_fontColor);
	glyph_Z.texture.scaled(0.008).drawAt({ m_ZR.centroid().x - 0.25, m_ZR.centroid().y }, m_fontColor);
	glyph_R.texture.scaled(0.008).drawAt({ m_ZR.centroid().x + 0.25, m_ZR.centroid().y }, m_fontColor);

	glyph_HOME.texture.scaled(0.010).drawAt(m_home.center, m_fontColor);
}

void JoyConGUI::drawJoyConL() const
{
	const auto buttonColor_forCircle = [=](const Circle& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	const auto buttonColor_forOuterCircle = [=](const Circle& outer, const Circle& inner, Direction direction) {
		return (isMouseOverArc(outer, inner, direction) && MouseL.pressed() ? m_buttonColorPushed
		: isMouseOverArc(outer, inner, direction) ? m_buttonColorOver
		: m_buttonColor);
		};

	const auto buttonColor_forRect = [=](const RectF& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	const auto buttonColor_forPolygon = [=](const Polygon& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};


	const auto buttonColor_forRoundRect = [=](const RoundRect& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	Transformer2D t{ Mat3x2::Scale(20, m_joyConLPos).rotated(m_joyConAngle, m_joyConLPos), TransformCursor::Yes };

	body_left_1.drawPie(0_deg, -90_deg, m_joyConLColor);
	body_left_2.draw(m_joyConLColor);
	body_left_3.drawPie(180_deg, 90_deg, m_joyConLColor);
	body_left_4.draw(m_joyConLColor);

	m_left_arrow.draw(buttonColor_forCircle(m_left_arrow));
	m_right_arrow.draw(buttonColor_forCircle(m_right_arrow));
	m_up_arrow.draw(buttonColor_forCircle(m_up_arrow));
	m_down_arrow.draw(buttonColor_forCircle(m_down_arrow));

	m_left_arrow_triangle.draw(m_fontColor);
	m_right_arrow_triangle.draw(m_fontColor);
	m_up_arrow_triangle.draw(m_fontColor);
	m_down_arrow_triangle.draw(m_fontColor);

	// stick と click の間に隙間があるので、下地として描画
	m_left_stick.draw(buttonColor_forCircle(m_left_stick));

	m_left_stick.drawArc(-45_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_left_stick, m_left_click, Direction::Up)); // 上
	m_left_stick.drawArc(135_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_left_stick, m_left_click, Direction::Down)); // 下
	m_left_stick.drawArc(-135_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_left_stick, m_left_click, Direction::Left)); // 左
	m_left_stick.drawArc(45_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_left_stick, m_left_click, Direction::Right)); // 右
	

	m_left_click.draw(buttonColor_forCircle(m_left_click));
	m_screenshot.draw(buttonColor_forRoundRect(m_screenshot));
	m_minus.draw(buttonColor_forRect(m_minus));

	m_L.draw(buttonColor_forPolygon(m_L));
	m_ZL.draw(buttonColor_forPolygon(m_ZL));

	drawButtonLLetters();
}


void JoyConGUI::drawJoyConR() const
{
	const auto buttonColor_forCircle = [=](const Circle& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	const auto buttonColor_forOuterCircle = [=](const Circle& outer, const Circle& inner, Direction direction) {
		return (isMouseOverArc(outer, inner, direction) && MouseL.pressed() ? m_buttonColorPushed
		: isMouseOverArc(outer, inner, direction) ? m_buttonColorOver
		: m_buttonColor);
		};

	const auto buttonColor_forRect = [=](const RectF& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	const auto buttonColor_forPolygon = [=](const Polygon& input) {
		return (input.leftPressed() ? m_buttonColorPushed : input.mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	const auto buttonColor_forShape2D = [=](const Shape2D& input) {
		return (input.asPolygon().leftPressed() ? m_buttonColorPushed : input.asPolygon().mouseOver() ? m_buttonColorOver : m_buttonColor);
		};

	Transformer2D t{ Mat3x2::Scale(20, m_joyConRPos).rotated(m_joyConAngle, m_joyConRPos), TransformCursor::Yes };

	m_right_body_1.drawPie(0_deg, 90_deg, m_joyConRColor);
	m_right_body_2.draw(m_joyConRColor);
	m_right_body_3.drawPie(180_deg, -90_deg, m_joyConRColor);;
	m_right_body_4.draw(m_joyConRColor);

	m_A.draw(buttonColor_forCircle(m_A));
	m_B.draw(buttonColor_forCircle(m_B));
	m_X.draw(buttonColor_forCircle(m_X));
	m_Y.draw(buttonColor_forCircle(m_Y));

	// stick と click の間に隙間があるので、下地として描画
	m_right_stick.draw(buttonColor_forCircle(m_right_stick));

	m_right_stick.drawArc(-45_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_right_stick, m_right_click, Direction::Up)); // 上
	m_right_stick.drawArc(135_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_right_stick, m_right_click, Direction::Down)); // 下
	m_right_stick.drawArc(-135_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_right_stick, m_right_click, Direction::Left)); // 左
	m_right_stick.drawArc(45_deg, 90_deg, 0.7, 0, buttonColor_forOuterCircle(m_right_stick, m_right_click, Direction::Right)); // 右

	m_right_click.draw(buttonColor_forCircle(m_right_click));
	m_home.draw(buttonColor_forCircle(m_home));
	m_plus.draw(buttonColor_forShape2D(m_plus));

	m_R.draw(buttonColor_forPolygon(m_R));
	m_ZR.draw(buttonColor_forPolygon(m_ZR));

	drawButtonRLetters();
}

void JoyConGUI::update()
{
	updateJoyConL();
	updateJoyConR();
}

void JoyConGUI::draw() const
{
	// 左ジョイコン
	drawJoyConL();
	// 右ジョイコン
	drawJoyConR();
}
