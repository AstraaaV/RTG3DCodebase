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
#include <assert.h>
#include <helper.h>
#include <RenderPass.h>

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
	m_cube = new Cube();
	BuildMap();

	m_creatureMesh = new AIMesh("Assets\\beast\\beast.obj");
	if (m_creatureMesh)
	{
		m_creatureMesh->addTexture("Assets\\beast\\beast_texture.bmp", FIF_BMP);
	}

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
	GenerateTorchPos();
}

void Scene::GenerateTorchPos()
{
	m_torchPos.clear();
	for (int row = 0; row < m_mapLayout.size(); ++row)
	{
		for (int col = 0; col < m_mapLayout[row].length(); ++col)
		{
			if (m_mapLayout[row][col] == 'T')
			{
				float x = col * 2.2f, y = 2.5f, z = row * 2.2f;
				m_torchPos.push_back(glm::vec3(x, y, z));
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
	for (auto cam : m_Cameras) cam->Tick(_dt, window);
	for (auto go : m_GameObjects) go->Tick(_dt, window);
}

void Scene::CycleCams()
{
	if (m_Cameras.empty()) return; //Stop cycle if cams dont exist
	
	m_useCameraIndex = (m_useCameraIndex + 1) % m_Cameras.size();
	auto it = m_Cameras.begin(); // Update active cam
	advance(it, m_useCameraIndex);
	m_useCamera = *it;

	cout << "Switched to Cam: " << m_useCamera->GetName() << endl;
}

//Render Everything
void Scene::Render()
{
	if (!m_useCamera) return;

	mat4 view = m_useCamera->GetView();
	mat4 proj = m_useCamera->GetProj();

	RenderMapLayout(m_texDirLightShader, view, proj);
	RenderTorches(m_texPointLightShader, view, proj, m_pointLightPosition, m_pointLightColour, m_pointLightAmbient);
	RenderCreature(m_texDirLightShader);
}

void Scene::RenderMapLayout(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection)
{
	if (!m_cube) return;

	GLint pLocation;
	glUseProgram(shaderProgram);

	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &view[0][0]);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projection[0][0]);
	
	Helper::SetUniformLocation(shaderProgram, "DIRDir", &pLocation);
	glUniform3fv(pLocation, 1, value_ptr(m_dirLightDirection));
	Helper::SetUniformLocation(shaderProgram, "DIRCol", &pLocation);
	glUniform3fv(pLocation, 1, value_ptr(m_dirLightColour));
	Helper::SetUniformLocation(shaderProgram, "DIRAmb", &pLocation);
	glUniform3fv(pLocation, 1, value_ptr(m_dirLightAmbient));

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

			default:
				continue;
			}
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, &model[0][0]);
			m_cube->render();
		}
	}
}

void Scene::RenderTorches(GLuint shaderProgram, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const glm::vec3& lightPos, const glm::vec3& lightCol, const glm::vec3& ambientCol)
{
	if (!m_cube) return;

	GLint pLocation;
	glUseProgram(shaderProgram);

	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &projMatrix[0][0]);

	Helper::SetUniformLocation(shaderProgram, "pointPos", &pLocation);
	glUniform3fv(pLocation, 1, &lightPos[0]);
	Helper::SetUniformLocation(shaderProgram, "pointCol", &pLocation);
	glUniform3fv(pLocation, 1, &lightCol[0]);
	Helper::SetUniformLocation(shaderProgram, "ambientCol", &pLocation);
	glUniform3fv(pLocation, 1, &ambientCol[0]);

	for (const auto& pos : m_torchPos)
	{
		mat4 model = glm::translate(mat4(1), pos) * glm::scale(mat4(1), vec3(0.3f, 0.3f, 0.3f));
		Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &model[0][0]);

		m_cube->render();
	}
}

void Scene::RenderCreature(GLuint shaderProgram)
{
	if (!m_creatureMesh) return;

	GLint pLocation;
	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);
	mat4 transform = translate(mat4(1), vec3(11, 0, 11)) * eulerAngleY(radians(180.0f));
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &transform[0][0]);

	m_creatureMesh->setupTextures();
	m_creatureMesh->render();
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
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Light* newLight = LightFactory::makeNewLight(type);
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

		m_Shaders.push_back(new Shader(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

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
}
