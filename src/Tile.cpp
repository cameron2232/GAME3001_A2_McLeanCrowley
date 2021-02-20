#include "Tile.h"

#include <iomanip>
#include <sstream>


#include "Config.h"
#include "TextureManager.h"
#include "Util.h"

Tile::Tile() : m_cost(0.0f)
{
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);


}

Tile::~Tile()
= default;

void Tile::draw()
{
	Util::DrawRect(getTransform()->position, getWidth(), getHeight());
}

void Tile::update()
{
}

void Tile::clean()
{
}

Tile* Tile::getNeighbourTile(const NeighbourTile position)
{
	return m_neighbours[position];
}

TileStatus Tile::getTileStatus() const
{
	return m_status;
}

void Tile::setNeighbourTile(const NeighbourTile position, Tile* tile)
{
	m_neighbours[position] = tile;
}

void Tile::setTileStatus(TileStatus status)
{
	m_status = status;

	switch (status)
	{
	case UNVISITED:
		m_statusLabel->setText("U");
		break;
	case OPEN:
		m_statusLabel->setText("O");
		break;
	case CLOSED:
		m_statusLabel->setText("C");
		break;
	case IMPASSABLE:
		m_statusLabel->setText("I");
		break;
	case GOAL:
		m_statusLabel->setText("G");
		break;
	case START:
		m_statusLabel->setText("S");
		break;
	case DEFAULT:
		m_statusLabel->setText("--");
		break;
	}
}

float Tile::getTileCost() const
{
	return m_cost;
}

//void Tile::setTileCost(glm::vec2 target, glm::vec2 tile)
//{
//	auto distanceX = abs(tile.x - target.x);
//	auto distanceY = abs(tile.y - target.y);
//	m_cost = distanceX + distanceY;
//	//format string to 1 decimal place
//	std::stringstream stream;
//	stream << std::fixed << std::setprecision(1) << m_cost;
//	const std::string cost_string = stream.str();
//	
//	m_costLabel->setText(cost_string);
//}

void Tile::setTileCost(float cost)
{
	m_cost = cost;

	//format string to 1 decimal place
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << cost;
	const std::string cost_string = stream.str();

	m_costLabel->setText(cost_string);
}

void Tile::addLabels()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	m_costLabel = new Label("99.9", "Consolas", 12, { 255,255,255,255 });
	m_costLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, -6.0f);
	getParent()->addChild(m_costLabel);
	m_costLabel->setEnabled(false);


	m_statusLabel = new Label("--", "Consolas", 12, { 255,255,255,255 });
	m_statusLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, 6.0f);
	getParent()->addChild(m_statusLabel);
	m_statusLabel->setEnabled(false);
}

void Tile::setStatusLabel(int col, int row)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << col << "," << row;
	const std::string pos_string = stream.str();
	m_statusLabel->setText(pos_string);
}

void Tile::setLabelsEnabled(const bool state)
{
	m_costLabel->setEnabled(state);
	m_statusLabel->setEnabled(state);
}