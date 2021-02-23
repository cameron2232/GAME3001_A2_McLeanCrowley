#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	m_pBackground->draw();
	
	drawDisplayList();
	
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
<<<<<<< Updated upstream
=======
	if (m_shipIsMoving)
	{
		m_moveShip();
	}
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	if(m_getDebugMode())
	{
		for (auto tile : m_pGrid)
		{
			if (EventManager::Instance().getMouseButton(0) && CollisionManager::pointRectCheck(EventManager::Instance().getMousePosition(), tile->getTransform()->position + offset, tile->getWidth(), tile->getHeight()))
			{
				if (tile->getTileStatus() != GOAL && tile->getTileStatus() != IMPASSABLE)
				{
					std::cout << "Changing start tile to (" << tile->getGridPosition().x << ", " << tile->getGridPosition().y << ")\n";
					m_getTile(m_pShip->getGridPosition())->setTileStatus(DEFAULT);
					m_pShip->getTransform()->position = tile->getTransform()->position + offset;
					m_pShip->setGridPosition(tile->getGridPosition().x, tile->getGridPosition().y);
					tile->setTileStatus(START);
					m_pPathList.clear();
					m_pPathList.shrink_to_fit();
					m_pOpenList.clear();
					m_pOpenList.shrink_to_fit();
					m_pClosedList.clear();
					m_pClosedList.shrink_to_fit();
					for (auto tile : m_pGrid)
					{
						tile->setTileStatus(DEFAULT);
					}
					m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
					m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
					m_computeTileCosts();
					setBarriers();
					m_shipIsMoving = false;
					moveCounter = 0;
					for (auto i : m_pPathTile)
					{
						removeChild(i);
					}
					m_pPathTile.clear();
					m_pPathList.shrink_to_fit();

				}
				else
				{
					std::cout << "Cannot set Start Tile to Goal Tile!\n";
				}
			}
			if (EventManager::Instance().getMouseButton(2) && CollisionManager::pointRectCheck(EventManager::Instance().getMousePosition(), tile->getTransform()->position + offset, tile->getWidth(), tile->getHeight()))
			{
				if (tile->getTileStatus() != START && tile->getTileStatus() != IMPASSABLE)
				{
					std::cout << "Changing Goal tile to (" << tile->getGridPosition().x << ", " << tile->getGridPosition().y << ")\n";
					m_getTile(m_pTarget->getGridPosition())->setTileStatus(DEFAULT);
					m_pTarget->getTransform()->position = tile->getTransform()->position + offset;
					m_pTarget->setGridPosition(tile->getGridPosition().x, tile->getGridPosition().y);
					tile->setTileStatus(GOAL);
					m_computeTileCosts();
				}
				else
				{
					std::cout << "Cannot set Goal Tile to Start Tile!\n";
				}
			}
		}
	}
>>>>>>> Stashed changes
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
<<<<<<< Updated upstream
=======

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_F))
	{
		m_findAllViablePaths();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_M))
	{
		m_shipIsMoving = true;
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_G))
	{
		m_setGridEnabled(!m_getGridEnabled());
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		m_setDebugMode(!m_getDebugMode());
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_R))
	{
		auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
		m_pPathList.clear();
		m_pPathList.shrink_to_fit();
		m_pOpenList.clear();
		m_pOpenList.shrink_to_fit();
		m_pClosedList.clear();
		m_pClosedList.shrink_to_fit();
		m_pShip->getTransform()->position = m_getTile(3, 2)->getTransform()->position + offset;
		m_pShip->setGridPosition(3, 2);

		m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
		m_pTarget->setGridPosition(15, 11);
		for (auto tile : m_pGrid)
		{
			tile->setTileStatus(DEFAULT);
		}
		m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCosts();
		setBarriers();
		m_shipIsMoving = false;
		moveCounter = 0;
		for (auto i : m_pPathTile)
		{
			removeChild(i);
		}
		m_pPathTile.clear();
		m_pPathList.shrink_to_fit();
		m_pSaveTiles.clear();
		m_pSaveTiles.shrink_to_fit();
		m_pListOfPaths.clear();
		m_pListOfPaths.shrink_to_fit();
		m_PathIsCalculated = false;
	}

>>>>>>> Stashed changes
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_buildGrid();

	m_pBackground = new Background();
	//addChild(m_pBackground);
	
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(15, 11);
	addChild(m_pTarget);

	m_pGoal = new Goal();
	m_pGoal->getTransform()->position = m_getTile(4, 6)->getTransform()->position + offset;
	m_pGoal->setGridPosition(4, 6);
	addChild(m_pGoal);

	m_computeTileCosts();
	
}

