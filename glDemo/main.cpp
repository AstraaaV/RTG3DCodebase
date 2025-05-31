#include "Scene.h"
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

using namespace std;
using namespace glm;

#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;
bool				g_camSwitchPressed = false;
bool g_lightsEnabled = true;

int g_showing = 1;
int g_NumExamples = 3;
int g_currentCam = 1;
int g_NumCams = 3;
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

#pragma endregion

GLFWwindow* g_window = nullptr;

// Function prototypes
void processKeys(GLFWwindow* window, float deltaTime);
void updateScene(GLFWwindow* window);
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);

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

	g_Scene = new Scene();
	g_Scene->Init();

	//
	//Set up Scene class
	//

	g_showing = 2;

	if (!g_Scene->GetActiveCamera())
	{
		cout << "No active camera set after Init." << endl;
	}
	else
	{
		cout << "Active camera is: " << g_Scene->GetActiveCamera()->GetName() << endl;
	}

	bool camSpawnSet = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


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

		g_Scene->Render();
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		float fps = g_gameClock->averageFPS();
		float spf = g_gameClock->averageSPF();
		sprintf_s(timingString, 256, "Dungeon Keeper 2 Clone | GDV5001 | FPS: %.1f | SPF: %.4f", fps, spf);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
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
}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

// Function to call when window resized
void resizeWindow(GLFWwindow* _window, int _width, int _height)
{
	if (_height == 0) _height = 1;

	float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
	
	if (g_Scene && g_Scene->GetActiveCamera())
	{
		g_Scene->GetActiveCamera()->SetAspect((float)_width / (float)_height);
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
			g_showing = (g_showing + 1) % g_NumExamples;
			break;

		case GLFW_KEY_C:
			if (!g_camSwitchPressed)
			{
				g_Scene->CycleCams();
				cout << "Cam switched.\n" << g_Scene->GetActiveCamera()->GetName() << endl;
				g_camSwitchPressed = true;
			}
			break;

		case GLFW_KEY_L:
			g_lightsEnabled = !g_lightsEnabled;
			g_Scene->SetLightsEnabled(g_lightsEnabled);
			cout << "Lights enabled: " << (g_lightsEnabled ? "On" : "Off") << endl;
			break;
		}
	}
	else if(_action == GLFW_RELEASE)
	{
		if (_key == GLFW_KEY_C)
			g_camSwitchPressed = false;
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

	if (!g_Scene) return;

	Camera* activeCam = g_Scene->GetActiveCamera();
	ArcballCamera* arcCam = dynamic_cast<ArcballCamera*>(activeCam);

	if (arcCam)
	{
		if (_yoffset < 0.0)
			arcCam->scaleRadius(1.1f);
		else if (_yoffset > 0.0)
			arcCam->scaleRadius(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion