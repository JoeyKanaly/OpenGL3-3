#define GLEW_STATIC
#include <iostream>
#include <algorithm>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <Tools.h>
#include <Camera.hpp>

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void mouse(GLFWwindow* window, double xpos, double ypos)
{
	/*Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	cam->ProcessMouseMovement()*/
}

void mouseButtons(GLFWwindow* window, int button, int action, int mods)
{

}

void scroll(GLFWwindow* window, double xOff, double yOff)
{
	Camera* cam =(Camera*)glfwGetWindowUserPointer(window);
	cam->ProcessMouseScroll(yOff);
	std::cout << "Mouse Scroll" << std::endl;
}

GLFWwindow* initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowPos(window, 500, 100);
	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetMouseButtonCallback(window, mouseButtons);
	glfwSetScrollCallback(window, scroll);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return nullptr;
	}
	return window;
}

Camera initCamera()
{
	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return cam;
}

void handleMovement(GLFWwindow* window, Camera* camera, GLfloat deltaTime)
{
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
	//Window Initialization
	GLFWwindow* window = initWindow();
	if (window == nullptr)
		std::cout << "Unable to initialize window, possible that OpenGL 3.3 isn't supported." << std::endl;

	//Start Timer
	GLfloat deltaTime, lastFrame, currentFrame;
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	//Viewport Initialization
	int width = 800, height = 600;
	glViewport(0, 0, width, height);

	//VBO, VAO,	and	LightVAO Initialization
	GLuint VBO, VAO, LightVAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &LightVAO);

#pragma region Verts
	GLfloat verts[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f
	};
#pragma endregion
	
	//Shader Compilation
	GLuint program = compileShaders("..\\OpenGL3-3\\shaders\\tut8.vert.glsl", "..\\OpenGL3-3\\shaders\\tut8.frag.glsl");
	GLuint basicProgram = compileShaders("..\\OpenGL3-3\\shaders\\basic.vert.glsl", "..\\OpenGL3-3\\shaders\\basic.frag.glsl");

	//VAO Data
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//LightVAO Data
	glBindVertexArray(LightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Setting up the camera
	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	//Setting up the Model, View, and Projection Matrices
	glm::mat4 model, view, proj;
	model = glm::mat4();
	view = cam.GetViewMatrix();
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	//Setup Uniform locations
	GLuint modelUniform, viewUniform, projUniform;
	GLuint modelUniform2, viewUniform2, projUniform2;
	GLuint objectColor, lightColor;
	modelUniform = glGetUniformLocation(program, "model");
	viewUniform = glGetUniformLocation(program, "view");
	projUniform = glGetUniformLocation(program, "proj");

	modelUniform2 = glGetUniformLocation(basicProgram, "model");
	viewUniform2 = glGetUniformLocation(basicProgram, "view");
	projUniform2 = glGetUniformLocation(basicProgram, "proj");

	objectColor = glGetUniformLocation(program, "objectColor");
	lightColor = glGetUniformLocation(program, "lightColor");

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Update timer
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Poll for events such as mouse and keyboard input.
		glfwPollEvents();

		//Set the background color of the scene.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update view matrix with camera's
		view = cam.GetViewMatrix();

		//Update the projection matrix with camera fov
		proj = glm::perspective(glm::radians(cam.Zoom), (float)width / (float)height, 0.1f, 100.0f);

		//Set Uniform Data
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(proj));
		
		glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewUniform2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projUniform2, 1, GL_FALSE, glm::value_ptr(proj));
		
		glUniform3fv(objectColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		glUniform3fv(lightColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		//Bind the program for use
		glUseProgram(program);
		handleMovement(window, &cam, deltaTime);
		//Bind the Vertex Array
		glBindVertexArray(VAO);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUseProgram(basicProgram);
		glDrawArrays(GL_LINE_LOOP, 0, 36);
		glBindVertexArray(0);

		//Swap between front and back buffers.
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &LightVAO);

	glfwTerminate();
	return 0;
}