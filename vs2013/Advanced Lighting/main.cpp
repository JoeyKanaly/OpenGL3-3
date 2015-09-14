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

bool blinn = true;

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
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	glUseProgram(0);

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

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
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniformMatrix4fv(glGetUniformLocation(shaders, "view"), 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(10.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaders, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(shaders, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shaders, "viewPos"), 1, &cam.Position[0]);
		glUniform1i(glGetUniformLocation(shaders, "blinn"), blinn);

		glBindVertexArray(VAO);
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
	/*if (key == GLFW_KEY_SPACE)
	cam->Position.y += .01f;*/
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
}
