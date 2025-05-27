
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "FirstPersonCamera.h"
#include "IsometricCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "shader_setup.h"
#include "helper.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Scene.h"


using namespace std;
using namespace glm;


#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;

bool				g_camSwitchPressed = false;

bool g_lightsEnabled = true;

// Texture handling
GLuint g_wallTex = 0;

// Global Example objects
// shouldn't really be anything in here for the final submission
ArcballCamera* g_mainCamera = nullptr;
FirstPersonCamera* g_fpCamera = nullptr;
IsometricCamera* g_isoCamera = nullptr;
CGPrincipleAxes* g_principleAxes = nullptr;

GLuint g_flatColourShader;

GLuint g_texDirLightShader;

GLuint g_texPointLightShader;

// Torch positions
std::vector<glm::vec3> torches =
{
	vec3(2.2f, 2.5f, 0.2f), // Back, middle-left
	vec3(6.6f, 2.5f, 0.2f), // Back, middle-right
	vec3(0.2f, 2.5f, 4.4f), // Left, center
	vec3(8.6f, 2.5f, 4.4f), // Right, center
	vec3(2.2f, 2.5f, 8.6f), // Front, left
	vec3(6.6f, 2.5f, 8.6f) // Front, right
};

int g_showing = 1;
int g_NumExamples = 3;

int g_currentCam = 1;
int g_NumCams = 3;

//Global Game Object
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

#pragma endregion


// Function prototypes
void renderScene();
void processKeys(GLFWwindow* window, float deltaTime);
void updateScene(GLFWwindow* window);
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);

GLFWwindow* g_window = nullptr;

int main()
{
	//
	// 1. Initialisation
	//

	g_gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);
	g_window = window;

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //hides cursor

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, g_initWidth, g_initHeight);

#pragma endregion

	// Initialise scene - geometry and shaders etc
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//
	// Setup the Example Objects
	//

	GLuint dirShader = setupShaders("Assets\\Shaders\\texture-directional.vert", "Assets\\Shaders\\texture-directional.frag");
	GLuint pointShader = setupShaders("Assets\\Shaders\\texture-pointlight.vert", "Assets\\Shaders\\texture-pointlight.frag");


	g_flatColourShader = setupShaders(string("Assets\\Shaders\\flatColour.vert"), string("Assets\\Shaders\\flatColour.frag"));
	g_wallTex = loadTexture("Assets\\Textures\\rock_wall.JPG", FIF_JPEG);

	if (g_wallTex == 0)
	{
		cout << "Texture failed to load." << endl;
	}

	g_mainCamera = new ArcballCamera(0.0f, 0.0f, 1.98595f, 55.0f, 1.0f, 0.1f, 500.0f);

	// First person camera
	g_fpCamera = new FirstPersonCamera();
	g_fpCamera->SetName("MAIN");
	g_fpCamera->setAspect((float)g_initWidth / g_initHeight);
	
	//
	//Set up Scene class
	//

	g_Scene = new Scene();
	g_Scene->SetWallTexture(g_wallTex);
	g_Scene->SetTexDirLightShader(dirShader);
	g_Scene->SetTexPointLightShader(pointShader);

	ifstream manifest;
	manifest.open("manifest.txt");

	g_Scene->Load(manifest);
	g_Scene->Init();

	g_showing = 1;

	manifest.close();

	if (!g_Scene->GetActiveCamera())
	{
		cout << "No active camera set after Init." << endl;
	}
	else
	{
		cout << "Active camera is: " << g_Scene->GetActiveCamera()->GetName() << endl;
	}
	
	g_Scene->AddCamera(g_fpCamera);
	g_Scene->SetActiveCamera(g_fpCamera);

	bool camSpawnSet = false;

	const std::vector<std::string>& map = g_Scene->GetMapLayout();

	for (int row = 0; row < map.size(); ++row)
	{
		for (int col = 0; col < map[row].length(); ++col)
		{
			if (map[row][col] == 'P')
			{
				float x = col * 2.2f;
				float z = row * 2.2f;
				g_fpCamera->setPosition(glm::vec3(5.0f, 10.0f, 15.0f));
				g_fpCamera->setYaw(-90.0f);
				g_fpCamera->setPitch(-30.0f);
				camSpawnSet = true;
				break;
			}
		}
	}

	// Spacing in map
	float spacing = 2.2f;
	glm::vec3 spawnPos = glm::vec3(1 * spacing, 0.0f, 1 * spacing);
	if (g_fpCamera) g_fpCamera->setPosition(glm::vec3(2.2f,1.5f,2.2f));

	// Isometric camera
	g_isoCamera = new IsometricCamera();
	g_isoCamera->setAspect((float)g_initWidth / g_initHeight);

	g_principleAxes = new CGPrincipleAxes();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//
	// Main loop
	// 

	float g_deltaTime = 0.0f; // Declares global deltaTime
	float lastFrame = 0.0f; // Stores time of last frame

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		g_deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		updateScene(window);

		processKeys(window, (float)g_gameClock->gameTimeDelta());

		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", g_gameClock->averageFPS(), g_gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}
}

// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* activeCam = g_Scene->GetActiveCamera();
	if (!activeCam) return;

	mat4 cameraProjection = activeCam->GetProj();
	mat4 cameraView = activeCam->GetView();
	mat4 cameraTransform = cameraProjection * cameraView;

	cameraTransform = cameraProjection * cameraView;

#// Render principle axes - no modelling transforms so just use cameraTransform
	if (true)
	{
		// Render axes 
		glUseProgram(g_flatColourShader);
		GLint pLocation;
		Helper::SetUniformLocation(g_flatColourShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_flatColourShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_flatColourShader, "modelMatrix", &pLocation);
		mat4 modelTransform = identity<mat4>();
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);

		g_principleAxes->render();
	}

	switch (g_showing)
	{
	case 0:
	{
		GLuint dirShader = g_Scene->GetTexDirLightShader();
		glUseProgram(dirShader);

		GLint pLocation;
		Helper::SetUniformLocation(g_texDirLightShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_texDirLightShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_texDirLightShader, "texture", &pLocation);
		glUniform1i(pLocation, 0); // set to point to texture unit 0 for AIMeshes
		Helper::SetUniformLocation(g_texDirLightShader, "DIRDir", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightDirection()));
		Helper::SetUniformLocation(g_texDirLightShader, "DIRCol", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightColour()));
		Helper::SetUniformLocation(g_texDirLightShader, "DIRAmb", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightAmbient()));

		g_Scene->RenderCreature(g_texDirLightShader);
	}
	break;

	case 1:
	{
		// Render cube 
		glUseProgram(g_texDirLightShader);

		GLint pLocation;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_Scene->GetWallTexture());

		Helper::SetUniformLocation(g_texDirLightShader, "texture", &pLocation);
		glUniform1i(pLocation, 0);

		// Lighting
		Helper::SetUniformLocation(g_texDirLightShader, "lightsEnabled", &pLocation);
		glUniform1i(pLocation, g_lightsEnabled ? 1 : 0);

		// Directional
		Helper::SetUniformLocation(g_texDirLightShader, "DIRDir", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightDirection()));
		Helper::SetUniformLocation(g_texDirLightShader, "DIRCol", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightColour()));
		Helper::SetUniformLocation(g_texDirLightShader, "DIRAmb", &pLocation);
		glUniform3fv(pLocation, 1, glm::value_ptr(g_Scene->GetDirLightAmbient()));

		// Matrices
		Helper::SetUniformLocation(g_texDirLightShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_texDirLightShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);

		Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);
		
		g_Scene->RenderFloorCeiling(g_texDirLightShader, cameraView, cameraProjection, g_wallTex);

		const std::vector<std::string>& map = g_Scene->GetMapLayout();

		g_Scene->RenderMapLayout(g_texDirLightShader, cameraView, cameraProjection);

		// Front wall (with door)
		/*for (int x = 0; x < 5; x++)
		{
			if (x == 2) continue; // gap for the door
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(x * 2.2f, 1.0f, 8.8f)) *
								  glm::scale(identity<mat4>(), vec3(2.0f, 4.0f, 0.3f));

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
			g_cube->render();
		}

		// Back wall
		for (int x = 0; x < 5; x++)
		{
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(x * 2.2f, 1.0f, 0.0f)) *
							 glm::scale(identity<mat4>(), vec3(2.0f, 4.0f, 0.3f));

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
			g_cube->render();
		}

		// Left wall
		for (int z = 0; z < 4; z++)
		{
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(0.0f, 1.0f, z * 2.2f)) *
								  glm::scale(identity<mat4>(), vec3(0.3f, 4.0f, 2.0f));

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
			g_cube->render();
		}

		// Right wall
		for (int z = 0; z < 4; z++)
		{
			mat4 modelTransform = glm::translate(identity<mat4>(), vec3(8.8f, 1.0f, z * 2.2f)) *
								  glm::scale(identity<mat4>(), vec3(0.3f, 4.0f, 2.0f));

			glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
			g_cube->render();
		}*/

		g_Scene->RenderTorches(g_texPointLightShader, cameraView, cameraProjection, g_Scene->GetPointLightPosition(), g_Scene->GetPointLightColour(), g_Scene->GetPointLightAmbient());

		//debug
		glUseProgram(g_texDirLightShader);
		Helper::SetUniformLocation(g_texDirLightShader, "viewMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraView);
		Helper::SetUniformLocation(g_texDirLightShader, "projMatrix", &pLocation);
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&cameraProjection);
		Helper::SetUniformLocation(g_texDirLightShader, "modelMatrix", &pLocation);

		FirstPersonCamera* fpCam = dynamic_cast<FirstPersonCamera*>(g_Scene->GetActiveCamera());
		glm::vec3 camPos = fpCam ? fpCam->getPosition() : glm::vec3(0);
		mat4 modelTransform = glm::translate(identity<mat4>(), camPos) *
							  glm::scale(identity<mat4>(), vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&modelTransform);
		g_Scene->GetCube()->render();

		break;
	}
	case 2:
		g_Scene->Render();
	}

}

