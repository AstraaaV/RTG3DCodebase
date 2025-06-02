#include "Scene.h"
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include "Cube.h"
#include "Beast.h"
#include <assert.h>
#include <helper.h>
#include <RenderPass.h>
#include "IsometricCamera.h"
#include "TextureLoader.h"
#include <iostream>

Scene::Scene()
{
}

Scene::~Scene()
{
	for (auto cam : m_Cameras) delete cam;
	for (auto light : m_Lights) delete light;
	for (auto model : m_Models) delete model;
	for (auto texture : m_Textures) delete texture;
	for (auto shader : m_Shaders) delete shader;
	for (auto go : m_GameObjects) delete go;
	if (m_cube) { delete m_cube; m_cube = nullptr; }
}

void Scene::Init()
{
	GLuint wallTex = loadTexture("Assets\\Textures\\rock_wall.JPG", FIF_JPEG);

	if (wallTex == 0)
	{
		std::cout << "Failed to load the wall texture." << std::endl;
	}
	else
	{
		SetWallTexture(wallTex);
	}

	ifstream manifest("manifest.txt");

	if (manifest.is_open())
	{
		Load(manifest);
		manifest.close();
	}
	else
	{
		std::cout << "Failed to open manifest.txt\n" << std::endl;
	}

	m_cube = new Cube();
	BuildMap();

	m_torchPos.clear();

	for (int row = 0; row < m_mapLayout.size(); ++row)
	{
		for (int col = 0; col < m_mapLayout[row].size(); ++col)
		{
			if (m_mapLayout[row][col] == 'T')
			{
				float x = col * 2.2f;
				float z = row * 2.2f;
				m_torchPos.push_back(glm::vec3(x, 1.5f, z));
			}
		}
	}

	Beast* beast = new Beast();
	beast->SetShader(m_texDirLightShader);
	AddGameObject(beast);

	// Isometric Camera
	m_isoCamera = new IsometricCamera();
	AddCamera(m_isoCamera);

	if (!m_Cameras.empty())
	{
		m_useCamera = *m_Cameras.begin();
		m_useCameraIndex = 0;
	}
}

void Scene::BuildMap()
{
	m_mapLayout.clear();
	int width = 10, height = 10;

	for (int row = 0; row < height; ++row)
	{
		std::string line;
		for (int col = 0; col < width; ++col)
		{
			if (row == 0 || col == 0 || row == height - 1 || col == width - 1)
			{
				line += 'W';
			}
			else if (row == height / 2 && col == width - 1)
			{
				line += 'P';
			}
			else if ((row + col) % 7 == 0)
			{
				line += 'T';
			}
			else
			{
				line += '.';
			}
		}
		m_mapLayout.push_back(line);
	}
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !m_camSwitchPressed)
	{
		CycleCams();
		m_camSwitchPressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
	{
		m_camSwitchPressed = false;
	}

	//update lights, cams, and game objects
	for (auto light : m_Lights) light->Tick(_dt, window);
	
	if (m_useCamera)
	{
		m_useCamera->Tick(_dt, window);
	}

	for (auto go : m_GameObjects) go->Tick(_dt, window);
}

void Scene::CycleCams()
{
	if (m_Cameras.empty()) return;

	m_useCameraIndex = (m_useCameraIndex + 1) % m_Cameras.size();
	auto it = m_Cameras.begin();
	std::advance(it, m_useCameraIndex);
	
	if (it != m_Cameras.end() && *it != nullptr)
	{
		m_useCamera = *it;
		m_activeCamera = m_useCamera;
		std::cout << "Switched to cam: " << m_useCamera->GetName() << endl;
	}
	else
	{
		std::cout << "Camera switch failed." << endl;
	}
}

