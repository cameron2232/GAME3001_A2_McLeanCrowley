#include "Goal.h"


#include "SoundManager.h"
#include "TextureManager.h"

Goal::Goal()
{
	TextureManager::Instance()->load("../Assets/textures/Goal.png", "goal");

	const auto size = TextureManager::Instance()->getTextureSize("goal");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//setType(GOAL);

	//SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Goal::~Goal()
= default;

void Goal::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("goal", x, y, 0, 255, true);
}

void Goal::update()
{
}

void Goal::clean()
{
}

glm::vec2 Goal::getGridPosition() const
{
	return m_gridPosition;
}

void Goal::setGridPosition(float col, float row)
{
	m_gridPosition = glm::vec2(col, row);
}

void Goal::m_checkBounds()
{
}

void Goal::m_reset()
{
}
	
