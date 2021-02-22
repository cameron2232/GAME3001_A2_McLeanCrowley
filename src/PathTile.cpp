#include "PathTile.h"

#include "Util.h"

PathTile::PathTile()
{
	setWidth(40);
	setHeight(40);
}

PathTile::~PathTile()
{
	
}

void PathTile::draw()
{
	Util::DrawRect(getTransform()->position, getWidth(), getHeight(), {1, 0, 0, 1});	
}

void PathTile::update()
{
	
}

void PathTile::clean()
{
	
}