//Render Everything
void Scene::Render()
{
	for (GameObject* go : m_GameObjects)
	{
		if (go->GetRenderPass() == RP_BACKGROUND)
			go->Render();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::cout << "TEXDIR: " << m_texDirLightShader << " | TEXPOINT: " << m_texPointLightShader << endl;
	
	if (!m_useCamera) return;

	mat4 view = m_useCamera->GetView();
	mat4 proj = m_useCamera->GetProj();

	RenderMapLayout(m_texDirLightShader, view, proj);
	RenderTorches(m_texPointLightShader, view, proj);

	if (dynamic_cast<FirstPersonCamera*>(m_useCamera))
	{
		RenderCreature();
	}
}

void Scene::RenderMapLayout(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection)
{
	if (!m_cube)
	{
		std::cout << "ERROR: m_cube is null!" << endl;
		return;
	}

	GLint pLocation;

	if (shaderProgram == 0)
	{
		std::cout << "ERROR: Shader program is 0!" << std::endl;
		return;
	}

	glUseProgram(shaderProgram);

	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &view[0][0]);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projection[0][0]);

	SetShaderUniforms(shaderProgram);

	Helper::SetUniformLocation(shaderProgram, "lightsEnabled", &pLocation);
	glUniform1i(pLocation, m_lightsEnabled ? 1 : 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_wallTex);
	Helper::SetUniformLocation(shaderProgram, "texture", &pLocation);
	glUniform1i(pLocation, 0);

	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);

	for (int row = 0; row < m_mapLayout.size(); ++row)
	{
		for (int col = 0; col < m_mapLayout[row].length(); ++col)
		{
			char tile = m_mapLayout[row][col];
			float x = col * 2.2f, z = row * 2.2f;
			mat4 model;

			switch (tile)
			{
			case 'W': // Wall
				model = translate(mat4(1), vec3(x, 1.0f, z)) *
					scale(mat4(1), vec3(2.0f, 4.0f, 2.0f));
				break;

			case 'T': // Torch
				model = translate(mat4(1), vec3(x, 1.5f, z)) *
					scale(mat4(1), vec3(0.3f, 2.0f, 0.3f));
				break;

			case 'D': // Door
				model = translate(mat4(1), vec3(x, 1.0f, z)) *
					scale(mat4(1), vec3(1.0f, 2.0f, 0.3f));
				break;
			case 'P':
			case '.':
				model = translate(mat4(1), vec3(x, -1.0f, z)) *
					scale(mat4(1), vec3(2.2f, 0.5f, 2.2f));
				break;
			default:
				continue;
			}
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, &model[0][0]);
			m_cube->render();

			if (tile == 'P')
			{
				glm::mat4 markerCube = translate(mat4(1), vec3(x, 1.0f, z)) *
					scale(mat4(1), vec3(1.0f, 2.0f, 1.0f));
				glUniformMatrix4fv(pLocation, 1, GL_FALSE, &markerCube[0][0]);
				m_cube->render();
			}
		}
	}
}

void Scene::RenderTorches(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	GLint loc;

	int lightCount = static_cast<int>(m_torchPos.size());
	if (lightCount > 16) lightCount = 16;
	
	if (!m_cube) return;

	if (shaderProgram == 0)
	{
		std::cout << "ERROR: Shader program is 0!" << endl;
		return;
	}

	glUseProgram(shaderProgram);

	GLint pLocation;
	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projMatrix[0][0]);
	
	for (int i = 0; i < lightCount; ++i)
	{
		std::string idx = std::to_string(i);
		glm::vec3 pos = m_torchPos[i];

		glUniform3fv(glGetUniformLocation(shaderProgram, ("lightPos[" + idx + "]").c_str()), 1, &pos[0]);
	
		glm::vec3 lightColour(1.0f, 0.7f, 0.3f);
		glUniform3fv(glGetUniformLocation(shaderProgram, ("lightColour[" + idx + "]").c_str()), 1, &lightColour[0]);
	
		glm::vec3 ambientColour(0.2f, 0.1f, 0.05f);
		glUniform3fv(glGetUniformLocation(shaderProgram, ("ambientColour[" + idx + "]").c_str()), 1, &ambientColour[0]);

		glUniform1f(glGetUniformLocation(shaderProgram, ("intensity[" + idx + "]").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, ("constant[" + idx + "]").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, ("linear[" + idx + "]").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(shaderProgram, ("quadratic[" + idx + "]").c_str()), 0.032f);
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "lightCount"), lightCount);

	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);

	for (const auto& pos : m_torchPos)
	{
		mat4 model = glm::translate(mat4(1), pos) * glm::scale(mat4(1), glm::vec3(0.3f));
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &model[0][0]);

		m_cube->render();
	}
}

