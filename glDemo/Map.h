#pragma once
#include <vector>
#include <string>
#include "Scene.h"
#include "RenderPass.h"

class Map
{
public:
	Map(Scene* scene);
	~Map();

	void Init();
	void LoadMap();
	const std::vector<std::string>& GetMap() const;

	glm::vec3 GetPlayerSpawn() const { return m_playerSpawn; }
	bool IsWall(int x, int z) const;

private:
	std::vector<std::string> m_map;
	Scene* m_scene;

	glm::vec3 m_playerSpawn = glm::vec3(0.0f);
	
	void SpawnTile(char tile, int x, int z);
	void CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, RenderPass rp);
};