void PlayScene::GUI_Function() 
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 4", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar/* | ImGuiWindowFlags_NoMove*/);

	static bool isGridEnabled = false;
	if(ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		// toggle grid on/off
		m_setGridEnabled(isGridEnabled);
		
	}

	ImGui::Separator();
	static int targetPosition[] = { m_pTarget->getGridPosition().x, m_pTarget->getGridPosition().y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, Config::COL_NUM-1))
	{
		if(targetPosition[1] > Config::ROW_NUM-1)
		{
			targetPosition[1] > Config::ROW_NUM - 1;
		}
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position + offset;
		m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	
	ImGui::Separator();
	
	if(ImGui::Button("Start"))
	{
<<<<<<< Updated upstream
		
=======
		m_findAllViablePaths();
>>>>>>> Stashed changes
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		
<<<<<<< Updated upstream
=======
		m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
		m_pTarget->setGridPosition(15, 11);
		for(auto tile : m_pGrid)
		{
			tile->setTileStatus(DEFAULT);
		}
		m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCosts();
		setBarriers();
		m_shipIsMoving = false;
		moveCounter = 0;
		//removeChild(m_pPathList);
		for(auto i : m_pPathTile)
		{
			removeChild(i);
		}
		m_pPathTile.clear();
		m_pPathList.shrink_to_fit();
		m_PathIsCalculated = false;
>>>>>>> Stashed changes
	}

	ImGui::Separator();

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	//add tiles to grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = new Tile(); // create empty tile
			tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
			tile->setGridPosition(col, row);
			addChild(tile);
			tile->addLabels();
			tile->setStatusLabel(col, row);
			//tile->setPassable(true);
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}
	setBarriers();
	// create references for each tile to every other tile
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			//top row
			if(row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				if (!m_getTile(col, row - 1)->getPassable())
				{
					tile->setNeighbourTile(TOP_TILE, nullptr);
				}
				else
				{
					tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
				}
				
			}
			//right collumn
			if(col == Config::COL_NUM-1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				if(!m_getTile(col + 1, row)->getPassable())
				{
					tile->setNeighbourTile(RIGHT_TILE, nullptr);
				}
				else
				{
					tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));
				}
			}

			//bottom row
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				if (!m_getTile(col, row + 1)->getPassable())
				{
					tile->setNeighbourTile(BOTTOM_TILE, nullptr);
				}
				else
				{
					tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));
				}
			}

			//left collumn
			if(col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
<<<<<<< Updated upstream
				if (!m_getTile(col - 1, row)->getPassable())
				{
					tile->setNeighbourTile(LEFT_TILE, nullptr);
				}
				else
				{
					tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
				}
			}

		}
	}
}

void PlayScene::m_computeTileCosts()
=======
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}

		}
	}
}

void PlayScene::m_computeTileCosts()
{
	float distance, dx, dy;

	for (auto tile : m_pGrid)
	{
		switch (currentHeuristic)
		{
		case MANHATTAN:
			//Manhattan distance
			dx = abs(tile->getGridPosition().x - m_pTarget->getGridPosition().x);
			dy = abs(tile->getGridPosition().y - m_pTarget->getGridPosition().y);
			distance = dx + dy;
			break;
		case EUCLIDEAN:
			//Euclidean Distance
			distance = Util::distance(m_pTarget->getGridPosition(), tile->getGridPosition());
			break;
		}

		//tile->setTileCost(m_pTarget->getGridPosition(), tile->getGridPosition());
		tile->setTileCost(distance);
	}
}

