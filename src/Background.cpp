#include "Background.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance()->load("../Assets/textures/oceanbackground.png", "background");

	const auto size = TextureManager::Instance()->getTextureSize("background");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(BACKGROUND);
}

Background::~Background()
= default;

void Background::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("background", x, y, 0, 255, false);
}

void Background::update()
{
}

void Background::clean()
{
}
