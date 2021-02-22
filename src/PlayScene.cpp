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
	if (!m_getDebugMode())
	{
		m_setGridEnabled(false);
	}
	m_pBackground->draw();
	
	drawDisplayList();
	
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	if(m_getDebugMode())
	{
		m_setGridEnabled(true);
	}

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
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
					std::cout << "Changing start tile...\n";
					m_getTile(m_pShip->getGridPosition())->setTileStatus(DEFAULT);
					m_pShip->getTransform()->position = tile->getTransform()->position + offset;
					m_pShip->setGridPosition(tile->getGridPosition().x, tile->getGridPosition().y);
					tile->setTileStatus(START);
					m_computeTileCosts();
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_F))
	{
		m_findShortestPath();
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
		m_pUnvisitedList.clear();
		m_pUnvisitedList.shrink_to_fit();
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
	}

}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_buildGrid();

	m_pBackground = new Background();
	//addChild(m_pBackground);
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	currentHeuristic = EUCLIDEAN;

	//add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = m_getTile(3, 2)->getTransform()->position + offset;
	m_pShip->setGridPosition(3, 2);
	m_getTile(3, 2)->setTileStatus(START);
	addChild(m_pShip);

	//added the target as a goal
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = m_getTile(15, 11)->getTransform()->position + offset;
	m_pTarget->setGridPosition(15, 11);
	m_getTile(15, 11)->setTileStatus(GOAL);
	addChild(m_pTarget);


	setBarriers();
	m_computeTileCosts();
	
}

void PlayScene::GUI_Function() 
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 5", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool isGridEnabled = false;
	if(ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		// toggle grid on/off
		m_setGridEnabled(isGridEnabled);
		
	}

	ImGui::Separator();

	auto radio = static_cast<int>(currentHeuristic);
	ImGui::LabelText("", "Heuristic Type");
	/*ImGui::RadioButton("Manhattan", &radio, static_cast<int>(MANHATTAN));*/
	ImGui::SameLine();
	ImGui::RadioButton("Euclidean", &radio, static_cast<int>(EUCLIDEAN));
	if (currentHeuristic != Heuristic(radio))
	{
		currentHeuristic = Heuristic(radio);
		m_computeTileCosts();
	}

	ImGui::Separator();
	static int startPosition[] = { m_pShip->getGridPosition().x, m_pShip->getGridPosition().y };
	if (ImGui::SliderInt2("Start Position", startPosition, 0, Config::COL_NUM - 1))
	{
		if (startPosition[1] > Config::ROW_NUM - 1)
		{
			startPosition[1] > Config::ROW_NUM - 1;
		}
		m_getTile(m_pShip->getGridPosition())->setTileStatus(DEFAULT);
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pShip->getTransform()->position = m_getTile(startPosition[0], startPosition[1])->getTransform()->position + offset;
		m_pShip->setGridPosition(startPosition[0], startPosition[1]);
		m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}

	ImGui::Separator();
	static int targetPosition[] = { m_pTarget->getGridPosition().x, m_pTarget->getGridPosition().y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, Config::COL_NUM-1))
	{
		if(targetPosition[1] > Config::ROW_NUM-1)
		{
			targetPosition[1] = Config::ROW_NUM - 1;
		}
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(DEFAULT);
		SDL_RenderClear(Renderer::Instance()->getRenderer());
		m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position + offset;
		m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
		m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
		m_computeTileCosts();
		SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
		SDL_RenderPresent(Renderer::Instance()->getRenderer());
	}
	
	ImGui::Separator();
	
	if(ImGui::Button("Start"))
	{
		m_findShortestPath();
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		m_pPathList.clear();
		m_pPathList.shrink_to_fit();
		m_pOpenList.clear();
		m_pOpenList.shrink_to_fit();
		m_pClosedList.clear();
		m_pClosedList.shrink_to_fit();
		m_pUnvisitedList.clear();
		m_pUnvisitedList.shrink_to_fit();
		m_pShip->getTransform()->position = m_getTile(3, 2)->getTransform()->position + offset;
		m_pShip->setGridPosition(3, 2);
		
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
			//tile->setStatusLabel(col, row);
			tile->setEnabled(false);
			m_pGrid.push_back(tile);
		}
	}

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
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}

			//right collumn
			if(col == Config::COL_NUM-1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));
			}

			//bottom row
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));
			}

			//left collumn
			if(col == 0)
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

