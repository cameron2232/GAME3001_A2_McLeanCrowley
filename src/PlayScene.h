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

	Target* m_pTarget;
	Goal* m_pGoal;
	Background* m_pBackground;
	// Pathfinding functions and objects
	void m_buildGrid();
	void m_computeTileCosts();
<<<<<<< Updated upstream
=======
	void m_findAllViablePaths();
	void m_findShortestPath(Tile* startingTile);
	void m_displayPathList();

>>>>>>> Stashed changes

	
	void m_setGridEnabled(bool state);
	std::vector<Tile*> m_pGrid;

	// convenience functions
	Tile* m_getTile(int col, int row);
<<<<<<< Updated upstream
	void m_setWall(Tile* tile, bool passable);
	void setBarriers();
	
=======
	Tile* m_getTile(glm::vec2 grid_position);


	//heuristic
	Heuristic currentHeuristic;

	//Open, closed, and path lists
	std::vector<Tile*> m_pOpenList;
	std::vector<Tile*> m_pClosedList;
	std::vector<Tile*> m_pPathList;
	std::vector<Tile*> m_pSaveTiles;

	std::vector<std::vector <Tile*>> m_pListOfPaths;

	std::vector<PathTile*> m_pPathTile;

	Label* m_pInstructions{};
	Label* m_pInstructionsTwo{};
	
	void m_moveShip();
	int moveCounter = 0;
	bool m_shipIsMoving = false;
	bool m_GridEnabled = false;
	bool m_PathIsCalculated = false;
>>>>>>> Stashed changes
};


#endif /* defined (__PLAY_SCENE__) */
