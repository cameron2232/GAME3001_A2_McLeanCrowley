#pragma once
#ifndef __NAVIGATION_OBJECT__
#define __NAVIGATION_OBJECT__

#include "DisplayObject.h"

class NavigationObject : public DisplayObject
{
public:
	//Constuctor
	NavigationObject();

	//Destructor
	~NavigationObject();

	//Inherited from displayobject
	virtual void draw() override = 0;
	virtual void update() override = 0;
	virtual void clean() override = 0;

	//getters
	glm::vec2 getGridPosition() const;

	//setters
	void setGridPosition(float col, float row);

private:
	glm::vec2 m_gridPosition;
};

#endif /* (Defined __NAVIGATION_OBJECT__)*/