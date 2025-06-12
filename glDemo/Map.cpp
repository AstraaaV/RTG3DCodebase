#include "Map.h"
#include "GameObject.h"
#include "Scene.h"
#include "Light.h"

Map::Map(Scene* scene) : m_scene(scene) {}

Map::~Map() {}

void Map::Init()
{
	// 1. Creates floor geometry
	CreateFloor(24, 24);

	// 2. Outer Box (torches every 5 tiles)
	CreateLongWall(-4, 24, 24, true, true); // Left
	CreateLongWall(-4, 0, 24, true, true); // Right
	CreateLongWall(0, 4, 24, false, true); // Bottom
	CreateLongWall(24, 4, 24, false, true); // Top

	// 3. Interior Walls
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

	// 4. Player start
	SetPlayerSpawn(7, 7);
}

// Player spawn point (start position for player)
void Map::SetPlayerSpawn(int x, int z)
{
	m_playerSpawn = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
}

// Torch spawner
void Map::CreateTorch(int x, int z, const std::string& direction)
{
	// 1. Basic transform
	glm::vec3 base(x, 0.0f, z);
	glm::vec3 rot(0.0f);
	glm::vec3 offset(0.0f);

	if (direction == "SOUTH") { rot.y = 0.0f; offset = { 0.0f, 1.2f, 0.45f }; }
	if (direction == "NORTH") { rot.y = 180.0f; offset = { 0.0f, 1.2f, -0.45f }; }
	if (direction == "EAST") { rot.y = 90.0f; offset = { 0.45f, 1.2f, 0.0f }; }
	if (direction == "WEST") { rot.y = 270.0f; offset = { -0.45f, 1.2f, 0.0f }; }

	glm::vec3 worldPos = base + offset;
	std::string tag = "Torch_" + std::to_string(x) + "_" + std::to_string(z);

	// 2. Sconce creator
	CreateObject("Sconce_" + std::to_string(x) + "_" + std::to_string(z),
		"WALLSCONCE", "WALLSCONCE_BASE", "TEXPBR", worldPos, rot, RP_OPAQUE);

	// 3. Transparent flame
	glm::vec3 flamePos = worldPos + glm::vec3(0.0f, 0.3f, 0.0f);
	CreateObject("Flame_" + tag, "PLANE", "FLAME_TEX", "EMISSIVE", flamePos, glm::vec3(0), RP_TRANSPARENT);

	// 4. Light source
	Light* torchLight = new Light();
	torchLight->SetName(tag);
	torchLight->SetType("POINT");
	torchLight->SetPos(worldPos);
	torchLight->SetColour(glm::vec3(1.0f, 0.6f, 0.2f));
	torchLight->SetAmbient(glm::vec3(0.05f, 0.03f, 0.01f));
	torchLight->SetAttenuation(1.0f, 0.09f, 0.032f);
	m_scene->AddLight(torchLight);
}

// Spawns all objects
void Map::CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, const glm::vec3& rot, RenderPass rp)
{
	GameObject* obj = new GameObject(name, model, texture, shader);
	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetRenderPass(rp);

	// 1. Quick scaler & helps render textures
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
	else if (model == "PLANE" && texture == "FLAME_TEX")
	{
		obj->SetScale(glm::vec3(0.5f));
		obj->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		obj->SetScale(glm::vec3(1.0f));
	}
	obj->Init(m_scene);
	m_scene->AddGameObject(obj);
}

// Builds the walls
void Map::CreateLongWall(int startX, int startZ, int length, bool horizontal, bool placeTorches)
{
	glm::vec3 rot(0.0f);

	if (!horizontal)
		rot.y = 90.0f;

	// 1. Wall maker (horizontal & vertical)
	for (int i = 0; i < length; ++i)
	{
		int x = horizontal ? startX + i : startX;
		int z = horizontal ? startZ : startZ + i;

		glm::vec3 pos(static_cast<float>(x), 0.0f, static_cast<float>(z));

		if (rot.y == 90.0f)
			pos.z += 0.5f; // Centres the z-aligned walls

		CreateObject("LongWall_" + std::to_string(x) + "_" + std::to_string(z),
			"WALL", "WALL_DIFFUSE", "TEXWALL", pos, rot, RP_OPAQUE);

		// 2. Auto torch placer (Every 5 tiles)
		if (placeTorches)
		{
			int centerIndex = length / 2;

			if (i == centerIndex)
			{
				string facing = horizontal ? "SOUTH" : "EAST";

				bool canPlace = false;

				if (horizontal)
					canPlace = (z + 1 < 100);
				else
					canPlace = (x + 1 < 100);

				if (canPlace)
				{
					CreateTorch(x, z, facing);
				}
			}
		}
	}
}

// Floor tiler
void Map::CreateFloor(int w, int h)
{
	glm::vec3 rot(0.0f);

	// 1. Floor creator
	for (int z = 0; z < h; ++z)
	{
		for (int x = 0; x < w; ++x)
		{
			glm::vec3 pos(static_cast<float>(x), 0.0f, static_cast<float>(z));

			CreateObject("Floor_" + std::to_string(x) + "_" + std::to_string(z),
				"PLANE", "ROCK", "TEXMAP", pos, rot, RP_OPAQUE);
		}
	}
}
