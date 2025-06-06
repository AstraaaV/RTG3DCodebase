#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
#include "Light.h"

class Map
{
public:
	Map();
	~Map();

	void Init();

	const std::vector<GameObject*>& GetObjects() const;

private:
	std::vector<std::string> m_layout;
	std::vector<GameObject*> m_objects;
};