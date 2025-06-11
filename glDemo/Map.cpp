#include "Map.h"
#include "GameObject.h"
#include "Scene.h"
#include "Light.h"

Map::Map(Scene* scene) : m_scene(scene) {}

Map::~Map() {}

void Map::Init()
{
	CreateFloor(24, 24);

	// Outer Box
	CreateLongWall(-4, 24, 24, true, true); // Left
	CreateLongWall(-4, 0, 24, true, true); // Right
	CreateLongWall(0, 4, 24, false, true); // Bottom
	CreateLongWall(24, 4, 24, false, true); // Top

	// Interior Walls
	CreateLongWall(6, 5, 14, true, true);
	CreateLongWall(12, 10, 6, true, true);
	CreateLongWall(-4, 3, 4, true, false);
	CreateLongWall(4, 19, 4, true, false);
	CreateLongWall(10, 16, 4, true, false);
	CreateLongWall(0, 12, 4, true, false);

	CreateLongWall(8, 14, 12, false, true);
	CreateLongWall(18, 15, 5, false, true);
	CreateLongWall(4, 19, 9, false, false);
	CreateLongWall(16, 10, 4, false, false);
	CreateLongWall(4, 7, 4, false, false);

	SetPlayerSpawn(7, 7);
}


void Map::SetPlayerSpawn(int x, int z)
{
	m_playerSpawn = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
}

void Map::CreateTorch(int x, int z, const std::string& direction)
{
	glm::vec3 pos = glm::vec3(static_cast<float>(x), 1.0f, static_cast<float>(z));
	glm::vec3 rot(0.0f);

	CreateObject("Sconce_" + std::to_string(x) + "_" + std::to_string(z),
		"WALLSCONCE", "WALLSCONCE_BASE", "TEXPBR", pos, rot, RP_OPAQUE);

	GameObject* sconce = m_scene->GetGameObject("Sconce_" + std::to_string(x) + "_" + std::to_string(z));
}

void Map::CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, const glm::vec3& rot, RenderPass rp)
{
	GameObject* obj = new GameObject(name, model, texture, shader);
	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetRenderPass(rp);

	if (model == "WALL")
	{
		obj->SetScale(glm::vec3(1.0f, 2.0f, 1.0f));
	}
	else if (model == "WALLSCONCE")
	{
		obj->SetScale(glm::vec3(0.9f));
		obj->SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		obj->SetTexture("WALLSCONCE_BASE");
		obj->SetTexture2("WALLSCONCE_NORMAL");
		obj->SetTexture3("WALLSCONCE_ROUGHNESS");
		obj->SetTexture4("WALLSCONCE_METALLIC");
	}
	else
	{
		obj->SetScale(glm::vec3(1.0f));
	}
	obj->Init(m_scene);
	m_scene->AddGameObject(obj);
}

void Map::CreateLongWall(int startX, int startZ, int length, bool horizontal, bool placeTorches)
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
			"WALL", "WALL_DIFFUSE", "TEXWALL", pos, rot, RP_OPAQUE);

		if (placeTorches && i % 5 == 0)
		{
			std::string facing = horizontal ? "SOUTH" : "EAST";
			CreateTorch(x, z, facing);
		}
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
				"PLANE", "", "FLAT", pos, rot, RP_OPAQUE);
		}
	}
}
