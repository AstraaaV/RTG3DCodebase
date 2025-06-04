#pragma once
#include <vector>
#include <string>

class Scene;

class Map {
public:
	Map();
	void GenerateMap();
	void SpawnIntoMap(Scene* scene);
private:
	std::vector<std::string> m_layout;
};