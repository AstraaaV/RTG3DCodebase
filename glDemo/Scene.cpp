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

	if (m_cube)
	{
		delete m_cube;
		m_cube = nullptr;
	}
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		if (!m_camSwitchPressed)
		{
			CycleCams();
			m_camSwitchPressed = true;
		}
	}
	else
	{
		m_camSwitchPressed = false;
	}

	//update all lights
	for (auto it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt, window);
	}

	//update all cameras
	for (auto it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt, window);
	}

	//update all GameObjects
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt, window);
	}
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

void Scene::CycleCams()
{
	if (m_Cameras.empty()) return; //Stop cycle if cams dont exist
	
	m_useCameraIndex = (m_useCameraIndex + 1) % m_Cameras.size();

	// Update active cam
	auto it = m_Cameras.begin();
	advance(it, m_useCameraIndex);
	
	if (m_useCamera)
		cout << "Switched from Cam: " << m_useCamera->GetName();
	else
		cout << "Switch from Cam: (none)" << (*it)->GetName() << endl;

	m_useCamera = *it;

	cout << " to Cam: " << m_useCamera->GetName() << endl;
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}

Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

Model* Scene::GetModel(string _modelName)
{
	for (list<Model*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


//Render Everything
void Scene::Render()
{
	// Render background objects
	glDepthMask(GL_FALSE);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);

	for (auto gameObject : m_GameObjects)
	{
		if (gameObject->GetRP() & RP_BACKGROUND)
		{
			std::cout << "Rendering: " << gameObject->GetName() << std::endl;
			GLuint SP = gameObject->GetShaderProg();
			glUseProgram(SP);

			m_useCamera->SetRenderValues(SP);
			SetShaderUniforms(SP);
			gameObject->PreRender();
			gameObject->Render();
		}
	}
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	// Render Opaque objects

	//check out the example stuff back in main.cpp to see what needs setting up here
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		// The bitwise operation checks if the object is set to be rendered in opaque pass
		if ((*it)->GetRP() & RP_OPAQUE)
		{
			//set shader program using
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			//set up for uniform shader values for current camera
			m_useCamera->SetRenderValues(SP);

			//loop through setting up uniform shader values for anything else
			SetShaderUniforms(SP);

			//set any uniform shader values for the actual model
			(*it)->PreRender();

			//actually render the GameObject
			(*it)->Render();
		}
	}

	for (auto gameObject : m_GameObjects)
	{
		if (gameObject->GetRP() & RP_TRANSPARENT)
		{
			GLuint SP = gameObject->GetShaderProg();
			glUseProgram(SP);

			m_useCamera->SetRenderValues(SP);
			SetShaderUniforms(SP);
			gameObject->PreRender();
			gameObject->Render();
		}
	}
}

void Scene::RenderCreature(GLuint shaderProgram)
{
	if (!m_creatureMesh) return;

	GLint pLocation;

	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);
	mat4 modelTransform = glm::translate(identity<mat4>(), m_beastPos) * glm::eulerAngleY(glm::radians(m_beastRotation));
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

	m_creatureMesh->setupTextures();
	m_creatureMesh->render();
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
		mat4 modelTransform = glm::translate(identity<mat4>(), pos) *
			glm::scale(identity<mat4>(), vec3(0.3f, 0.3f, 0.3f));

		Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &modelTransform[0][0]);

		m_cube->render();
	}
}