void PlayScene::m_findShortestPath()
{
	if (m_pPathList.empty())
	{
		// Step 1 - Add Start position to the open list
		auto startTile = m_getTile(m_pShip->getGridPosition());
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
					if (neighbour->getTileStatus() == IMPASSABLE || neighbour->getTileStatus() == CLOSED) //if neighbour impassable, close
					{
						neighbour->setTileStatus(CLOSED);
						count++;
					}
					else
					{
						if (neighbour->getTileStatus() != GOAL && neighbour->getTileStatus() != CLOSED && neighbour->getTileStatus() != OPEN) //if neighbour is not goal, closed, or impassable
						{
							if (neighbour->getTileCost() < min) //if neighbour tile cost is less than current tile cost
							{
								
								min = neighbour->getTileCost(); //set min to neighbour tile cost
								minTile = neighbour; //set min to neighbour
								minTile->getGridPosition();
								minTileIndex = count; //set minTileIndex to count, counting shortest path.
							}
							count++;
						}
						else if(neighbour->getTileStatus() == GOAL) //if neighbour IS goal
						{
							std::cout << "goal found\n";
							minTile = neighbour; //set minTile to neighbour
							m_pPathList.push_back(m_pOpenList[0]);
							m_pPathList.push_back(minTile); //add neighbour to goal
							goalFound = true; //goal is found! time to break out of the search algo.
							break;
						}
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

			//push all remaining neighbours onto the closed list
			
			for (auto neighbour : neighbourList)
			{
				if (neighbour != nullptr)
				{
					if (neighbour->getTileStatus() == UNVISITED || neighbour->getTileStatus() == DEFAULT) //if neighbour is unvisited or default (was not checked)
					{
						neighbour->setTileStatus(CLOSED); //set to closed
						
					}
				}
			}
		} // end of while loop, path calculated
		m_pOpenList.clear();
		m_pOpenList.shrink_to_fit();
		//setting unvisited tiles
		setBarriers();
		for (int i = 0; i < m_pPathList.size(); i++) //go through each tile in list 
		{		
			auto currTile = m_pPathList[i]; //set tile, starting at 0 (start tile)
			for (int tempindex = 0; tempindex < NUM_OF_NEIGHBOUR_TILES; ++tempindex) //for each neighbour
			{
				auto neighbour = currTile->getNeighbourTile(NeighbourTile(tempindex));
				if(neighbour == nullptr)
				{
					continue;
				}
				if (neighbour->getTileStatus() == CLOSED && neighbour->getTileCost() <= currTile->getTileCost()
					&& neighbour->getTileStatus() != UNVISITED) //if neighbour is closed and not already set to unvisited 
															    // (some tiles are a good option for one tile, but not another, but neighbour both tiles)
				{
					neighbour->setTileStatus(UNVISITED);
					//m_pUnvisitedList.push_back(neighbour);
				}
				else 
				{
					//m_pClosedList.push_back(neighbour);
				}				
			}		
		}
		for (int i = 0; i < m_pPathList.size(); i++) //go through each tile in list 
		{
			auto currTile = m_pPathList[i]; //set tile, starting at 0 (start tile)
			for (int tempindex = 0; tempindex < NUM_OF_NEIGHBOUR_TILES; ++tempindex) //for each neighbour
			{
				auto neighbour = currTile->getNeighbourTile(NeighbourTile(tempindex));
				if (neighbour != nullptr)
				{
					if (neighbour->getTileStatus() == CLOSED)
					{
						neighbour->setTileStatus(DEFAULT);
					}
				}
			}
		}
		//calculate unvisited paths
		m_pNewPathList = m_pPathList;
		
		m_findAllPaths(m_pPathList);
		
		m_pNewPathList = m_pPathList;
		
		/*m_ptestList = m_ptempPathList;
		m_findAllPaths(m_ptestList);
		m_findUnvisiteds(m_ptestList);
							
		
		m_findUnvisiteds(m_ptestList);
											*/
		
		//m_findUnvisiteds(m_ptempPathList, );
		m_displayPathList();
	} 
}


