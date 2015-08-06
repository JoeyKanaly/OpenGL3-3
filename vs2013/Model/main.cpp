#define GLEW_STATIC
#include <iostream>
#include <algorithm>
#include <sstream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <Camera.hpp>
#include <Model.h>
#include <Tools.h>

bool wireframe;

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (key == GLFW_KEY_SPACE)
		cam->Position.y += .01f;
	if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
		cam->Position.y -= .01f;
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
		printf("%f, %f, %f", cam->Position.x, cam->Position.y, cam->Position.z);
	if (key == GLFW_KEY_X && action == GLFW_RELEASE)
		wireframe = !wireframe;
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

GLFWwindow* initWindow()
{
	//Initialize GLFW
	glfwInit();

	//Setup the window properties, to use OpenGL 3.3 and restrain it to the core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	return window;
}

Camera initCamera()
{
	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	//cam.MovementSpeed = 100.0f;
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
}

int main()
{
	// Initialize the window.
	GLFWwindow* window = initWindow();
	if (window == nullptr)
	{
		std::cout << "Unable to initialize the window, possible that OpenGL 3.3 isn't supported." << std::endl;
		std::cin.get();
		return -1;
	}

	// Start the timer
	GLfloat deltaTime, lastFrame, currentFrame;
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	// Setup the viewport
	int width = 800, height = 600;
	glViewport(0, 0, width, height);

	// VBO, VAO initialize
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// Shader Compilation
	GLuint program = compileShaders("../OpenGL3-3/shaders/models/model.vert.glsl", "../OpenGL3-3/shaders/models/model.frag.glsl");

	// Camera setup
	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f)
	};

	// Load the model
	Model dice[] = { Model("./models/dice/4.obj"), Model("./models/dice/6.obj"), Model("./models/dice/8.obj"), Model("./models/dice/12.obj"), Model("./models/dice/20.obj") };

	// Initialize the model, view, and projection matrix
	glm::mat4 model, view, projection;
	view = cam.GetViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	// Setup the uniforms
	GLint modelUniform, viewUniform, projUniform;
	modelUniform = glGetUniformLocation(program, "model");
	viewUniform = glGetUniformLocation(program, "view");
	projUniform = glGetUniformLocation(program, "proj");

	while (!glfwWindowShouldClose(window))
	{
		// Update Timer
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// Poll for input
		glfwPollEvents();
		
		// Clear out the color and the depth buffers and set the clear color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		handleMovement(window, deltaTime);
		// Update the view matrix with the camera data
		view = cam.GetViewMatrix();

		// Use the shader program
		glUseProgram(program);
		
		// Set the uniform data
		//glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1f(glGetUniformLocation(program, "time"), glfwGetTime());
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (int i = 0; i < 5; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(cubePositions[i].x * 0.5f, cubePositions[i].y * 0.5f, cubePositions[i].z * 0.5f));
			model = glm::rotate(model, glm::radians(20.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::scale(model, glm::vec3(0.1f));
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			dice[i].draw(program);
		}

		// Swap between the front and the back buffers
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	return 0;
}