#pragma once
#include "core.h"
#include <stdio.h>
#include <string>
#include "RenderPass.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"

using namespace std;
class Scene;

using namespace glm;

//Base class of a GameObject
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update the GameObject
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt);

	virtual void PreRender();//set up any shader values needed for this object
	virtual void Render();//render this object

	//various getters and setters
	void SetModel(Model* model) { m_model = model; }
	void SetTexture(Texture* tex) { m_texture = tex; }
	void SetShader(Shader* shader) { m_shader = shader; }
	void SetPos(const glm::vec3& pos) { m_pos = pos; }
	void SetScale(const glm::vec3& scale) { m_scale = scale; }
	void SetName(string _name) { m_name = _name; }
	
	string GetName() { return m_name; }
	GLuint GetShaderProg() { return m_ShaderProg; }

	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

	//this GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }

protected:

	string m_name;
	string m_type;

	Model* m_model = nullptr;
	Texture* m_texture = nullptr;
	Shader* m_shader = nullptr;

	vec3		m_pos;
	vec3		m_rot;
	vec3		m_scale;
	vec3		m_rot_incr;

	glm::mat4	m_worldMatrix;

	GLuint m_ShaderProg;

	RenderPass m_RP = RP_OPAQUE;
};
