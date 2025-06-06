#include "Map.h"
#include "Scene.h"
#include "GameObject.h"
#include "Light.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Map::Map(Scene* scene) : m_scene(scene) {}

Map::~Map()
{
	
}

void Map::Init()
{
	std::vector<std::string> layout =
	{
		"WWWWWWWWWW",
		"W..T.....W",
		"W..D..P..W",
		"W....G...W",
		"WWWWWWWWWW"
	};
	LoadLayout(layout);
	GenObjects();
}

void Map::LoadLayout(const std::vector<std::string>& mapLayout)
{
	m_layout = mapLayout;
}

void Map::GenObjects()
{
	float tileSize = 1.0f;
	int numRows = static_cast<int>(m_layout.size());

	for (int z = 0; z < numRows; ++z)
	{
		const std::string& row = m_layout[z];
		int flipZ = numRows - 1 - z;

		for (int x = 0; x < row.size(); ++x)
		{
			char tile = row[x];
			switch (tile)
			{
			case 'W':
				AddWall(x, flipZ);
				break;
			case 'T':
				AddTorch(x, flipZ);
				break;
			case 'D':
				AddDoor(x, flipZ);
				break;
			case 'P':
			{
				GameObject* beast = m_scene->GetGameObject("BEAST");

				if (beast)
				{
					beast->SetPos(vec3(x * tileSize, 0.0f, flipZ * tileSize));
					beast->SetAnimated(true);
					beast->EnablePacing(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f, 1.5f);
				}
				break;
			}
			case 'G':
			{
				GameObject* ghost = m_scene->GetGameObject("GHOST");

				if (ghost)
				{
					ghost->SetPos(vec3(x * tileSize, 0.0f, flipZ * tileSize));
					ghost->EnableFloating(true);
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

void Map::AddWall(int x, int z)
{
	vec3 pos = vec3(x, 0.0f, z);
	GameObject* wall = new GameObject();

	wall->SetName("WALL_" + std::to_string(x) + "_" + std::to_string(z));
	wall->SetModel(m_scene->GetModel("CUBE"));
	wall->SetTexture(m_scene->GetTexture("WALL_DIFFUSE"));
	wall->SetShader(m_scene->GetShader("TEXDIR"));
	wall->SetPos(glm::vec3(x, 0.5f, z));
	wall->SetScale(vec3(1.0f));

	m_scene->AddGameObject(wall);
	m_objects.push_back(wall);
}

void Map::AddDoor(int x, int z)
{
	vec3 pos = vec3(x, 0.0f, z);
	GameObject* door = new GameObject();

	door->SetName("DOOR_" + std::to_string(x) + "_" + std::to_string(z));
	door->SetModel(m_scene->GetModel("CUBE"));
	door->SetTexture(m_scene->GetTexture("ROCK"));
	door->SetShader(m_scene->GetShader("FLAT"));
	door->SetPos(pos);
	door->SetScale(vec3(1.0f, 1.5f, 0.1f));

	m_scene->AddGameObject(door);
	m_objects.push_back(door);
}

void Map::AddTorch(int x, int z)
{
	vec3 pos = vec3(x, 0.0f, z);
	GameObject* torch = new GameObject();

	torch->SetName("TORCH_" + std::to_string(x) + "_" + std::to_string(z));
	torch->SetModel(m_scene->GetModel("CUBE"));
	torch->SetTexture(m_scene->GetTexture("ROCK"));
	torch->SetShader(m_scene->GetShader("TEXDIR"));
	torch->SetPos(pos);
	torch->SetScale(vec3(0.5f));

	m_scene->AddGameObject(torch);
	m_objects.push_back(torch);

	Light* point = new Light();
	point->SetName("TORCH_LIGHT_" + std::to_string(x) + "_" + std::to_string(z));
	point->SetPos(vec3(x, 1.5f, z));
	point->SetColour(vec3(1.0f, 0.5f, 0.2f));
	point->SetAmbient(vec3(0.1f, 0.05f, 0.02f));

	m_scene->AddLight(point);
}

