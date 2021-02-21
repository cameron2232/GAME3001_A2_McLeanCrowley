#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "SpaceShip.h"
#include "Target.h"
#include "Tile.h"
#include "Goal.h"
#include "ship.h"
#include "Heuristic.h"
#include "Background.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	Target* m_pTarget;
	Goal* m_pGoal;
	Ship* m_pShip;

	Background* m_pBackground;

	//Barriers
	void setBarriers();
	// Pathfinding functions and objects
	void m_buildGrid();
	void m_computeTileCosts();
	void m_findShortestPath();
	void m_displayPathList();
	
	void m_setGridEnabled(bool state);
	bool m_getGridEnabled() const;
	std::vector<Tile*> m_pGrid;


	void m_setDebugMode(bool state);
	bool m_getDebugMode() const;
	// convenience functions
	Tile* m_getTile(int col, int row);
	Tile* m_getTile(glm::vec2 grid_position);


	//heuristic
	Heuristic currentHeuristic;

	//Open, closed, and path lists
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;
	std::vector<Tile*> m_pUnvisitedList;
	std::vector<Tile*> m_ptempPathList;
	std::vector<Tile> m_pNewPathList;
	
	void m_moveShip();
	int moveCounter = 0;
	bool m_shipIsMoving = false;
	bool m_GridEnabled = false;
};

#endif /* defined (__PLAY_SCENE__) */