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
#include "Player.h"
#include <assert.h>
#include <helper.h>
#include <RenderPass.h>
#include "IsometricCamera.h"
#include "TextureLoader.h"

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
	if (m_player) { delete m_player; m_player = nullptr; }
}

void Scene::Init()
{
	GLuint wallTex = loadTexture("Assets\\Textures\\rock_wall.JPG", FIF_JPEG);

	if (wallTex == 0)
	{
		cout << "Failed to load the wall texture." << endl;
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
		cout << "Failed to open manifest.txt\n" << endl;
	}

	m_cube = new Cube();
	BuildMap();

	m_player = new Player();
	m_player->SetShader(m_texDirLightShader);
	AddGameObject(m_player);

	m_beast = new Beast();
	m_beast->SetShader(m_texDirLightShader);
	AddGameObject(m_beast);

	cout << "Assigned TEXDIR = " << m_texDirLightShader << "\n";
	cout << "Assigned TEXPOINT = " << m_texPointLightShader << "\n";

	m_fpCamera = new FirstPersonCamera();
	m_fpCamera->setPosition(glm::vec3(2.2f * 5, 1.5f, 2.2f * 5));
	m_fpCamera->setYaw(-90.0f);
	m_fpCamera->setPitch(-30.0f);
	AddCamera(m_fpCamera);
	SetActiveCamera(m_fpCamera);

	// Arcball camera
	m_arcCamera = new ArcballCamera();
	AddCamera(m_arcCamera);

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
	InitTorches();
}

void Scene::InitTorches()
{
	m_torchPos.clear();
	for (int row = 0; row < m_mapLayout.size(); ++row)
	{
		for (int col = 0; col < m_mapLayout[row].length(); ++col)
		{
			if (m_mapLayout[row][col] == 'T')
			{
				float x = col * 2.2f, y = 2.5f, z = row * 2.2f;
				glm::vec3 torchPos(x, y, z);
				m_torchPos.push_back(torchPos);

				Light* point = LightFactory::makeNewLight("POINT");
				point->SetPosition(torchPos);
				point->SetDiffuse(glm::vec3(1.0f, 0.8f, 0.5f));
				point->SetAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
				point->SetSpecular(glm::vec3(1.0f));
				point->SetName("TorchLight_" + std::to_string(m_Lights.size()));

				m_Lights.push_back(point);
			}
		}
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

	glm::vec3 playerPos = m_useCamera->GetPos();
	int row = static_cast<int>(playerPos.z / 2.2f);
	int col = static_cast<int>(playerPos.x / 2.2f);

	if (row >= 0 && row < m_mapLayout.size() &&
		col >= 0 && col < m_mapLayout[row].length())
	{
		char tile = m_mapLayout[row][col];

		if (tile == 'P')
		{
			printf("Player stepped on pressure plate at [%d,%d]!\n", row, col);
		}
	}

	if (m_player)
		InteractionTriggers(m_player->GetPosition());

	for (auto go : m_GameObjects)
	{
		if (go->GetName() == "Beast" && !dynamic_cast<FirstPersonCamera*>(m_useCamera))
			continue;

		go->Tick(_dt, window);
	}
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
		cout << "Switched to cam: " << m_useCamera->GetName() << endl;
	}
	else
	{
		cout << "Camera switch failed." << endl;
	}
}

//Render Everything
void Scene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cout << "TEXDIR: " << m_texDirLightShader << " | TEXPOINT: " << m_texPointLightShader << endl;
	
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
		cout << "ERROR: m_cube is null!" << endl;
		return;
	}

	GLint pLocation;

	if (shaderProgram == 0)
	{
		cout << "ERROR: Shader program is 0!" << endl;
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
	if (!m_cube) return;

	if (shaderProgram == 0)
	{
		cout << "ERROR: Shader program is 0!" << endl;
		return;
	}

	glUseProgram(shaderProgram);

	GLint pLocation;
	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projMatrix[0][0]);

	std::vector<glm::vec3> positions, colours, ambients;

	for (Light* light : m_Lights)
	{
		if (light->GetType() == "POINT")
		{
			positions.push_back(light->GetPosition());
			colours.push_back(light->GetColour());
			ambients.push_back(light->GetAmbient());
		}
	}

		int count = static_cast<int>(positions.size());
		if (count > 16) count = 16;

		if (count > 0)
		{
			glUniform1i(glGetUniformLocation(shaderProgram, "numPointLights"), count);
			glUniform3fv(glGetUniformLocation(shaderProgram, "pointPos"), count, glm::value_ptr(positions[0]));
			glUniform3fv(glGetUniformLocation(shaderProgram, "pointCol"), count, glm::value_ptr(colours[0]));
			glUniform3fv(glGetUniformLocation(shaderProgram, "ambientCol"), count, glm::value_ptr(ambients[0]));
		}
	
		Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);
		for (const auto& pos : m_torchPos)
		{
			mat4 model = glm::translate(mat4(1), pos) * glm::scale(mat4(1), glm::vec3(0.3f));
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, &model[0][0]);
		
			m_cube->render();
		}
}

