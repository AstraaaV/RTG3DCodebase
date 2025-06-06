#include "Map.h"
#include "glm/glm.hpp"

Map::Map()
{
	m_layout =
	{
		"WWWWWWWW",
		"W......W",
		"W.WW.W.W",
		"W......W",
		"WWWWWWWW"
	};
}

Map::~Map()
{
	for (auto obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear();
}

void Map::Init()
{
	float tileSize = 1.0f;

	for (int z = 0; z < m_layout.size(); ++z)
	{
		for (int x = 0; x < m_layout[z].length(); ++x)
		{
			char tile = m_layout[z][x];
			glm::vec3 position = glm::vec3(x * tileSize, 0.5f, z * tileSize);

			if(tile == 'W')
			{
				GameObject* wall = new GameObject("WALL", "CUBE", "WALL_DIFFUSE", "TEXDIR");
				wall->SetPosition(position);
				m_objects.push_back(wall);
				break;
			}
		}
	}
}

const std::vector<GameObject*>& Map::GetObjects() const
{
	return m_objects;
}
