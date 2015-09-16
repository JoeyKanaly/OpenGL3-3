#define GLEW_STATIC
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.hpp>
#include <Model.h>
#include <Tools.h>

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse(GLFWwindow* window, double xpos, double ypos);
void mouseButtons(GLFWwindow* window, int button, int action, int mods);
void scroll(GLFWwindow* window, double xOff, double yOff);
void enableSettings();
GLFWwindow* initWindow();
Camera initCamera();
void handleMovement(GLFWwindow* window, GLfloat deltaTime);

GLboolean blinn = true;

int main()
{
	GLFWwindow* window = initWindow();
	if (window == nullptr)
	{
		std::cerr << "Couldn't initialize the window" << std::endl;
		return -1;
	}

	int width = 800, height = 600;
	glViewport(0, 0, width, height);

	double currentTime, previousTime, deltaTime;
	currentTime = 0;
	previousTime = 0;
	deltaTime = 0;

#pragma region planeVerts

	GLfloat plane[] = {
		// Positions          // Normals         // Texture Coords
		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,

		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f
	};

#pragma endregion

	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	GLuint program = compileShaders("../OpenGL3-3/shaders/lighting/advanced/basic.vert.glsl", "../OpenGL3-3/shaders/lighting/advanced/basic.frag.glsl");
	GLuint shaders = compileShaders("../OpenGL3-3/shaders/lighting/advanced/blinnPhong.vert.glsl", "../OpenGL3-3/shaders/lighting/advanced/blinnPhong.frag.glsl");
	GLuint texture = loadTexture("../OpenGL3-3/images/wall.jpg");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
	glUseProgram(shaders);
	glUniformMatrix4fv(glGetUniformLocation(shaders, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUseProgram(0);

	glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		glfwPollEvents();
		handleMovement(window, deltaTime);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaders);
		glUniformMatrix4fv(glGetUniformLocation(shaders, "view"), 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(10.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shaders, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(shaders, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shaders, "viewPos"), 1, &cam.Position[0]);
		glUniform1i(glGetUniformLocation(shaders, "blinn"), blinn);

		//std::cout << (blinn ? "true" : "false") << std::endl;

		glBindVertexArray(VAO);
			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
	}
	return 0;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		blinn = !blinn;
	//cam->Position.y += .01f;
	if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
		cam->Position.y -= .01f;
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
		printf("%f, %f, %f", cam->Position.x, cam->Position.y, cam->Position.z);
	/*if (key == GLFW_KEY_X && action == GLFW_RELEASE)
	wireframe = !wireframe;*/
}

void mouse(GLFWwindow* window, double xpos, double ypos)
{
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	cam->ProcessMouseMovement(xpos, ypos);

}

void mouseButtons(GLFWwindow* window, int button, int action, int mods)
{

}

void scroll(GLFWwindow* window, double xOff, double yOff)
{
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	cam->ProcessMouseScroll(yOff);
	std::cout << "Mouse Scroll" << std::endl;
}

void enableSettings()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_BLEND);
	//glEnable(GL_PROGRAM_POINT_SIZE);
}

GLFWwindow* initWindow()
{
	//Initialize GLFW
	glfwInit();

	//Setup the window properties, to use OpenGL 3.3 and restrain it to the core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 16);


	//Create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Unable to create window." << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	//More window options
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowPos(window, 500, 100);

	//Setup all the callback functions
	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetMouseButtonCallback(window, mouseButtons);
	glfwSetScrollCallback(window, scroll);

	//Initialize GLEW and make sure glewExperimental is true for OpenGL 3.3
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return nullptr;
	}
	enableSettings();
	/*glCullFace(GL_BACK);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);*/
	glDepthFunc(GL_LEQUAL);
	return window;
}

Camera initCamera()
{
	Camera cam = Camera(glm::vec3(0.0f, 10.0f, 0.0f));
	cam.MovementSpeed = 10.0f;
	return cam;
}

void handleMovement(GLFWwindow* window, GLfloat deltaTime)
{
	Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

	if (glfwGetKey(window, GLFW_KEY_W))
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_B))
	{
		blinn = !blinn;
	}
}

