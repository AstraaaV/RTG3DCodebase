#include "AIModel.h"
#include "stringHelp.h"
#include "AIMesh.h"

AIModel::AIModel()
{
	m_type = "AI";
	m_AImesh = nullptr;
}

AIModel::~AIModel()
{
	delete m_AImesh;
}

void AIModel::Load(ifstream& _file)
{
	Model::Load(_file);
	string fileName;
	StringHelp::String(_file, "FILE", fileName);

	m_AImesh = new AIMesh(fileName);
}

void AIModel::Render()
{
	m_AImesh->render();
}