void processKeys(GLFWwindow* window, float deltaTime)
{
	static bool debugOn = false;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		debugOn = !debugOn;

	Camera* activeCam = g_Scene->GetActiveCamera();
	FirstPersonCamera* fpCam = dynamic_cast<FirstPersonCamera*>(activeCam);
	if (!fpCam)
	{
		cout << "Active cam is NOT FPC\n";
		return;
	}

	static bool printed = false;
	if (!printed)
	{
		cout << "Using FPC for movement\n";
		printed = true;
	}
	
	static glm::vec3 lastPos = fpCam->getPosition();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		fpCam->processKeys(CameraMovement::UP, deltaTime);

	glm::vec3 currentPos = fpCam->getPosition();
	if (currentPos != lastPos)
	{
		cout << "Cam moved to: " << currentPos.x << ", " << currentPos.y << ", " << currentPos.z << endl;
		lastPos = currentPos;
	}

}

// Function called to animate elements in the scene
void updateScene(GLFWwindow* window) 
{
	float tDelta = 0.0f;

	if (g_gameClock) {

		g_gameClock->tick();
		tDelta = (float)g_gameClock->gameTimeDelta();
	}

	g_Scene->Update(tDelta, g_window);

	FirstPersonCamera* fpCam = dynamic_cast<FirstPersonCamera*>(g_Scene->GetActiveCamera());

	if (fpCam)
	{
		vec3 pos = fpCam->getPosition();
		pos.z += 0.1f;
		fpCam->setPosition(pos);
	}
}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

// Function to call when window resized
void resizeWindow(GLFWwindow* _window, int _width, int _height)
{
	if (_height == 0) _height = 1;

	float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
	
	if (g_mainCamera) {

		g_mainCamera->setAspect((float)_width / (float)_height);
	}

	if (g_Scene)
	{
		g_Scene->Update(0.0f, g_window);
	}

	glViewport(0, 0, _width, _height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_PRESS) {

		// check which key was pressed...
		switch (_key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, true);
			break;

		case GLFW_KEY_SPACE:
			g_showing++;
			g_showing = g_showing % g_NumExamples;
			break;

		case GLFW_KEY_C:
			if (_action == GLFW_PRESS && !g_camSwitchPressed)
			{
				g_Scene->CycleCams();
				g_camSwitchPressed = true;
			}
			else if (_action == GLFW_RELEASE)
			{
				g_camSwitchPressed = false;
			}

		case GLFW_KEY_L:
		{
			bool current = g_Scene->GetLightsEnabled();
			g_Scene->SetLightsEnabled(!current);
			cout << "Lights enabled: " << (!current ? "On" : "Off") << endl;
			break;
		}

		default:
		{
		}
		}
	}
	else if (_action == GLFW_RELEASE) 
	{
		// handle key release events
		switch (_key)
		{
		default:
		{
		}
		}
	}
}


void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos) 
{
	static int frameCount = 0;
	if (++frameCount % 30 == 0)
	{
		std::cout << "Mouse moved: " << _xpos << ", " << _ypos << std::endl;
	}

	static bool fpMouse = true;

	Camera* activeCam = g_Scene->GetActiveCamera();
	FirstPersonCamera* fpCam = dynamic_cast<FirstPersonCamera*>(g_Scene->GetActiveCamera());
	if (!fpCam)
	{
		std::cout << "Active Cam: " << (fpCam ? "FirstPersonCamera" : "Not using FP Cam.") << std::endl;
		return;
	}

	if (fpMouse)
	{
		g_prevMouseX = _xpos;
		g_prevMouseY = _ypos;
		fpMouse = false;
	}

	float dx = static_cast<float>(_xpos - g_prevMouseX);
	float dy = static_cast<float>(_ypos - g_prevMouseY);
	g_prevMouseX = _xpos;
	g_prevMouseY = _ypos;

	float yaw = fpCam->getYaw() + dx * fpCam->getSensitivity();
	float pitch = fpCam->getPitch() - dy * fpCam->getSensitivity();

	pitch = glm::clamp(pitch, -89.0f, 89.0f); // avoids flipping

	fpCam->setYaw(yaw);
	fpCam->setPitch(pitch);
}

void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods) 
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (_action == GLFW_PRESS) 
		{
			g_mouseDown = true;
			glfwGetCursorPos(_window, &g_prevMouseX, &g_prevMouseY);
		}
		else if (_action == GLFW_RELEASE) 
		{
			g_mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset) {

	if (g_mainCamera) 
	{
		if (_yoffset < 0.0)
			g_mainCamera->scaleRadius(1.1f);
		else if (_yoffset > 0.0)
			g_mainCamera->scaleRadius(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion