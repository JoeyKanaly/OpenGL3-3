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

int main()
{
	GLFWwindow* window = initWindow();
	if (window == nullptr)
	{
		std::cerr << "Couldn't initialize the window" << std::endl;
		return -1;
	}

	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



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
	glEnable(GL_MULTISAMPLE);
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
	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 55.0f));
	cam.MovementSpeed = 100.0f;
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
