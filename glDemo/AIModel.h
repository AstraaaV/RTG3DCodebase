#pragma once
#include "Model.h"
class AIMesh;

//Uses ASSIMP to load/render in an obj model

class AIModel :
	public Model
{
public:
	AIModel();
	virtual ~AIModel();

	void Load(ifstream& _file) override;
	void Render() override;

protected:
	AIMesh* m_AImesh;
};