void PlayScene::m_findAllViablePaths()
{
	m_findShortestPath(m_getTile(m_pShip->getGridPosition()));
	m_pListOfPaths.push_back(m_pPathList);
	m_pPathList.clear();
	m_pPathList.shrink_to_fit();
	std::cout << m_pListOfPaths[0].size() << std::endl;
	for(int i = 0; i < m_pListOfPaths[0].size(); i++)
	{
		m_pSaveTiles.push_back(m_pListOfPaths[0][i]);		
		std::vector<Tile*> neighbourList;
		for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index) //for each neighbour
		{
			neighbourList.push_back(m_pSaveTiles[i]->getNeighbourTile(NeighbourTile(index))); //get neighbour tiles of current tile		
		}
		for(auto neighbour : neighbourList)
		{
			if (neighbour->getTileStatus() == CLOSED)
			{
				if (m_pListOfPaths[0][i+1]->getTileCost() >= neighbour->getTileCost())
				{
					m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
					m_pPathList.clear();
					m_pPathList.shrink_to_fit();
					m_PathIsCalculated = false;
					m_findShortestPath(neighbour);
					m_pListOfPaths.push_back(m_pSaveTiles);
					for(int j = 0; j < m_pPathList.size(); j++)
					{
						m_pListOfPaths[m_pListOfPaths.size() - 1].push_back(m_pPathList[j]);
					}
				}
			}
		}
	}
	m_pPathList.clear();
	m_pPathList.shrink_to_fit();
	for(int i = 0; i < m_pListOfPaths.size(); i++)
	{
		for(int j = 0; j < m_pListOfPaths[i].size(); j++)
		{
			m_pSaveTiles.push_back(m_pListOfPaths[i][j]);
			std::vector<Tile*> neighbourList;
			for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index) //for each neighbour
			{
				neighbourList.push_back(m_pSaveTiles[j]->getNeighbourTile(NeighbourTile(index))); //get neighbour tiles of current tile		
			}
			for(auto neighbour : neighbourList)
			{
				if(neighbour->getTileStatus() == CLOSED)
				{
					if(m_pListOfPaths[i][j]->getTileCost() >= neighbour->getTileCost())
					{
						m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
						m_pPathList.clear();
						m_pPathList.shrink_to_fit();
						m_PathIsCalculated = false;
						m_findShortestPath(neighbour);
						m_pListOfPaths.push_back(m_pSaveTiles);
						for(int h = 0; h < m_pPathList.size(); h++)
						{
							m_pListOfPaths[m_pListOfPaths.size() - 1].push_back(m_pPathList[h]);
						}
					}
				}
			}
		}
	}
	m_pPathList = m_pListOfPaths[0];
	for(int i = 1; i < m_pListOfPaths.size(); i++)
	{
		if(m_pPathList.size() >= m_pListOfPaths[i].size())
		{
			m_pPathList = m_pListOfPaths[i];
		}				
	}
	m_displayPathList();
}


void PlayScene::m_findShortestPath(Tile* startingTile)
{
	if (!m_PathIsCalculated)
	{
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_pOpenList.clear();
		m_pOpenList.shrink_to_fit();
		// Step 1 - Add Start position to the open list
		auto startTile = startingTile;
		startTile->setTileStatus(OPEN); //set starting tile to open - this is where we start.
		m_pOpenList.push_back(startTile); //add starting tile to open list.
		bool goalFound = false;
		// Step 2 - Loop until the OpenList is empty or the Goal is found
		while (!m_pOpenList.empty() && !goalFound)
		{
			auto min = INFINITY; 
			Tile* minTile;
			int minTileIndex = 0;
			int count = 0;
			std::vector<Tile*> neighbourList;
			for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index) //for each neighbour
			{				
				neighbourList.push_back(m_pOpenList[0]->getNeighbourTile(NeighbourTile(index))); //get neighbour tiles of current tile				
			}
			for (auto neighbour : neighbourList) //For loop through neighbours
			{				
				if (neighbour!= nullptr)
				{				
						
					if (neighbour->getTileStatus() != IMPASSABLE && neighbour->getTileStatus() != GOAL)
					{
						if (neighbour->getTileCost() < min) //if neighbour tile cost is less than current tile cost
						{
							min = neighbour->getTileCost(); //set min to neighbour tile cost
							minTile = neighbour; //set min to neighbour
							if (minTile->getTileStatus() != OPEN)
								minTile->setTileStatus(CLOSED);
							minTileIndex = count; //set minTileIndex to count, counting shortest path.
						}
					}
						count++;
						 
						if(neighbour->getTileStatus() == GOAL) //if neighbour IS goal
								{
							m_pPathList.push_back(m_pOpenList[0]);
							m_pPathList.push_back(neighbour);
							goalFound = true; //goal is found! time to break out of the search algo.
							m_PathIsCalculated = true;
							break;
								}					
				}
			} //end of for loop
			if (!goalFound)
			{
				m_pPathList.push_back(m_pOpenList[0]);
			}//add current tile
			m_pOpenList.pop_back(); // empties the open list so minTile will be [0]
			m_pOpenList.push_back(minTile); //add minTile to open list
			minTile->setTileStatus(OPEN); //set minTile to OPEN
			neighbourList.erase(neighbourList.begin() + minTileIndex); //erase list of neighbours			
		} // end of while loop, path calculated
		
	}


}



