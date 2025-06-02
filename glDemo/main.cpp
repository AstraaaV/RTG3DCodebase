#include "Scene.h"
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "FirstPersonCamera.h"
#include "IsometricCamera.h"
#include "GUClock.h"
#include "shader_setup.h"
#include "helper.h"

using namespace std; // Allows cleaner syntax like 'cout' instead of 'std::cout'
using namespace glm; // Same thing as above, no need for 'glm::'

#pragma region Global variables

GUClock* g_gameClock = nullptr; // Used to track frame timing/calculate delta time
GLFWwindow* g_window = nullptr;

// Mouse input tracking, mainly for controlling cam
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;
bool				g_camSwitchPressed = false; // Prevents rapid camera cycling
bool				g_lightsEnabled = true; // Toggles lighting on/off - more for debug

// Main Scene object that handles rendering, updates, and cam switching
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

#pragma endregion

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);
	g_window = window;

	// Check window was created successfully
	if (!window)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Locks/hides mouse cursor
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // setup background colour to be black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glPolygonMode(GL_FRONT, GL_FILL); //Normal solid render
	glPolygonMode(GL_BACK, GL_LINE); // Draws back faces as wireframe (debugging)
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	// Initialise scene, loads models/lights/cams/shaders,etc
	g_Scene = new Scene();

	ifstream manifest("manifest.txt");
	if (!manifest.is_open())
	{
		std::cout << "Failed to open manifest file!\n";
		return -1;
	}
	g_Scene->Load(manifest);
	manifest.close();
	g_Scene->Init();

	//
	//Set up Scene class
	//

	if (!g_Scene->GetActiveCamera())
	{
		std::cout << "No active camera set after Init." << endl;
	}
	else
	{
		std::cout << "Active camera is: " << g_Scene->GetActiveCamera()->GetName() << endl;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Everything drawn for both sides of geometry

	//
	// Main loop
	// 

	float g_deltaTime = 0.0f; // Declares global deltaTime
	float lastFrame = 0.0f; // Stores time of last frame

	// Keeps running until player closes window
	while (!glfwWindowShouldClose(window))
	{
		// Calculate frame timing
		float currentFrame = glfwGetTime();
		g_deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Updates everything (timing, etc)
		updateScene(window);
		 // Calls player input stuff
		processKeys(window, (float)g_gameClock->gameTimeDelta());
		// Renders it all to screen
		g_Scene->Render();
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// Update window title
		char timingString[256];
		float fps = g_gameClock->averageFPS();
		float spf = g_gameClock->averageSPF();
		sprintf_s(timingString, 256, "Dungeon Keeper 2 Clone | GDV5001 | FPS: %.1f | SPF: %.4f", fps, spf);
		glfwSetWindowTitle(window, timingString);
	}

	// Once main loop exits, clean it up
	glfwTerminate();

	// Stop clock and print timing stats (more for diagnostics)
	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}
}

// Handles movement controls for current active cam
void processKeys(GLFWwindow* window, float deltaTime)
{	
	// Handle movement only on FirstPersonCamera
	Camera* activeCam = g_Scene->GetActiveCamera();
	FirstPersonCamera* fpCam = dynamic_cast<FirstPersonCamera*>(activeCam);
	if (!fpCam)
	{
		static int frameCounter = 0;
		if (frameCounter++ % 300 == 0)
		{
			std::cout << "Active cam is NOT FPC\n";
		}
		return;
	}
	
	// Stores last cam pos (for debug)
	static glm::vec3 lastPos = fpCam->getPosition();

	// Movement controls (WASD + QE for vertical)
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

	// Only print if cam has moved (also good for grey screen error...)
	glm::vec3 currentPos = fpCam->getPosition();
	if (currentPos != lastPos)
	{
		std::cout << "Cam moved to: " << currentPos.x << ", " << currentPos.y << ", " << currentPos.z << endl;
		lastPos = currentPos;
	}
}

// Function called to animate elements in the scene
void updateScene(GLFWwindow* window) 
{
	// Ticks clock to see how much time has passed since last frame
	if (g_gameClock)
	{
		g_gameClock->tick();
		float tDelta = (float)g_gameClock->gameTimeDelta();
		g_Scene->Update(tDelta, g_window);
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
	
	// Updates active cam aspect ratio to match window size
	if (g_Scene && g_Scene->GetActiveCamera())
	{
		g_Scene->GetActiveCamera()->SetAspect((float)_width / (float)_height);
	}

	// Forces update after resizing
	if (g_Scene)
	{
		g_Scene->Update(0.0f, _window);
	}

	glViewport(0, 0, _width, _height);		// Draw into entire window
}

// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_PRESS) {

		// Check which key was pressed...
		switch (_key)
		{
		case GLFW_KEY_ESCAPE: // Close window
			glfwSetWindowShouldClose(_window, true);
			break;

		case GLFW_KEY_C:
			if (!g_camSwitchPressed)
			{
				// Switch camera
				g_Scene->CycleCams();

				Camera* cam = g_Scene->GetActiveCamera();
				if (cam)
				{
					std::string name = cam->GetName();
					std::cout << "Cam switched.\n" << g_Scene->GetActiveCamera()->GetName() << endl;
				}
				else
				{
					std::cout << "Error. Active camera is null!" << endl;
				}
				g_camSwitchPressed = true;
			}
			break;

		case GLFW_KEY_L:
			// Toggle lights on and off
			g_lightsEnabled = !g_lightsEnabled;
			g_Scene->SetLightsEnabled(g_lightsEnabled);
			std::cout << "Lights enabled: " << (g_lightsEnabled ? "On" : "Off") << endl;
			break;
		}
	}
	// Allow cam switching again once C key is released
	else if(_action == GLFW_RELEASE)
	{
		if (_key == GLFW_KEY_C)
			g_camSwitchPressed = false;
	}
}

// Handles mouse movement aka rotates FPC view
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos) 
{
	static bool fpMouse = true;

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

	// Calculates mouse movement since last frame
	float dx = static_cast<float>(_xpos - g_prevMouseX);
	float dy = static_cast<float>(_ypos - g_prevMouseY);
	g_prevMouseX = _xpos;
	g_prevMouseY = _ypos;

	// Updates yaw/pitch for mouse and sensitivity
	float yaw = fpCam->getYaw() + dx * fpCam->getSensitivity();
	float pitch = fpCam->getPitch() - dy * fpCam->getSensitivity();

	pitch = glm::clamp(pitch, -89.0f, 89.0f); // Avoids flipping cam upside down

	fpCam->setYaw(yaw);
	fpCam->setPitch(pitch);
}

// Detects left mouse clicking (more for future interaction)
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

// Handles mouse scroll (arcball cam zooms in and out)
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset) {

	Camera* activeCam = g_Scene->GetActiveCamera();
	ArcballCamera* arcCam = dynamic_cast<ArcballCamera*>(activeCam);

	if (arcCam)
	{
		// Zoom out
		if (_yoffset < 0.0)
			arcCam->scaleRadius(1.1f);
		// Zoom in
		else if (_yoffset > 0.0)
			arcCam->scaleRadius(0.9f);
	}
}
// Currently unused - could be used to pause game?
void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion