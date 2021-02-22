#pragma once
#ifndef __PATHTILE__
#define __PATHTILE__
#include "CollisionManager.h"

class PathTile : public DisplayObject
{
public:
	// Constructor
	PathTile();

	// Destructor
	~PathTile();

	void draw() override;
	void update() override;
	void clean() override;










private:
	
};



#endif /* defined (__TILE__) */