void PlayScene::m_displayPathList()
>>>>>>> Stashed changes
{
	for (auto tile : m_pGrid)
	{
		//auto distance = Util::distance(m_pTarget->getGridPosition(), tile->getGridPosition());
		tile->setTileCost(m_pTarget->getGridPosition(), tile->getGridPosition());
	}
}

void PlayScene::m_setGridEnabled(bool state) 
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}

	if(state == false)
	{
		SDL_RenderClear(Renderer::Instance()->getRenderer());
	}
}

Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::m_setWall(Tile* tile, const bool passable)
{
<<<<<<< Updated upstream
	tile->setPassable(passable);
=======
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::m_moveShip()
{
	//std::cout << "Moving Ship...\n";
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	if (moveCounter < m_pPathList.size())
	{
	
		if (m_pShip->getGridPosition().x < m_pPathList[moveCounter]->getGridPosition().x && m_pShip->getGridPosition().y == m_pPathList[moveCounter]->getGridPosition().y) //moving right
		{			
		m_pShip->setCurrentHeading(0);
		m_pShip->setFlip(0);
		}
		else if(m_pShip->getGridPosition().x > m_pPathList[moveCounter]->getGridPosition().x && m_pShip->getGridPosition().y == m_pPathList[moveCounter]->getGridPosition().y) //moving left
		{
			m_pShip->setCurrentHeading(0);
			m_pShip->setFlip(1);
		}
		else if (m_pShip->getGridPosition().y < m_pPathList[moveCounter]->getGridPosition().y && m_pShip->getGridPosition().x == m_pPathList[moveCounter]->getGridPosition().x) //moving down
		{
			m_pShip->setCurrentHeading(90);
		}
		else if (m_pShip->getGridPosition().y > m_pPathList[moveCounter]->getGridPosition().y && m_pShip->getGridPosition().x == m_pPathList[moveCounter]->getGridPosition().x) //moving down
		{
			m_pShip->setCurrentHeading(-90);
		}
		
		
		m_pShip->setTargetPosition(m_getTile(m_pPathList[moveCounter]->getGridPosition())->getTransform()->position + offset);
	
			
		
		
		m_pShip->getTransform()->position = m_getTile(m_pPathList[moveCounter]->getGridPosition())->getTransform()->position + offset;
		m_pShip->setGridPosition(m_pPathList[moveCounter]->getGridPosition().x, m_pPathList[moveCounter]->getGridPosition().y);
		
		
		if (Game::Instance()->getFrames() % 40 == 0)
		{
			SoundManager::Instance().playSound("Splash", 1, -1);
			moveCounter++;
		}
		
	}
	else
	{
		m_shipIsMoving = false;
	}
>>>>>>> Stashed changes
}

void PlayScene::setBarriers()
{
	//Boat
	for(int i = 0; i < 5; i++)
	{
		m_setWall(m_getTile(2, i), false);
		m_setWall(m_getTile(4, i), false);
	}
	m_setWall(m_getTile(3, 0), false);
	m_setWall(m_getTile(3, 1), false);

	//Log
	for(int i = 6; i < 11; i ++)
	{
		m_setWall(m_getTile(16, i), false);
	}

	//Liferafts
	m_setWall(m_getTile(6, 6), false);
	m_setWall(m_getTile(13, 9), false);

	//Whirlpool
	for(int i = 2; i < 5; i++)
	{
		for(int j = 9; j < 12; j++)
		{
			m_setWall(m_getTile(i, j), false);
		}
	}

	//Tanks
	m_setWall(m_getTile(8, 4), false);
	m_setWall(m_getTile(8, 9), false);

	//Sharks
	for(int i = 6; i < 9; i ++)
	{
		m_setWall(m_getTile(i, 12), false);
	}
	m_setWall(m_getTile(14, 3), false);
	m_setWall(m_getTile(15, 3), false);
	m_setWall(m_getTile(15, 4), false);
	m_setWall(m_getTile(16, 4), false);

	//Eel
	for(int i = 10; i < 13; i++)
	{
		m_setWall(m_getTile(i, 6), false);
	}
	m_setWall(m_getTile(10, 7), false);
	m_setWall(m_getTile(11, 7), false);
}