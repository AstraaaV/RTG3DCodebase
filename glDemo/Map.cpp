#include "Map.h"
#include "GameObject.h"
#include "Scene.h"

Map::Map(Scene* scene) : m_scene(scene) {}

Map::~Map() {}

void Map::Init()
{
	CreateFloor(24, 24);

	// Outer Box
	CreateLongWall(-4, 24, 24, true); // Left
	CreateLongWall(-4, 0, 24, true); // Right
	CreateLongWall(0, 4, 24, false); // Bottom
	CreateLongWall(24, 4, 24, false); // Top

	// Interior Walls
	CreateLongWall(6, 5, 14, true);
	CreateLongWall(12, 10, 6, true);
	CreateLongWall(-4, 3, 4, true);
	CreateLongWall(4, 19, 4, true);
	CreateLongWall(10, 16, 4, true);
	CreateLongWall(0, 12, 4, true);
	CreateLongWall(8, 14, 12, false);
	CreateLongWall(18, 15, 5, false);
	CreateLongWall(4, 19, 9, false);
	CreateLongWall(16, 10, 4, false);
	CreateLongWall(4, 7, 4, false);

	// Torches
	CreateTorch(3, 1);
	CreateTorch(8, 1);

	SetPlayerSpawn(1, 1);
}

void Map::SetPlayerSpawn(int x, int z)
{
	m_playerSpawn = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
}

void Map::CreateTorch(int x, int z)
{
	glm::vec3 pos = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
	glm::vec3 rot(0.0f);

	CreateObject("Lantern_" + std::to_string(x) + "_" + std::to_string(z),
		"LANTERN", "LANTERN_BASE", "TEXPBR", pos, rot, RP_OPAQUE);

	glm::vec3 lightPos = pos + glm::vec3(0.0f, 1.5f, 0.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 0.8f, 0.5f);
	float lightIntensity = 1.0f;
	m_scene->AddPointLight(lightPos, lightCol, lightIntensity);
}

void Map::CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, const glm::vec3& rot, RenderPass rp)
{
	GameObject* obj = new GameObject(name, model, texture, shader);
	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetRenderPass(rp);

	if (model == "WALL" || model == "LANTERN")
	{
		obj->SetCollide(true);
	}
	else
	{
		obj->SetCollide(false);
	}

	if (model == "WALL")
	{
		obj->SetScale(glm::vec3(1.0f));
	}
	else if (model == "LANTERN")
	{
		obj->SetScale(glm::vec3(0.5f));
		obj->SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
	}
	else
	{
		obj->SetScale(glm::vec3(1.0f));
	}
	obj->Init(m_scene);
	m_scene->AddGameObject(obj);
}

void Map::CreateLongWall(int startX, int startZ, int length, bool horizontal)
{
	glm::vec3 rot(0.0f);

	if (!horizontal)
		rot.y = 90.0f;

	for (int i = 0; i < length; ++i)
	{
		int x = horizontal ? startX + i : startX;
		int z = horizontal ? startZ : startZ + i;

		glm::vec3 pos(static_cast<float>(x), 0.0f, static_cast<float>(z));

		if (rot.y == 90.0f)
			pos.z += 0.5f;

		CreateObject("LongWall_" + std::to_string(x) + "_" + std::to_string(z),
			"WALL", "WALL_DIFFUSE", "TEXDIR", pos, rot, RP_OPAQUE);
	}
}

void Map::CreateFloor(int w, int h)
{
	glm::vec3 rot(0.0f);

	for (int z = 0; z < h; ++z)
	{
		for (int x = 0; x < w; ++x)
		{
			glm::vec3 pos(static_cast<float>(x), 0.0f, static_cast<float>(z));

			CreateObject("Floor_" + std::to_string(x) + "_" + std::to_string(z),
				"PLANE", "FLOOR_DIFFUSE", "TEXDIR", pos, rot, RP_OPAQUE);
		}
	}
}
