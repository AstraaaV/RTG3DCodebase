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
		"WWWWWWWWWWWW",
		"W..T....T..W",
		"W.WWW..WWW.W",
		"W.W......W.W",
		"W.W.W££W.W.W",
		"W.W.WWWW.W.W",
		"W.W......W.W",
		"W.WWW..WWW.W",
		"W..T....T..W",
		"WWWWWWWWWWWW"
	};
}

const std::vector<std::string>& Map::GetMap() const
{
	return m_map;
}

bool Map::IsWall(int x, int z) const
{
	if (z < 0 || z >= m_map.size() || x < 0 || x >= m_map[z].length())
		return true;

	return m_map[z][x] == 'W';
}

void Map::SpawnTile(char tile, int x, int z)
{
	glm::vec3 pos = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));

	glm::vec3 rot(0.0f);

	CreateObject("Floor_" + std::to_string(x) + "_" + std::to_string(z),
		"PLANE", "FLOOR_DIFFUSE", "TEXDIR", pos, rot, RP_OPAQUE);
	
	switch (tile)
	{
	case 'W':
	{
		bool wallL = IsWall(x - 1, z);
		bool wallR = IsWall(x + 1, z);
		bool wallU = IsWall(x, z - 1);
		bool wallD = IsWall(x, z + 1);

		bool hori = (wallL || wallR);
		bool vert = (wallU || wallD);

		if (hori && !vert)
		{
			rot.y = 90.0f;
		}

		CreateObject("Wall_" + std::to_string(x) + "_" + std::to_string(z),
			"WALL", "WALL_DIFFUSE", "TEXDIR", pos, rot, RP_OPAQUE);
		break;
	}
	case 'T':
		CreateObject("Torch", "CUBE", "TORCH_DIFFUSE", "TEXDIR", pos, rot, RP_TRANSPARENT);
		break;
	case 'P':
		m_playerSpawn = pos;
		break;
	default:
		break;
	}
}

void Map::CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, const glm::vec3& rot, RenderPass rp)
{
	GameObject* obj = new GameObject(name, model, texture, shader);
	obj->SetPosition(pos);
	obj->SetRotation(rot);

	if (model == "WALL")
	{
		obj->SetScale(glm::vec3(0.8f));
	}
	else
	{
		obj->SetScale(glm::vec3(1.0f));
	}
	obj->Init(m_scene);
	obj->SetRenderPass(rp);
	m_scene->AddGameObject(obj);
}
