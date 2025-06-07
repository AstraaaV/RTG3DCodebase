#include "Map.h"
#include "GameObject.h"
#include "Scene.h"

Map::Map(Scene* scene) : m_scene(scene) {}

Map::~Map() {}

void Map::Init()
{
	LoadMap();

	for (int z = 0; z < m_map.size(); ++z)
	{
		for (int x = 0; x < m_map[z].length(); ++x)
		{
			SpawnTile(m_map[z][x], x, z);
		}
	}
}

void Map::LoadMap()
{
	m_map =
	{
		"WWWWWWWWWW",
		"W..T..B..W",
		"W........W",
		"W....P...W",
		"WWWWWWWWWW"
	};
}

const std::vector<std::string>& Map::GetMap() const
{
	return m_map;
}

void Map::SpawnTile(char tile, int x, int z)
{
	glm::vec3 pos = glm::vec3((float)x, 0.0f, (float)z);

	switch (tile)
	{
	case 'W':
		m_scene->AddWall(pos);
		break;
	case 'T':
		m_scene->AddTorch(pos);
		break;
	case 'B':
		m_scene->AddBeast(pos);
		break;
	case 'P':
		m_scene->SetPlayerSpawn(pos);
		break;
	default:
		break;
	}
}
