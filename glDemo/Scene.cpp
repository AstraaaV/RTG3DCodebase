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
#include <assert.h>
#include <helper.h>

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
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
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
	// Render all opaque objects first
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		// Only draw if object is marked as opaque
		if ((*it)->GetRP() & RP_OPAQUE)
		{
			GLuint shaderProg = (*it)->GetShaderProg();
			glUseProgram(shaderProg);

			m_useCamera->SetRenderValues(shaderProg);

			SetShaderUniforms(shaderProg);

			(*it)->PreRender();

			(*it)->Render();
		}
	}

	// Transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if ((*it)->GetRP() & RP_TRANSPARENT)
		{
			GLuint shaderProg = (*it)->GetShaderProg();
			glUseProgram(shaderProg);

			m_useCamera->SetRenderValues(shaderProg);
			SetShaderUniforms(shaderProg);
			(*it)->PreRender();
			(*it)->Render();
		}
	}

	// Restore settings for next frame
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

	GLint texLoc;
	if (Helper::SetUniformLocation(_shaderprog, "tex", &texLoc))
		glUniform1i(texLoc, 0);
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

void Scene::Init()
{
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "OVERVIEW")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		else if ((*it)->GetName() == "FIRSTPERSONCAMERA")
		{
			
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = GetCamera("FIRSTPERSONCAMERA");
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}

	std::cout << "[DEBUG] LOADING MAP...\n";
	LoadMap();
}

void Scene::CycleCameras()
{
	if (m_Cameras.empty()) return;

	auto it = std::find(m_Cameras.begin(), m_Cameras.end(), m_useCamera);

	if (it != m_Cameras.end() && ++it != m_Cameras.end())
		m_useCamera = *it;
	else
		m_useCamera = m_Cameras.front();

	std::cout << "Switched to camera: " << m_useCamera->GetName() << std::endl;
}

void Scene::LoadMap()
{
	m_mapLayout =
	{
		"WWWWWWWWWW",
		"W..T.....W",
		"W..D..P..W",
		"W........W",
		"WWWWWWWWWW"
	};

	float tileSize = 1.0f;
	int numRows = static_cast<int>(m_mapLayout.size());

	for (int z = 0; z < numRows; ++z)
	{
		const std::string& row = m_mapLayout[z];
		int flipZ = numRows - 1 - z;

		for (int x = 0; x < row.size(); ++x)
		{
			char tile = row[x];
			vec3 pos = vec3(x * tileSize, 0.0f, flipZ * tileSize);

			if (tile == 'W')
			{
				GameObject* wall = new GameObject();

				wall->SetName("WALL_" + std::to_string(x) + "_" + std::to_string(z));
				wall->SetModel(GetModel("CUBE"));
				wall->SetTexture(GetTexture("WALL_DIFFUSE"));
				wall->SetShader(GetShader("TEXDIR"));
				wall->SetPos(pos);
				wall->SetScale(vec3(1.0f));

				AddGameObject(wall);
			}

			else if (tile == 'T')
			{
				GameObject* torch = new GameObject();

				torch->SetName("TORCH_" + std::to_string(x) + "_" + std::to_string(z));
				torch->SetModel(GetModel("CUBE"));
				torch->SetTexture(GetTexture("ROCK"));
				torch->SetShader(GetShader("TEXDIR"));
				torch->SetPos(pos);
				torch->SetScale(vec3(0.5f));

				AddGameObject(torch);

				Light* point = new Light();
				point->SetName("TORCH_LIGHT_" + std::to_string(x) + "_" + std::to_string(z));
				point->SetPos(vec3(x * tileSize, 1.5f, flipZ * tileSize));
				point->SetColour(vec3(1.0f, 0.5f, 0.2f));
				point->SetAmbient(vec3(0.1f, 0.05f, 0.02f));

				m_Lights.push_back(point);
			}

			else if (tile == 'D')
			{
				GameObject* door = new GameObject();

				door->SetName("DOOR_" + std::to_string(x) + "_" + std::to_string(z));
				door->SetModel(GetModel("CUBE"));
				door->SetTexture(GetTexture("ROCK"));	door->SetShader(GetShader("FLAT"));
				door->SetPos(pos);
				door->SetScale(vec3(1.0f, 1.5f, 0.1f));

				AddGameObject(door);
			}

			else if (tile == 'P')
			{
				GameObject* beast = GetGameObject("BEAST");

				if (beast)
				{
					beast->SetPos(pos);
				}
			}
		}
	}
}
