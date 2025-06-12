#pragma once
#include "core.h"
#include <stdio.h>
#include <string>
#include "Shader.h"
#include "RenderPass.h"

using namespace std;
using namespace glm;

class Scene;

//Basic scene object class - used for rendering only
class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader);
	virtual ~GameObject();

	// File -> Object (called by Scene::Load)
	virtual void Load(ifstream& _file);

	// Update per-frame (idle bob, scanning, etc)
	virtual void Tick(float _dt);

	virtual void PreRender(); //Set up any shader values needed for this object
	virtual void Render(); //Render this object

	// Various getters and setters
	void SetName(string _name) { m_name = _name; }
	string GetName() { return m_name; }

	void SetShader(Shader* _shader);
	GLuint GetShaderProg() { return m_ShaderProg; }
	const std::string& GetShaderName() const { return m_shaderName; }
	const std::string& GetModelName() const { return m_modelName; }
	
	void SetPosition(const glm::vec3& pos) { m_pos = pos; }
	glm::vec3 GetPosition() const { return m_pos; }

	void SetRotation(const glm::vec3& rot);
	const glm::vec3& GetRotation() const;

	void SetScale(const glm::vec3& scale);

	void SetTexture(const std::string& texture) { m_textureName = texture; }
	void SetTexture2(const std::string& texture) { m_textureName2 = texture; }
	void SetTexture3(const std::string& texture) { m_textureName3 = texture; }
	void SetTexture4(const std::string& texture) { m_textureName4 = texture; }

	void SetRenderPass(RenderPass rp);
	// This GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }

	void SetParent(GameObject* parent) { m_parent = parent; }
	GameObject* GetParent() const { return m_parent; }

	void SetLocalOffset(const glm::vec3& offset) { m_localOffset = offset; }
	
	glm::mat4 GetModelMatrix() const;

	// Scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

protected:
	// Names
	string m_name;
	string m_type;
	string m_shaderName;

	string m_modelName;
	string m_textureName; // albedo
	string m_textureName2; // normal
	string m_textureName3; // roughness
	string m_textureName4; // metallic
	string m_textureName5; // height

	// Transform
	vec3		m_pos;
	vec3		m_rot;
	vec3		m_scale;

	vec3		m_rot_incr;
	glm::vec3 m_localOffset = glm::vec3(0.0f);

	float m_baseY = 0.0f;
	float m_baseRotY = 0.0f;
	
	glm::mat4	m_worldMatrix;
	GLuint m_ShaderProg;

	RenderPass m_RP = RP_OPAQUE;
	
	// Scene links
	Scene* m_scene = nullptr;
	Shader* m_shader = nullptr;
	GameObject* m_parent = nullptr;
};
