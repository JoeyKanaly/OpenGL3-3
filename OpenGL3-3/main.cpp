#define GLEW_STATIC
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Tools.h"

GLFWwindow* window;
GLuint program;


void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		program = compileShaders(".\\shaders\\tut3.vert.glsl", ".\\shaders\\tut3.frag.glsl");
}

int initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, keyboard);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return -1;
	}
	return 0;
}

int main()
{
	if (initWindow() == -1)
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	GLfloat verts[] = {
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.6f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.9f, 0.1f,
		-0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.3f,
		-0.5f,  0.5f, 0.0f, 0.4f, 0.5f, 0.6f
	};
	
	GLuint indicies[] = {
		0, 1, 3,
		1, 2, 3
	};

	program = compileShaders(".\\shaders\\tut3.vert.glsl", ".\\shaders\\tut3.frag.glsl");

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	//GLint nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	//Wire frame mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + .5;
		GLint vertexColorLocation = glGetUniformLocation(program, "ourColor");
		glUseProgram(program);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}