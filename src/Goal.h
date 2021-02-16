#pragma once
#ifndef _GOAL_
#define _GOAL_

#include "DisplayObject.h"

class Goal final : public DisplayObject {
public:
	Goal();
	~Goal();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	glm::vec2 getGridPosition() const;
	void setGridPosition(float col, float row);

private:
	void m_checkBounds();
	void m_reset();

	glm::vec2 m_gridPosition;
};


#endif 