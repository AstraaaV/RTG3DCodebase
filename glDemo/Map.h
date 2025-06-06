#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class GameObject;
class Scene;

class Map
{
public:
	Map(Scene* scene);
	~Map();

	void Init();
	void LoadLayout(const std::vector<std::string>& mapLayout);
	void GenObjects();

private:
	Scene* m_scene;
	std::vector<std::string> m_layout;
	std::vector<GameObject*> m_objects;

	void AddWall(int x, int z);
	void AddDoor(int x, int z);
	void AddTorch(int x, int z);
};