void PlayScene::m_findUnvisiteds(std::vector<Tile*> tilevector)
{
	for (int i = 0; i < tilevector.size(); i++)
	{
		auto currentTile = tilevector[i]; //set tile, starting at 0 (start tile)
		for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index) //for each neighbour
		{
			auto neighbour = currentTile->getNeighbourTile(NeighbourTile(index));
			if (neighbour == nullptr)
			{
				continue;
			}
			if (neighbour->getTileStatus() != OPEN && neighbour->getTileCost() < currentTile->getTileCost() && neighbour->getTileStatus() != IMPASSABLE) //if neighbour is closed and not already set to unvisited 															                                                 // (some tiles are a good option for one tile, but not another, but neighbour both tiles)
			{



				
				neighbour->setTileStatus(UNVISITED);
			}
		}
	}
}

void PlayScene::m_findAllPaths(std::vector<Tile*> checkedvector)
{
	
		int unvisited = 0;
		bool newPathFound = false;
		for (auto i = 0; i < checkedvector.size(); i++)
		{
			if (checkedvector[i]->getGridPosition() != m_pTarget->getGridPosition())
			{
				auto currentTile = checkedvector[i];
				for (int index = 0; index < NUM_OF_NEIGHBOUR_TILES; ++index) //for each neighbour
				{
					auto neighbour = currentTile->getNeighbourTile(NeighbourTile(index));
					if (neighbour != nullptr)
					{
						if (neighbour->getTileStatus() == UNVISITED)
						{
							m_ptempPathList.clear();
							m_ptempPathList.shrink_to_fit();
							for (int j = 0; j < i; j++)
							{
								m_ptempPathList.push_back(checkedvector[j]);
							}
							m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
							m_findNewPath(neighbour);
							if (m_ptempPathList.size() < m_pNewPathList.size())
							{
								std::cout << "New path set!\n";
								m_pNewPathList = m_ptempPathList;
								m_findUnvisiteds(m_pNewPathList);
							
								newPathFound = true;								
							}
						}
					}
			}
		}
	}
	if(newPathFound)
	{
		m_pPathList = m_pNewPathList;
	}
}


void PlayScene::m_findNewPath(Tile* startingpoint)
{
	auto startTile = startingpoint;
	startTile->setTileStatus(OPEN);
	m_pOpenList.push_back(startTile); //add starting point tile to open list.
	startTile->setColour(255, 0, 0, 255);

	bool goalFound = false;

	while (!m_pOpenList.empty() && !goalFound) //loop until goal found
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
			if (neighbour != nullptr)
			{
				if (neighbour->getTileStatus() == IMPASSABLE || neighbour->getTileStatus() == CLOSED)
				{
					neighbour->setTileStatus(CLOSED);
					count++;
				}
				else
				{
					if(neighbour->getTileStatus() != GOAL && neighbour->getTileStatus() != CLOSED)
					{
						if(neighbour->getTileCost() < min)
						{
							min = neighbour->getTileCost(); //set min to neighbour tile cost
							minTile = neighbour; //set min to neighbour
							minTile->setColour(255, 0, 0, 255);
							minTileIndex = count; //set minTileIndex to count, counting shortest path.
						}
						count++;
					}
					else if(neighbour->getTileStatus() == GOAL)
					{
						std::cout << "goal found\n";
						minTile = neighbour; //set minTile to neighbour
						m_ptempPathList.push_back(m_pOpenList[0]);
						m_ptempPathList.push_back(minTile); //add neighbour to goal
						goalFound = true; //goal is found! time to break out of the search algo.
						break;
					}
				}
				
			}
		}

		if(!goalFound)
			m_ptempPathList.push_back(m_pOpenList[0]); //add current tile
		m_pOpenList.pop_back(); // empties the open list so minTile will be [0]
		m_pOpenList.push_back(minTile); //add minTile to open list
		minTile->setTileStatus(OPEN); //set minTile to OPEN
		neighbourList.erase(neighbourList.begin() + minTileIndex); //erase list of neighbours

	} //end of while loop
	m_pOpenList.clear();
	m_pOpenList.shrink_to_fit();

	setBarriers();
}

  