void Scene::RenderFloorCeiling(GLuint shader, const glm::mat4& view, const glm::mat4& proj, GLuint texture)
{
	GLint pLocation;

	glUseProgram(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	Helper::SetUniformLocation(shader, "texture", &pLocation);
	glUniform1i(pLocation, 0);

	Helper::SetUniformLocation(shader, "lightsEnabled", &pLocation);
	glUniform1i(pLocation, m_lightsEnabled ? 1 : 0);

	Helper::SetUniformLocation(shader, "DIRDir", &pLocation);
	glUniform3fv(pLocation, 1, (GLfloat*)&m_dirLightDirection);
	Helper::SetUniformLocation(shader, "DIRCol", &pLocation);
	glUniform3fv(pLocation, 1, (GLfloat*)&m_dirLightColour);
	Helper::SetUniformLocation(shader, "DIRAmb", &pLocation);
	glUniform3fv(pLocation, 1, (GLfloat*)&m_dirLightAmbient);

	Helper::SetUniformLocation(shader, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&view);
	Helper::SetUniformLocation(shader, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&proj);

	Helper::SetUniformLocation(shader, "modelMatrix", &pLocation);

	// Floor
	for (int x = 0; x < 5; x++)
	{
		for (int z = 0; z < 5; z++)
		{
			glm::mat4 model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(x * 2.2f, -1.1f, z * 2.2f)) *
				glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f, 0.2f, 2.0f));
		
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&model);
			m_cube->render();
		}
	}

	// Ceiling
	for (int x = 0; x < 5; x++)
	{
		for (int z = 0; z < 5; z++)
		{
			glm::mat4 model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(x * 2.2f, 3.1f, z * 2.2f)) *
				glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f, 0.2f, 2.0f));

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&model);
			m_cube->render();
		}
	}
}

void Scene::RenderMapLayout(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection)
{
	if (!m_cube) return;

	GLint pLocation;

	glUseProgram(shaderProgram);
	Helper::SetUniformLocation(shaderProgram, "viewMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&view);
	Helper::SetUniformLocation(shaderProgram, "projMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&projection);

	Helper::SetUniformLocation(shaderProgram, "modelMatrix", &pLocation);

	for (int row = 0; row < m_mapLayout.size(); ++row)
	{
		for (int col = 0; col < m_mapLayout[row].length(); ++col)
		{
			char tile = m_mapLayout[row][col];
			float x = col * 2.2f;
			float z = row * 2.2f;

			glm::mat4 modelTransform;

			switch (tile)
			{
			case 'W': // Wall
				modelTransform = glm::translate(identity<mat4>(), vec3(x, 1.0f, z)) *
					glm::scale(identity<mat4>(), vec3(2.0f, 4.0f, 2.0f));
				break;

			case 'T': // Torch
				modelTransform = glm::translate(identity<mat4>(), vec3(x, 1.5f, z)) *
					glm::scale(identity<mat4>(), vec3(0.3f, 2.0f, 0.3f));
				break;

			case 'D': // Door
				modelTransform = glm::translate(identity<mat4>(), vec3(x, 1.0f, z)) *
					glm::scale(identity<mat4>(), vec3(1.0f, 2.0f, 0.3f));
				break;

			default:
				continue;
			}
			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
			m_cube->render();
		}
	}

}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->SetRenderValues(_shaderprog);
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

void Scene::Init()
{
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(512, 512, this);

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}

	// Create cube
	m_cube = new Cube();

	// Map layout 2d
	std::vector<std::string> g_mapLayout = {
		"WWWWWWWWWW",
		"W..T....PW",
		"W.WWWWW..W",
		"W.W....W.W",
		"W.W.WW.W.W",
		"W.W.WW.W.W",
		"W.W....W.W",
		"W.WWWWW..W",
		"W..D.....W",
		"WWWWWWWWWW"
	};

	GenerateTorchPos();

	m_torchPos.clear();

	for (int row = 0; row < g_mapLayout.size(); ++row)
	{
		for (int col = 0; col < g_mapLayout[row].length(); ++col)
		{
			if (g_mapLayout[row][col] == 'T')
			{
				float x = col * 2.2f;
				float y = 2.5f;
				float z = row * 2.2f;
				m_torchPos.push_back(glm::vec3(x, y, z));
			}
		}
	}

	m_creatureMesh = new AIMesh("Assets\\beast\\beast.obj");
	if (m_creatureMesh)
	{
		m_creatureMesh->addTexture("Assets\\beast\\beast_texture.bmp", FIF_BMP);
	}
}

void Scene::SetActiveCamera(Camera* cam)
{
	m_useCamera = cam;
}

void Scene::AddCamera(Camera* cam)
{
	m_Cameras.push_back(cam);
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
				float x = col * 2.2f;
				float y = 2.5f;
				float z = row * 2.2f;
				m_torchPos.push_back(glm::vec3(x, y, z));
			}
		}
	}
}
