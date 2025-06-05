#include "Map.h"
#include "GameObject.h"
#include "Scene.h"
#include "Light.h"
#include "Beast.h"
#include <RenderPass.h>

Map::Map()
{
	GenerateMap();
}

void Map::GenerateMap()
{
	m_layout =
	{
		"WWWWWWWWWW",
		"W........W",
		"W.T...D..W",
		"W....B...W",
		"W........W",
		"WWWWWWWWWW"
	};
}

void Map::SpawnIntoMap(Scene* scene)
{
	for (int y = 0; y < m_layout.size(); ++y)
	{
		for (int x = 0; x < m_layout[y].size(); ++x)
		{
			char tile = m_layout[y][x];
			glm::vec3 pos = glm::vec3(x, 0.0f, y);

			if (tile == 'W')
			{
				GameObject* wall = new GameObject();
				wall->SetPosition(pos);
				wall->SetModel(scene->GetModel("CUBE"));
				wall->SetTexture(scene->GetTexture("ROCK"));
				wall->SetShader(scene->GetShader("TEXDIR"));
				wall->SetRenderPass(RP_OPAQUE);
				scene->AddGameObject(wall);
			}
			else if (tile == 'T')
			{
				Light* torch = new Light();
				torch->SetType("POINT");
				torch->SetPos(pos + glm::vec3(0, 1.5f, 0));
				torch->SetCol(glm::vec3(1.0f, 0.5f, 0.0f));
				torch->SetAmb(glm::vec3(0.2f));
				scene->AddLight(torch);
			}
			else if (tile == 'B')
			{
				GameObject* beast = new Beast();
				beast->SetPosition(pos);
				beast->SetShader(scene->GetShader("TEXDIR"));
				beast->SetModel(scene->GetModel("BEAST"));
				beast->SetRenderPass(RP_OPAQUE);
				scene->AddGameObject(beast);
			}
			else if (tile == 'D')
			{
				GameObject* door = new GameObject();
				door->SetPosition(pos);
				door->SetModel(scene->GetModel("CUBE"));
				door->SetTexture(scene->GetTexture("DOOR"));
				door->SetShader(scene->GetShader("TEXDIR"));
				door->SetRenderPass(RP_OPAQUE);
				scene->AddGameObject(door);
			}
		}
	}
}