//// Std. Includes
//#include <string>
//
//// GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>
//
//// GLFW
//#include <GLFW/glfw3.h>
//
//// GL includes
//#include <Camera.hpp>
//#include <Tools.h>
//
//// GLM Mathemtics
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//// Other Libs
//#include <SOIL/SOIL.h>
//
//// Properties
//const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;
//
//// Function prototypes
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void Do_Movement();
//GLuint loadTexture(GLchar* path);
//
//// Camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//
//// Delta
//GLfloat deltaTime = 0.0f;
//GLfloat lastFrame = 0.0f;
//
//// Options
//GLboolean blinn = false;
//
//// The MAIN function, from here we start our application and run our Game loop
//int main()
//{
//	// Init GLFW
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
//	glfwMakeContextCurrent(window);
//
//	// Set the required callback functions
//	glfwSetKeyCallback(window, key_callback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//
//	// Options
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	// Initialize GLEW to setup the OpenGL Function pointers
//	glewExperimental = GL_TRUE;
//	glewInit();
//
//	// Define the viewport dimensions
//	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//
//	// Setup some OpenGL options
//	glEnable(GL_DEPTH_TEST);
//	// glDepthFunc(GL_ALWAYS); // Set to always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
//
//	// Setup and compile our shaders
//	//Shader shader("advanced_lighting.vs", "advanced_lighting.frag");
//	GLuint shader = compileShaders("../OpenGL3-3/shaders/lighting/advanced/blinnPhong.vert.glsl", "../OpenGL3-3/shaders/lighting/advanced/blinnPhong.frag.glsl");
//
//	GLfloat planeVertices[] = {
//		// Positions          // Normals         // Texture Coords
//		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
//		-8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
//
//		8.0f, -0.5f, 8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
//		-8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
//		8.0f, -0.5f, -8.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f
//	};
//	// Setup plane VAO
//	GLuint planeVAO, planeVBO;
//	glGenVertexArrays(1, &planeVAO);
//	glGenBuffers(1, &planeVBO);
//	glBindVertexArray(planeVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//	glBindVertexArray(0);
//
//	// Light source
//	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//
//	// Load textures
//	GLuint floorTexture = loadTexture("../OpenGL3-3/images/wall.jpg");
//
//	// Game loop
//	while (!glfwWindowShouldClose(window))
//	{
//		// Set frame time
//		GLfloat currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		// Check and call events
//		glfwPollEvents();
//		Do_Movement();
//
//		// Clear the colorbuffer
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// Draw objects
//		glUseProgram(shader);
//		glm::mat4 view = camera.GetViewMatrix();
//		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
//		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//		// Set light uniforms
//		glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, &lightPos[0]);
//		glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &camera.Position[0]);
//		glUniform1i(glGetUniformLocation(shader, "blinn"), blinn);
//		// Floor
//		glBindVertexArray(planeVAO);
//		glBindTexture(GL_TEXTURE_2D, floorTexture);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glBindVertexArray(0);
//
//		std::cout << (blinn ? "true" : "false") << std::endl;
//
//		// Swap the buffers
//		glfwSwapBuffers(window);
//	}
//
//	glfwTerminate();
//	return 0;
//}
//
//// This function loads a texture from file. Note: texture loading functions like these are usually 
//// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
//// For learning purposes we'll just define it as a utility function.
//GLuint loadTexture(GLchar* path)
//{
//	// Generate texture ID and load texture data 
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	int width, height;
//	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
//	// Assign texture to ID
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	SOIL_free_image_data(image);
//	return textureID;
//
//}
//
//bool keys[1024];
//bool keysPressed[1024];
//// Moves/alters the camera positions based on user input
//void Do_Movement()
//{
//	// Camera controls
//	if (keys[GLFW_KEY_W])
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	if (keys[GLFW_KEY_S])
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	if (keys[GLFW_KEY_A])
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	if (keys[GLFW_KEY_D])
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//
//	if (keys[GLFW_KEY_B] && !keysPressed[GLFW_KEY_B])
//	{
//		blinn = !blinn;
//		keysPressed[GLFW_KEY_B] = true;
//	}
//}
//
//GLfloat lastX = 400, lastY = 300;
//bool firstMouse = true;
//// Is called whenever a key is pressed/released via GLFW
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//
//	if (key >= 0 && key <= 1024)
//	{
//		if (action == GLFW_PRESS)
//			keys[key] = true;
//		else if (action == GLFW_RELEASE)
//		{
//			keys[key] = false;
//			keysPressed[key] = false;
//		}
//	}
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	/*if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	GLfloat xoffset = xpos - lastX;
//	GLfloat yoffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;*/
//
//	camera.ProcessMouseMovement(xpos, ypos);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}
