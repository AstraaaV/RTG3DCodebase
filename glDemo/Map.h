#pragma once
#include <vector>
#include <string>
#include "Scene.h"

class Map
{
public:
	Map(Scene* scene);
	~Map();

	void Init();
	void LoadMap();
	const std::vector<std::string>& GetMap() const;

private:
	std::vector<std::string> m_map;
	Scene* m_scene;
	
	void SpawnTile(char tile, int x, int z);
};