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

GLFWwindow* window;
GLuint program;
glm::mat4 view;
const std::string vertexShader = ".\\shaders\\tut6.vert.glsl";
const std::string fragmeShader = ".\\shaders\\tut6.frag.glsl";
bool keys[1024];

glm::vec2 lastPosition = glm::vec2(400.0f, 300.0f);
Camera cam;

bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::mat4 proj;

void updateCameraMovement()
{
	if (keys[GLFW_KEY_W])
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		program = compileShaders(vertexShader, fragmeShader);
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
			system("cls");
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		std::cout << "Current GLFW time: " << glfwGetTime() << std::endl;
}

void mouse(GLFWwindow* window, double xpos, double ypos)
{
	cam.ProcessMouseMovement(xpos, ypos);
}

void mouseButtons(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		if (action == GLFW_PRESS)
			cam.Zoom = 20;
		else
			cam.Zoom = 45;
}

void scroll(GLFWwindow* window, double xOff, double yOff)
{
	cam.ProcessMouseScroll(yOff*10);
	std::cout << yOff  << ", " << cam.Zoom << std::endl;
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowPos(window, 500, 100);
	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse);
	glfwSetMouseButtonCallback(window, mouseButtons);
	glfwSetScrollCallback(window, scroll);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return -1;
	}
	return 0;
}

void initGame()
{
	cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

int main()
{
	if (initWindow() == -1)
	{
		return -1;
	}
	
	initGame();
	int width = 800, height = 600;
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

#pragma region Verts
	GLfloat verts[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
#pragma endregion

	GLuint indicies[] = {
		0, 1, 3,
		1, 2, 3
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	
	glm::mat4 trans;
	GLuint texture1 = loadTexture("images/container.jpg");
	GLuint texture2 = loadTexture("images/awesomeface.png");

	program = compileShaders(vertexShader, fragmeShader);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	//Wire frame mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Colors
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
	glm::vec3 result = lightColor * toyColor;

	
	//glm::mat4
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	//proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 model;
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	GLuint time = glGetUniformLocation(program, "time");
	GLuint transLocation = glGetUniformLocation(program, "trans");
	GLuint modelUni = glGetUniformLocation(program, "model");
	GLuint viewUni = glGetUniformLocation(program, "view");
	GLuint projUni = glGetUniformLocation(program, "projection");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		updateCameraMovement();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		proj = glm::perspective(glm::radians(cam.Zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
		glm::mat4 view = cam.GetViewMatrix();


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(program, "myTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(program, "myTexture2"), 1);


		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projUni, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1f(time, glfwGetTime());

		glUseProgram(program);
		glBindVertexArray(VAO);
		
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 2 == 0)
				model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			if (i % 3 == 0 && i!= 0)
				model = glm::translate(model, glm::vec3(0.0f, (GLfloat)sin(glfwGetTime())/3, 0.0f));
			glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}