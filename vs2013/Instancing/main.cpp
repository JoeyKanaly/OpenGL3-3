//#define GLEW_STATIC
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <map>
//#include <sstream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <SOIL/SOIL.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <Camera.hpp>
//#include <Model.h>
//#include <Tools.h>
//
//GLuint screenShader;
//
//void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, 1);
//	/*if (key == GLFW_KEY_SPACE)
//	cam->Position.y += .01f;*/
//	if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
//		cam->Position.y -= .01f;
//	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
//		printf("%f, %f, %f", cam->Position.x, cam->Position.y, cam->Position.z);
//	/*if (key == GLFW_KEY_X && action == GLFW_RELEASE)
//	wireframe = !wireframe;*/
//	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
//	{
//		screenShader = compileShaders("../OpenGL3-3/shaders/advanced/fbo.vert.glsl", "../OpenGL3-3/shaders/advanced/fbo.frag.glsl");
//		std::cout << "Recompiled" << std::endl;
//	}
//}
//
//void mouse(GLFWwindow* window, double xpos, double ypos)
//{
//	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
//	cam->ProcessMouseMovement(xpos, ypos);
//
//}
//
//void mouseButtons(GLFWwindow* window, int button, int action, int mods)
//{
//
//}
//
//void scroll(GLFWwindow* window, double xOff, double yOff)
//{
//	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
//	cam->ProcessMouseScroll(yOff);
//	std::cout << "Mouse Scroll" << std::endl;
//}
//
//void enableSettings()
//{
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	//glEnable(GL_STENCIL_TEST);
//	//glEnable(GL_BLEND);
//	//glEnable(GL_PROGRAM_POINT_SIZE);
//}
//
//GLFWwindow* initWindow()
//{
//	//Initialize GLFW
//	glfwInit();
//
//	//Setup the window properties, to use OpenGL 3.3 and restrain it to the core profile
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//
//	//Create the window
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
//	if (window == nullptr)
//	{
//		std::cout << "Unable to create window." << std::endl;
//		glfwTerminate();
//		return nullptr;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	//More window options
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetWindowPos(window, 500, 100);
//
//	//Setup all the callback functions
//	glfwSetKeyCallback(window, keyboard);
//	glfwSetCursorPosCallback(window, mouse);
//	glfwSetMouseButtonCallback(window, mouseButtons);
//	glfwSetScrollCallback(window, scroll);
//
//	//Initialize GLEW and make sure glewExperimental is true for OpenGL 3.3
//	glewExperimental = GL_TRUE;
//	if (glewInit() != GLEW_OK)
//	{
//		std::cout << "Failed to initialize GLEW." << std::endl;
//		return nullptr;
//	}
//	enableSettings();
//	/*glCullFace(GL_BACK);
//	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);*/
//	glDepthFunc(GL_LEQUAL);
//	return window;
//}
//
//Camera initCamera()
//{
//	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
//	//cam.MovementSpeed = 100.0f;
//	return cam;
//}
//
//void handleMovement(GLFWwindow* window, GLfloat deltaTime)
//{
//	Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
//
//	if (glfwGetKey(window, GLFW_KEY_W))
//		camera->ProcessKeyboard(FORWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_S))
//		camera->ProcessKeyboard(BACKWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_A))
//		camera->ProcessKeyboard(LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_D))
//		camera->ProcessKeyboard(RIGHT, deltaTime);
//}
//
//int main()
//{
//	GLFWwindow* window = initWindow();
//	if (window == nullptr)
//	{
//		std::cout << "Unable to initalize window." << std::endl;
//		return -1;
//	}
//
//	// Start the timer
//	GLfloat deltaTime, lastFrame, currentFrame;
//	lastFrame = 0.0f;
//	currentFrame = 0.0f;
//
//	// Setup the viewport;
//	GLfloat width = 800, height = 600;
//	glViewport(0, 0, width, height);
//
//	// Setup Camera
//	Camera cam = Camera();
//	glfwSetWindowUserPointer(window, &cam);
//
//	GLuint program = compileShaders("../OpenGL3-3/shaders/advanced/instance/basic.vert.glsl", "../OpenGL3-3/shaders/advanced/instance/basic.frag.glsl");
//
//	// Setup MVP model
//	glm::mat4 model, view, proj;
//	//view = cam.GetViewMatrix();
//	//proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
//	
//	proj = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
//
//#pragma region quad
//
//	GLfloat quadVertices[] = {
//		// Positions     // Colors
//		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
//		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
//		-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,
//
//		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
//		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
//		0.05f, 0.05f, 0.0f, 1.0f, 1.0f
//	};
//
//#pragma endregion
//
//	GLuint VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 2));
//	glBindVertexArray(0);
//
//	glm::vec2 translations[100]; 
//	int index = 0;
//	GLfloat offset = 0.1f;
//	for (GLint y = -10; y < 10; y += 2)
//	{
//		for (GLint x = -10; x < 10; x += 2)
//		{
//			glm::vec2 translation;
//			translation.x = (GLfloat)x / 10.0f + offset;
//			translation.y = (GLfloat)y / 10.0f + offset;
//			translations[index++] = translation;
//		}
//	}
//
//	glUseProgram(program);
//	for (int i = 0; i < 100; i++)
//	{
//		std::stringstream ss;
//		std::string index;
//		ss << i;
//		index = ss.str();
//		GLint location = glGetUniformLocation(program, ("offsets[" + index + "]").c_str());
//		glUniform2f(location, translations[i].x , translations[i].y);
//	}
//
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//		currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glUseProgram(program);
//		glBindVertexArray(VAO);
//		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
//		glBindVertexArray(0);
//
//		glfwSwapBuffers(window);
//
//	}
//
//
//
//	glfwTerminate();
//	return 0;
//}
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include <Tools.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


// The MAIN function, from here we start our application and run the Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup and compile our shader(s)
	GLuint program = compileShaders("../OpenGL3-3/shaders/advanced/instance/basic.vert.glsl", "../OpenGL3-3/shaders/advanced/instance/basic.frag.glsl");

	// Generate a list of 100 quad locations/translation-vectors
	glm::vec2 translations[100];
	int index = 0;
	GLfloat offset = 0.1f;
	for (GLint y = -10; y < 10; y += 2)
	{
		for (GLint x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (GLfloat)x / 10.0f + offset;
			translation.y = (GLfloat)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	// Store instance data in an array buffer
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate quad VAO
	GLfloat quadVertices[] = {
		// Positions   // Colors
		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		0.05f, 0.05f, 0.0f, 1.0f, 1.0f
	};
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	// Also set instance data
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
	glBindVertexArray(0);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Clear buffers
		glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw 100 instanced quads
		
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
		glBindVertexArray(0);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}