void Scene::RenderCreature()
{
	if (m_beast && m_showBeast)
	{
		m_beast->Render();
	}
}

void Scene::RenderPlayerMarker(GLuint shaderProgram)
{
	if (!m_cube || !m_useCamera) return;

	glUseProgram(shaderProgram);

	GLint loc;
	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &loc);

	glm::vec3 camPos = m_useCamera->GetPos();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), camPos) *
	glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

	m_cube->render();
}

void Scene::AddGameObject(GameObject* go)
{
	m_GameObjects.push_back(go);
}

void Scene::AddCamera(Camera* cam)
{
	m_Cameras.push_back(cam);
}

void Scene::SetActiveCamera(Camera* cam)
{
	m_useCamera = cam;
}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (auto l : m_Lights)
	{
		l->SetRenderValues(_shaderprog);
	}
}

GLuint Scene::GetFlatColourShader() const
{
	return m_flatColourShader ? m_flatColourShader->GetProg() : 0;
}

inline std::string Trim(const std::string& str)
{
	const std::string whitespace = " \t\n\r\f\v";

	size_t start = str.find_first_not_of(whitespace);
	if (start == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(whitespace);
	return str.substr(start, end - start + 1);
}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	std::cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		std::cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);
		newCam->Load(_file);

		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	std::cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	std::cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');

		string type, line;
		std::streampos blockStart = _file.tellg();
		while (getline(_file, line))
		{
			if (line.find("TYPE:") != string::npos)
			{
				size_t colon = line.find(':');
				if (colon != string::npos)
				{
					type = line.substr(colon + 1);
					type = Trim(type);
					break;
				}
			}
			else if (line.find('}') != string::npos)
			{
				break;
			}
		}
		_file.clear();
		_file.seekg(blockStart);

		if (type.empty())
		{
			std::cout << "ERROR: Light type missing or blank." << i << "Skipping." << endl;
			_file.ignore(256, '}');
			continue;
		}
		
		std::cout << "Loaded light type: [" << type << "]" << endl;
		
		Light* newLight = LightFactory::makeNewLight(type);
		if (!newLight)
		{
			std::cout << "ERROR: Failed to create light of type " << type << endl;
			_file.ignore(256, '}');
			continue;
		}
		
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	std::cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	std::cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		std::cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Model* newModel = ModelFactory::makeNewModel(type);
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	std::cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	std::cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		std::cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	std::cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	std::cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		std::cout << "{\n";

		Shader* shader = new Shader(_file);
		m_Shaders.push_back(shader);

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	for (Shader* shader : m_Shaders)
	{
		string name = shader->GetName();
		std::cout << "[SCENE] Shader in list: " << name << " -> " << shader->GetProg() << endl;

		if (name == "SUNLIGHT")
			m_texDirLightShader = shader->GetProg();
		else if (name == "TEXPOINT")
			m_texPointLightShader = shader->GetProg();
	}

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	std::cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		std::cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		GameObject* newGO = GameObjectFactory::makeNewGO(type);
		newGO->Load(_file);

		if (type == "BACKGROUND" || newGO->GetName().find("SKY_") != std::string::npos)
		{
			newGO->SetRenderPass(RP_BACKGROUND);
		}
		else
		{
			newGO->SetRenderPass(RP_OPAQUE);
		}

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		std::cout << "}\n";
	}

	if (m_texDirLightShader == 0)
		std::cout << "[SCENE] SUNLIGHT shader not loaded.\n" << endl;

	if (m_texPointLightShader == 0)
		std::cout << "[SCENE] TEXPOINT shader not loaded.\n" << endl;
}

void Scene::ToggleBeast()
{
	m_showBeast = !m_showBeast;
	std::cout << "[SCENE] Beast Visibility: " << (m_showBeast ? "ON" : "OFF") << endl;
}
