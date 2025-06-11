#pragma once
#include "core.h"
#include <stdio.h>
#include <string>
#include "Shader.h"
#include "RenderPass.h"

using namespace std;
class Scene;

using namespace glm;

//Base class of a GameObject
class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader);
	virtual ~GameObject();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update the GameObject
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt);

	virtual void PreRender();//set up any shader values needed for this object
	virtual void Render();//render this object

	//various getters and setters
	void SetName(string _name) { m_name = _name; }
	string GetName() { return m_name; }
	GLuint GetShaderProg() { return m_ShaderProg; }
	const std::string& GetModelName() const { return m_modelName; }
	void SetShader(Shader* _shader);

	glm::mat4 GetModelMatrix() const;

	const std::string& GetShaderName() const { return m_shaderName; }

	void SetPosition(const glm::vec3& pos) { m_pos = pos; }
	glm::vec3 GetPosition() const { return m_pos; }

	void SetRotation(const glm::vec3& rot);
	const glm::vec3& GetRotation() const;

	void SetTexture(const std::string& texture) { m_textureName = texture; }
	void SetTexture2(const std::string& texture) { m_textureName2 = texture; }
	void SetTexture3(const std::string& texture) { m_textureName3 = texture; }
	void SetTexture4(const std::string& texture) { m_textureName4 = texture; }

	void SetScale(const glm::vec3& scale);
	void SetRenderPass(RenderPass rp);

	void SetVisible(bool visible);
	bool IsVisible() const;

	void SetCollide(bool val) { m_collidable = val; }
	bool IsCollide() const { return m_collidable; }

	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

	//this GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }

	void SetParent(GameObject* parent) { m_parent = parent; }
	GameObject* GetParent() const { return m_parent; }

	void SetLocalOffset(const glm::vec3& offset) { m_localOffset = offset; }

protected:

	string m_name;
	string m_type;
	string m_modelName;
	string m_textureName;
	string m_textureName2; //normal
	string m_textureName3; //roughness
	string m_textureName4;
	string m_textureName5;
	string m_shaderName;

	vec3		m_pos;
	vec3		m_rot;
	vec3		m_scale;
	vec3		m_rot_incr;

	glm::mat4	m_worldMatrix;
	glm::vec3 m_localOffset = glm::vec3(0.0f);

	GLuint m_ShaderProg;

	RenderPass m_RP = RP_OPAQUE;

	bool m_visible = true;
	bool m_collidable = false;

	float m_baseY = 0.0f;
	float m_baseRotY = 0.0f;

	Scene* m_scene = nullptr;
	Shader* m_shader = nullptr;
	GameObject* m_parent = nullptr;
};
