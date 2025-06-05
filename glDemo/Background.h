#pragma once
#include "GameObject.h"
class Model;

class Background : public GameObject
{
public:
	Background();
	~Background() override;

	//load me from the file
	virtual void Load(ifstream& _file);

	//update _window allows for Keyboard access
	virtual void Tick(float _dt, GLFWwindow* window);

	//render this object
	virtual void PreRender();
	virtual void Render();

	virtual void Init(Shader* shader, GLuint texture, Model* model);

protected:

	string m_ShaderName, m_TexName, m_ModelName;

	GLuint m_texture = 0;
	GLuint m_normalMap = 0;
	Shader* m_shader = nullptr;
	Model* m_model = nullptr;
};

