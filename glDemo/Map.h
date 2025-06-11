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

	// Used to load map layout and spawn stuff
	void Init();

	// Returns spawn pos of player in world coords
	glm::vec3 GetPlayerSpawn() const { return m_playerSpawn; }
	void SetPlayerSpawn(int x, int z);

	void CreateTorch(int x, int z, const std::string& direction);

private:
	Scene* m_scene; // Spawns the objects and lights

	glm::vec3 m_playerSpawn = glm::vec3(0.0f);

	int m_width;
	int m_height;

	void CreateObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader, const glm::vec3& pos, const glm::vec3& rot, RenderPass rp);

	void CreateLongWall(int startX, int startZ, int length, bool horizontal);
	void CreateFloor(int w, int h);
};