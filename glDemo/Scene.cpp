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
#include "FirstPersonCamera.h"
#include "ArcballCamera.h"
#include "IsometricCamera.h"
#include "Map.h"
#include "Cube.h"
#include "Plane.h"
#include <assert.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naught and not deleting everything as we finish
	//what shoudl really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	if (!m_useCamera || !m_window)
		return;

	const float moveSpeed = 5.0f * _dt;
	const float rotateSpeed = 50.0f * _dt;

	if (FirstPersonCamera* fpc = dynamic_cast<FirstPersonCamera*>(m_useCamera))
	{
		if (m_beast)
		{
			glm::vec3 forward = glm::normalize(glm::vec3(fpc->GetForward().x, 0.0f, fpc->GetForward().z));
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

			glm::vec3 currentPos = m_beast->GetPosition();
			glm::vec3 newPos = currentPos;

			if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
				newPos += forward * moveSpeed;
			if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
				newPos -= forward * moveSpeed;
			if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
				newPos += right * moveSpeed;
			if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
				newPos -= right * moveSpeed;

			int x = static_cast<int>(newPos.x + 0.5f);
			int z = static_cast<int>(newPos.z + 0.5f);

			if (!m_map->IsWall(x, z))
			{
				m_beast->SetPosition(newPos);
			}

			glm::vec3 beastPos = m_beast->GetPosition() + glm::vec3(0.0f, 0.8f, 0.0f);
			glm::vec3 offset(0.0f, 0.8f, 0.0f);
			fpc->SetPos(beastPos + offset);

			float yaw = fpc->GetYaw();
			m_beast->SetRotation(glm::vec3(0.0f, yaw, 0.0f));
		}
	}
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
	//TODO: Set up for the Opaque Render Pass will go here
	//check out the example stuff back in main.cpp to see what needs setting up here
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetRP() & RP_OPAQUE)// TODO: note the bit-wise operation. Why?
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	std::vector<GameObject*> transparentObjects;

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if ((*it)->GetRP() & RP_TRANSPARENT)// TODO: note the bit-wise operation. Why?
		{
			transparentObjects.push_back(*it);
		}
	}

	std::sort(transparentObjects.begin(), transparentObjects.end(),
		[this](GameObject* a, GameObject* b)
		{
			glm::vec3 camPos = m_useCamera->GetPos();
			float distA = glm::length(camPos - a->GetPosition());
			float distB = glm::length(camPos - b->GetPosition());
			return distA > distB;
		}
	);

	for (GameObject* obj : transparentObjects)
	{
		GLuint SP = obj->GetShaderProg();
		glUseProgram(SP);

		m_useCamera->SetRenderValues(SP);
		SetShaderUniforms(SP);
		obj->PreRender();
		obj->Render();
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
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

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}


}

void Scene::Init(GLFWwindow* window)
{
	m_window = window;

	for (Camera* cam : m_Cameras)
	{
		if (cam)
		{
			cam->Init(800.0f, 600.0f, this);
			if (cam->GetType() == "FPC")
				m_firstPersonC = cam;
		}
	}

	m_map = new Map(this);
	m_map->Init();
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
			glm::vec3 spawn = m_map->GetPlayerSpawn();

			if (FirstPersonCamera* fpc = dynamic_cast<FirstPersonCamera*>(m_useCamera))
				fpc->SetPos(spawn + glm::vec3(0.0f, 1.8f, 0.0f));

			else if (ArcballCamera* arc = dynamic_cast<ArcballCamera*>(m_useCamera))
			{
				arc->SetTarget(spawn);
				arc->SetRadius(10.0f);
			}

			else if (IsometricCamera* iso = dynamic_cast<IsometricCamera*>(m_useCamera))
			{
				iso->SetFocus(spawn);
				iso->SetZoom(15.0f);
			}
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

	Cube* cube = new Cube();
	cube->SetName("CUBE");
	m_Models.push_back(new Cube());

	Model* floorModel = new Plane();
	floorModel->SetName("FLOOR");
	m_Models.push_back(floorModel);

	GameObject* beast = GetGameObject("BEAST");
	if (beast)
	{
		for (Camera* cam : m_Cameras)
		{
			if (cam && cam->GetType() == "FPC")
			{
				if (FirstPersonCamera* fpc = dynamic_cast<FirstPersonCamera*>(cam))
				{
					fpc->SetTarget(beast);
					fpc->SetPos(beast->GetPosition() + glm::vec3(0.0f, 1.8f, 0.0f));
					m_useCamera = fpc;
					m_useCameraIndex = 0;
				}
			}
		}
	}

	PossessBeast();
}

void Scene::CycleCams()
{
	if (m_Cameras.empty()) return;

	m_useCameraIndex++;
	if (m_useCameraIndex >= m_Cameras.size())
	{
		m_useCameraIndex = 0;
	}

	if (ArcballCamera* arc = dynamic_cast<ArcballCamera*>(m_useCamera))
	{
		arc->SetRadius(10.0f);
	}

	int count = 0;
	for (Camera* cam : m_Cameras)
	{
		if (count == m_useCameraIndex)
		{
			m_useCamera = cam;
			std::cout << "Switched to cam: \n" << cam->GetName() << std::endl;
			break;
		}
		count++;
	}
}

void Scene::PossessBeast()
{
	if (m_possessBeast)
	{
		if (m_prevCam)
		{
			m_useCamera = m_prevCam;
			m_prevCam = nullptr;
			m_possessBeast = false;

			if (m_beast)
				m_beast->SetVisible(true);

			cout << "Beast unpossessed.\n";
		}
		return;
	}

	GameObject* beast = GetGameObject("BEAST");
	if (!beast)
	{
		cout << "Beast not found!\n";
		return;
	}

	for (Camera* cam : m_Cameras)
	{
		if (cam && cam->GetType() == "FPC")
		{
			m_prevCam = m_useCamera;
			m_useCamera = cam;
			m_possessBeast = true;
			m_beast = beast;
			
			if (FirstPersonCamera* fpc = dynamic_cast<FirstPersonCamera*>(m_useCamera))
			{
				glm::vec3 beastPos = beast->GetPosition();
				glm::vec3 offset(0.0f, 1.8f, 0.0f);
				fpc->SetPos(beastPos + offset);

				glm::vec3 beastRot = beast->GetRotation();
				fpc->SetYaw(beastRot.y);

				fpc->SetTarget(beast);
			}

			m_beast->SetVisible(false);

			cout << "Beast possessed.\n";
			break;
		}
	}
}

void Scene::AddGameObject(GameObject* obj)
{
	m_GameObjects.push_back(obj);
}
