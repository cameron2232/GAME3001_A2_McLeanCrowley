#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "DisplayObject.h"

class Background final : public DisplayObject
{
public:
	Background();
	~Background();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
};


#endif 