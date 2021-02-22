#pragma once
#ifndef __TILE__
#define __TILE__
#include "Label.h"
#include "NeighbourTile.h"
#include "NavigationObject.h"
#include "TileStatus.h"
#include "CollisionManager.h"

class Tile : public NavigationObject
{
public:
	// Constructor
	Tile();
	
	// Destructor
	~Tile();
	
	// Life-Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;

	
	

	//getters
	float getTileCost() const;
	Tile* getNeighbourTile(NeighbourTile position);
	TileStatus getTileStatus() const;
	

	//setters
	void setColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	//void setTileCost(glm::vec2 target, glm::vec2 tile);
	void setTileCost(float cost);
	void setNeighbourTile(NeighbourTile position, Tile* tile);
	void setTileStatus(TileStatus status);

	void addLabels();
	void setLabelsEnabled(bool state);
	bool isPressed();
	

private:
	float m_cost;
	TileStatus m_status;

	Label* m_costLabel;
	Label* m_statusLabel;
	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];
	SDL_Color m_tileColour = { 255, 255, 255 ,255 };
};



#endif /* defined (__TILE__) */
