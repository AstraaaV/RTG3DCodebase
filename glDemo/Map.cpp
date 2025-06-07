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

	CreateObject("Floor_" + std::to_string(x) + "_" + std::to_string(z),
		"CUBE", "FLOOR_DIFFUSE", "TEXDIR", pos - glm::vec3(0.0f, 0.5f, 0.0f), RP_OPAQUE);

	switch (tile)
	{
	case 'W':
		CreateObject("Wall_" + std::to_string(x) + "_" + std::to_string(z),
			 "CUBE", "", "FLAT", pos, RP_OPAQUE);
		break;
	case 'T':
		CreateObject("Torch", "CUBE", "TORCH_DIFFUSE", "TEXDIR", pos, RP_TRANSPARENT);
		break;
	case 'B':
		CreateObject("Beast", "BEAST", "BEAST_DIFFUSE", "TEXDIR", pos, RP_OPAQUE);
		break;
	case 'P':
		m_playerSpawn = pos;
		break;
	default:
		break;
	}
}

void Map::CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, RenderPass rp)
{
	GameObject* obj = new GameObject(name, model, texture, shader);
	obj->SetPosition(pos);
	obj->SetScale(glm::vec3(1.0f));
	obj->Init(m_scene);
	obj->SetRenderPass(rp);
	m_scene->AddGameObject(obj);

	if (model == "CUBE" && name.find("FLOOR") != std::string::npos)
		obj->SetScale(glm::vec3(1.0f, 0.05f, 1.0f));
	else
		obj->SetScale(glm::vec3(1.0f));
}
