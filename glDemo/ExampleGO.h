#pragma once
#include "GameObject.h"

class Model;
class Shader;
class Texture;

//replicate the examples from the main.cpp
//each Game Object has a Mesh, a texture and a shader
//which it uses to render itself
class ExampleGO :
	public GameObject
{
public:
	ExampleGO();
	~ExampleGO();

	//load me from the file
	virtual void Load(std::ifstream& _file);

	//update _window allows for Keyboard access
	virtual void Tick(float _dt, GLFWwindow* window);

	//render this object
	virtual void PreRender();
	virtual void Render();

protected:

	string m_ShaderName, m_TexName, m_ModelName;

	Shader* m_shader = nullptr;
	Texture* m_texture = nullptr;
	GLuint m_normalMap = 0;

	Model* m_model = nullptr;
};

