void DrawVerticalGradientBackground(const ColorF& topColor, const ColorF& bottomColor)
{
	Scene::Rect()
		.draw(Arg::top = topColor, Arg::bottom = bottomColor);
}
