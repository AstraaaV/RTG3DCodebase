#pragma once
#include "core.h"
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <AIMesh.h>

using namespace std;

class GameObject;
class Camera;
class Light;
class Model;
class Texture;
class Shader;
class Cube;

//Note quite a proper scene graph but this contains data structures for all of our bits and pieces we want to draw
class Scene
{
public:
	Scene();
	~Scene();

	//tick all GOs
	void Update(float _dt, GLFWwindow* window);

	//add this GO to my list
	void AddGameObject(GameObject* _new);

	//method to cycle through cameras
	void CycleCams();

	//return a pointer to a given thing by its name
	GameObject* GetGameObject(string _GOName);
	Camera* GetCamera(string _camName);
	Light* GetLight(string _lightName);
	Texture* GetTexture(string _texName);
	Model* GetModel(string _modelName);
	Shader* GetShader(string _shaderName);

	//Render Everything
	void Render();

	void RenderCreature(GLuint shaderProgram);

	void RenderTorches(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projMatrix,
		const glm::vec3& lightPos, const glm::vec3& lightCol, const glm::vec3& ambientCol);

	void RenderFloorCeiling(GLuint shader, const glm::mat4& view, const glm::mat4& proj, GLuint texture);

	void RenderMapLayout(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& prokection);

	//set up all shader uniform values for all of our lights
	void SetShaderUniforms(GLuint _shaderprog);

	//load from file
	void Load(ifstream& _file);

	//initialise links between items in the scene
	void Init();

	void SetActiveCamera(Camera* cam);

	Camera* GetActiveCamera() { return m_useCamera; }

	Cube* GetCube() const { return m_cube; }

	const std::vector<std::string>& GetMapLayout() const { return m_mapLayout; }

	void AddCamera(Camera* cam);

	void GenerateTorchPos();
	const std::vector<glm::vec3>& GetTorchPositions() const { return m_torchPos; }

	glm::vec3 GetDirLightDirection() const { return m_dirLightDirection; }
	glm::vec3 GetDirLightColour() const { return m_dirLightColour; }
	glm::vec3 GetDirLightAmbient() const { return m_dirLightAmbient; }

	glm::vec3 GetPointLightPosition() const { return m_pointLightPosition; }
	glm::vec3 GetPointLightColour() const { return m_pointLightColour; }
	glm::vec3 GetPointLightAmbient() const { return m_pointLightAmbient; }

	void SetLightsEnabled(bool enabled) { m_lightsEnabled = enabled; }
	bool GetLightsEnabled() const { return m_lightsEnabled; }

protected:

	//data structures containing pointers to all our stuff
	int m_numCameras = 0;
	int m_numLights = 0;
	int m_numGameObjects = 0;
	int m_numModels = 0;
	int m_numTextures = 0;
	int m_numShaders = 0;
	std::list<Camera*>    m_Cameras;
	std::list<Light*>    m_Lights;
	std::list<Model*>		m_Models;
	std::list<Texture*>		m_Textures;
	std::list<Shader*>		m_Shaders;
	std::list<GameObject*> m_GameObjects;

	std::vector<std::string> m_mapLayout;
	std::vector<glm::vec3> m_torchPos;

	glm::vec3 m_dirLightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	glm::vec3 m_dirLightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_dirLightAmbient = glm::vec3(0.4f, 0.4f, 0.4f);

	glm::vec3 m_pointLightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_pointLightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_pointLightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);

	Cube* m_cube = nullptr;

	Camera* m_useCamera = nullptr; //current main camera in use
	int m_useCameraIndex = 0;
	int m_activeCameraIndex = 0;
	bool m_camSwitchPressed = false;

	bool m_lightsEnabled = true;

	// Examples
	AIMesh* m_creatureMesh = nullptr;
	glm::vec3 m_beastPos = glm::vec3(2.0f, 0.0f, 0.0f);
	float m_beastRotation = 0.0f;
};