void PlayScene::m_displayPathList()
{
	for (auto node : m_pPathList)
	{
		std::cout << "(" << node->getGridPosition().x << ", " << node->getGridPosition().y << ")" << std::endl;
		
	}
	std::cout << "Path Length: " << m_pPathList.size() << std::endl;
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
	m_isGridEnabled = state;
}

bool PlayScene::m_getGridEnabled() const
{
	return m_isGridEnabled;
}

void PlayScene::m_setDebugMode(bool state)
{
	m_GridEnabled = state;
}

bool PlayScene::m_getDebugMode() const
{
	return m_GridEnabled;
}


Tile* PlayScene::m_getTile(const int col, const int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(glm::vec2 grid_position)
{
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
		std::cout << "(" << m_pShip->getGridPosition().x << ", " << m_pShip->getGridPosition().y << ")" << std::endl;
		m_pShip->getTransform()->position = m_getTile(m_pPathList[moveCounter]->getGridPosition())->getTransform()->position + offset;
		m_pShip->setGridPosition(m_pPathList[moveCounter]->getGridPosition().x, m_pPathList[moveCounter]->getGridPosition().y);
		if (Game::Instance()->getFrames() % 20 == 0)
		{
			moveCounter++;
		}
	}
	else
	{
		m_shipIsMoving = false;
	}
}

void PlayScene::setBarriers()
{
	for(int i = 0; i < 20; i++)
	{
		m_getTile(i, 0)->setTileStatus(IMPASSABLE);
	}
	m_getTile(0, 1)->setTileStatus(IMPASSABLE);
	//boat
	m_getTile(2, 0)->setTileStatus(IMPASSABLE);
	m_getTile(2, 1)->setTileStatus(IMPASSABLE);
	m_getTile(4, 0)->setTileStatus(IMPASSABLE);
	m_getTile(4, 1)->setTileStatus(IMPASSABLE);
	m_getTile(3, 0)->setTileStatus(IMPASSABLE);
	m_getTile(3, 1)->setTileStatus(IMPASSABLE);

	//Log
	for(int i = 6; i < 11; i++)
	{
		m_getTile(16, i)->setTileStatus(IMPASSABLE);
	}

	//Liferafts
	m_getTile(6, 6)->setTileStatus(IMPASSABLE);
	m_getTile(13, 9)->setTileStatus(IMPASSABLE);

	//Whirlpool
	for(int i = 2; i < 5; i++)
	{
		for(int j = 9; j < 12; j++)
		{
			m_getTile(i, j)->setTileStatus(IMPASSABLE);
		}
	}

	//Tanks
	m_getTile(8, 5)->setTileStatus(IMPASSABLE);
	m_getTile(8, 9)->setTileStatus(IMPASSABLE);

	//Sharks
	for(int i = 6; i < 9; i++)
	{
		m_getTile(i, 12)->setTileStatus(IMPASSABLE);
	}
	m_getTile(14, 3)->setTileStatus(IMPASSABLE);
	m_getTile(15, 3)->setTileStatus(IMPASSABLE);
	m_getTile(15, 4)->setTileStatus(IMPASSABLE);
	m_getTile(16, 4)->setTileStatus(IMPASSABLE);

	//Eel
	for(int i = 10; i < 13; i++)
	{
		m_getTile(i, 6)->setTileStatus(IMPASSABLE);
	}
	m_getTile(10, 7)->setTileStatus(IMPASSABLE);
	m_getTile(11, 7)->setTileStatus(IMPASSABLE);
}