void Scene::InteractionTriggers(glm::vec3 playerPos)
{
	static float lastTriggerTime = 0.0f;
	float currentTime = glfwGetTime();

	if (currentTime - lastTriggerTime < 0.5f) return;

	int col = static_cast<int>(playerPos.x / 2.2f);
	int row = static_cast<int>(playerPos.z / 2.2f);

	if (row >= 0 && row < m_mapLayout.size() && col >= 0 && col < m_mapLayout[row].size())
	{
		char tile = m_mapLayout[row][col];

		if (tile == 'P')
		{
			cout << "Pressure Plate Activated at (" << row << "," << col << ")" << endl;

			std::vector<std::pair<int, int>> directions =
			{
				{-1, 0}, {1, 0}, {0, -1}, {0, 1}
			};

			for (auto& dir : directions)
			{
				int r = row + dir.first;
				int c = col + dir.second;

				if (r >= 0 && r < m_mapLayout.size() && c >= 0 && c < m_mapLayout[r].size())
				{
					if (m_mapLayout[r][c] == 'D')
					{
						m_mapLayout[r][c] = '.';
						cout << "Door opened at: " << r << ", " << c << endl;
						break;
					}
					else if (m_mapLayout[r][c] == 'T')
					{
						cout << "Torch enabled near the plate." << endl;
					}
				}
			}
			lastTriggerTime = currentTime;
		}
		else if (tile == 'T')
		{
			cout << "You feel warmth... A torch ignites near you." << endl;
		}
		else if (tile == 'W')
		{
			cout << "You bumped into a wall." << endl;
		}
	}
}

void Scene::RenderCreature()
{
	if (m_beast && m_showBeast)
		m_beast->Render();
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

static std::string Trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == string::npos) return "";

	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);
		newCam->Load(_file);

		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');

		string type, line;

		getline(_file, line);
		size_t colon = line.find(':');
		if (colon != string::npos)
			type = Trim(line.substr(colon + 1));

		if (type.empty())
		{
			cout << "ERROR: Light type missing or blank." << i << "Skipping." << endl;
			_file.ignore(256, '}');
			continue;
		}
		
		cout << "Loaded light type: [" << type << "]" << endl;
		
		Light* newLight = LightFactory::makeNewLight(type);
		if (!newLight)
		{
			cout << "ERROR: Failed to create light of type " << type << endl;
			_file.ignore(256, '}');
			continue;
		}
		
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Model* newModel = ModelFactory::makeNewModel(type);
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		Shader* shader = new Shader(_file);
		m_Shaders.push_back(shader);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	for (Shader* shader : m_Shaders)
	{
		string name = shader->GetName();
		cout << "[SCENE] Shader in list: " << name << " -> " << shader->GetProg() << endl;

		if (name == "SUNLIGHT")
			m_texDirLightShader = shader->GetProg();
		else if (name == "TEXPOINT")
			m_texPointLightShader = shader->GetProg();
	}

	cout << "m_texDirLightShader: " << m_texDirLightShader << endl;
	cout << "m_texPointLightShader: " << m_texPointLightShader << endl;
	cout << endl << endl;

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

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
		cout << "}\n";
	}

	if (m_texDirLightShader == 0)
		cout << "[SCENE] SUNLIGHT shader not loaded.\n" << endl;

	if (m_texPointLightShader == 0)
		cout << "[SCENE] TEXPOINT shader not loaded.\n" << endl;
}

void Scene::ToggleBeast()
{
	m_showBeast = !m_showBeast;
	cout << "[SCENE] Beast Visibility: " << (m_showBeast ? "ON" : "OFF") << endl;
}
