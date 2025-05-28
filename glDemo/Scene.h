#pragma once
#include "core.h"
#include <list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include "Cube.h"
#include "AIMesh.h"

//Note quite a proper scene graph but this contains data structures for all of our bits and pieces we want to draw
class Scene
{
public:
	Scene();
	~Scene();

	void Init();
	void Update(float _dt, GLFWwindow* window);
	void Render();
	void BuildMap();
	void RenderMapLayout(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection);
	void RenderCreature();
	void RenderPlayerMarker(GLuint shaderProgram);
	void RenderTorches(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);

	void SetShaderUniforms(GLuint _shaderprog);
	void GenerateTorchPos();

	void AddGameObject(GameObject* _new);
	void AddCamera(Camera* cam);
	void SetActiveCamera(Camera* cam);
	void CycleCams();

	Camera* GetActiveCamera() { return m_useCamera; }
	Cube* GetCube() const { return m_cube; }

	void SetWallTexture(GLuint tex) { m_wallTex = tex; }
	GLuint GetWallTexture() const { return m_wallTex; }

	void SetTexDirLightShader(GLuint shader) { m_texDirLightShader = shader; }
	void SetTexPointLightShader(GLuint shader) { m_texPointLightShader = shader; }

	GLuint GetTexDirLightShader() const { return m_texDirLightShader; }
	GLuint GetTexPointLightShader() const { return m_texPointLightShader; }

	glm::vec3 GetDirLightDirection() const { return m_dirLightDirection; }
	glm::vec3 GetDirLightColour() const { return m_dirLightColour; }
	glm::vec3 GetDirLightAmbient() const { return m_dirLightAmbient; }

	glm::vec3 GetPointLightPosition() const { return m_pointLightPosition; }
	glm::vec3 GetPointLightColour() const { return m_pointLightColour; }
	glm::vec3 GetPointLightAmbient() const { return m_pointLightAmbient; }

	void SetLightsEnabled(bool enabled) { m_lightsEnabled = enabled; }
	void Load(ifstream& _file);

protected:
	std::vector<Camera*>    m_Cameras;
	std::list<Light*>    m_Lights;
	std::list<Model*>		m_Models;
	std::list<Texture*>		m_Textures;
	std::list<Shader*>		m_Shaders;
	std::list<GameObject*> m_GameObjects;

	std::vector<std::string> m_mapLayout;
	std::vector<glm::vec3> m_torchPos;

	Cube* m_cube = nullptr;
	AIMesh* m_creatureMesh = nullptr;

	Camera* m_useCamera = nullptr; //current main camera in use
	Camera* m_activeCamera = nullptr;
	int m_useCameraIndex = 0;
	bool m_camSwitchPressed = false;

	bool m_lightsEnabled = false;

	GLuint m_wallTex = 0;
	GLuint m_texDirLightShader = 0;
	GLuint m_texPointLightShader = 0;

	int m_numCameras = 0;
	int m_numGameObjects = 0;
	int m_numLights = 0;
	int m_numModels = 0;
	int m_numShaders = 0;
	int m_numTextures = 0;

	glm::vec3 m_dirLightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	glm::vec3 m_dirLightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_dirLightAmbient = glm::vec3(0.7f, 0.7f, 0.7f);

	glm::vec3 m_pointLightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_pointLightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_pointLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
};

