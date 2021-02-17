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
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_buildGrid();

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
	
	ImGui::Begin("GAME3001 - Lab 4", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

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
		
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		
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
	tile->setPassable(passable);
}

void PlayScene::setBarriers()
{
	for (int i = 0; i < 8; i++)
	{
		m_setWall(m_getTile(0, i), false);
	}
	for (int i = 0; i < 5; i++)
	{
		m_setWall(m_getTile(i, 0), false);
	}
	
	m_setWall(m_getTile(5, 2), false);
	m_setWall(m_getTile(5, 3), false);
	m_setWall(m_getTile(6, 2), false);
	m_setWall(m_getTile(6, 3), false);
	
	m_setWall(m_getTile(5, 5), false);
	m_setWall(m_getTile(5, 7), false);
	m_setWall(m_getTile(6, 6), false);
	m_setWall(m_getTile(7, 5), false);
	m_setWall(m_getTile(7, 7), false);

	for(int i = 10; i < 16; i++)
	{
		m_setWall(m_getTile(i, 2), false);
		m_setWall(m_getTile(i, 5), false);
	}
	m_setWall(m_getTile(10, 3), false);
	m_setWall(m_getTile(15, 3), false);
	m_setWall(m_getTile(15, 4), false);

	for(int i = 11; i < 14; i++)
	{
		m_setWall(m_getTile(i, 7), false);
	}

	for(int i = 7; i < 11; i++)
	{
		m_setWall(m_getTile(i, 12), false);
		m_setWall(m_getTile(i, 13), false);
	}

	for(int i = 8; i < 12; i++)
	{
		m_setWall(m_getTile(18, i), false);
	}

	m_setWall(m_getTile(12, 12), false);
	m_setWall(m_getTile(12, 13), false);
	m_setWall(m_getTile(13, 12), false);
	m_setWall(m_getTile(15, 12), false);
	m_setWall(m_getTile(14, 13), false);
	m_setWall(m_getTile(15, 13), false);

	m_setWall(m_getTile(18, 1), false);
	m_setWall(m_getTile(18, 4), false);
	m_setWall(m_getTile(19, 2), false);
	m_setWall(m_getTile(19, 3), false);

	m_setWall(m_getTile(4, 10), false);
	m_setWall(m_getTile(4, 11), false);
	m_setWall(m_getTile(3, 11), false);
	m_setWall(m_getTile(2, 12), false);
	m_setWall(m_getTile(3, 12), false);
}