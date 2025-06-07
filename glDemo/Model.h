#pragma once
#include <string>

using namespace std;

//base class for Models that can be owned by GameObjects so they can be rendered
//current empty as this a interface/strawman to allow them to be put in the same data structure
class Model
{
public:
	Model();
	virtual ~Model();

	virtual void Load(ifstream& _file);
	virtual void Render() {};

	string GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

protected:
	string m_name;
	string m_type;
};