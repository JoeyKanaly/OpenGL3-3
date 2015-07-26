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
	Camera* cam = (Camera*)glfwGetWindowUserPointer(window);
	cam->ProcessMouseMovement(xpos, ypos);
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
	return window;
}

Camera initCamera()
{
	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
#pragma endregion
	
#pragma region Positions
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
#pragma endregion

	//Shader Compilation
	GLuint program = compileShaders("..\\OpenGL3-3\\shaders\\lighting\\simple.vert.glsl", "..\\OpenGL3-3\\shaders\\lighting\\spotlight.frag.glsl");
	GLuint basicProgram = compileShaders("..\\OpenGL3-3\\shaders\\basic.vert.glsl", "..\\OpenGL3-3\\shaders\\basic.frag.glsl");

	//Load the images
	int imgWidth, imgHeight;
	GLuint diffuseMap = loadTexture("../OpenGL3-3/images/container2.png");
	GLuint specMap = loadTexture("../OpenGL3-3/images/container2_specular2.png");

	//VAO Data
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//LightVAO Data
	glBindVertexArray(LightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Other light data
	glm::vec3 lightPos =glm::vec3(1.2f, 1.0f, 2.0f);

	//Setting up the camera
	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	//Setting up the Model, View, and Projection Matrices
	glm::mat4 model, view, proj;
	model = glm::mat4();
	view = cam.GetViewMatrix();
	proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	//Setup Uniform locations
	GLint modelUniform, viewUniform, projUniform;
	GLint modelUniform2, viewUniform2, projUniform2;
	GLint lightPosUniform, viewerPositonUniform;
	GLint matAmbient, matDiffuse, matSpecular, matShine;
	GLint lightAmbient, lightDiffuse, lightSpecular;
	GLint lightDir, lightCutOff, lightOuterCutOff;
	GLint lightConst, lightLinear, lightQuadratic;
	modelUniform = glGetUniformLocation(program, "model");
	viewUniform = glGetUniformLocation(program, "view");
	projUniform = glGetUniformLocation(program, "proj");
	viewerPositonUniform = glGetUniformLocation(program, "viewPos");

	lightPosUniform = glGetUniformLocation(program, "light.position");
	lightDir = glGetUniformLocation(program, "light.direction");
	lightCutOff = glGetUniformLocation(program, "light.cutOff");
	lightOuterCutOff = glGetUniformLocation(program, "light.outerCutOff");

	lightAmbient = glGetUniformLocation(program, "light.ambient");
	lightDiffuse = glGetUniformLocation(program, "light.diffuse");
	lightSpecular = glGetUniformLocation(program, "light.specular");

	lightConst = glGetUniformLocation(program, "light.constant");
	lightLinear = glGetUniformLocation(program, "light.linear");
	lightQuadratic = glGetUniformLocation(program, "light.quadratic");

	matDiffuse = glGetUniformLocation(program, "material.diffuse");
	matSpecular = glGetUniformLocation(program, "material.specular");
	matShine = glGetUniformLocation(program, "material.shine");

	modelUniform2 = glGetUniformLocation(basicProgram, "model");
	viewUniform2 = glGetUniformLocation(basicProgram, "view");
	projUniform2 = glGetUniformLocation(basicProgram, "proj");
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Update timer
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Poll for events such as mouse and keyboard input.
		glfwPollEvents();

		//Handle movement with the keys
		handleMovement(window, deltaTime);

		//Set the background color of the scene.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update view matrix with camera's
		view = cam.GetViewMatrix();

		//Update the projection matrix with camera fov
		proj = glm::perspective(glm::radians(cam.Zoom), (float)width / (float)height, 0.1f, 100.0f);

		//Bind the program for use
		glUseProgram(program);

		//Set Uniform Data
		model = glm::mat4();
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3f(viewerPositonUniform, cam.Position.x, cam.Position.y, cam.Position.z);

		//Set light properties
		glUniform3f(lightPosUniform, cam.Position.x, cam.Position.y, cam.Position.z);
		glUniform3f(lightDir, cam.Front.x, cam.Front.y, cam.Front.z);
		glUniform1f(lightCutOff, cos(glm::radians(12.5f)));
		glUniform1f(lightOuterCutOff, cos(glm::radians(17.5f)));
		glUniform3f(lightAmbient, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuse, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecular, 1.0f, 1.0f, 1.0f);
		glUniform1f(lightConst, 1.0f);
		glUniform1f(lightLinear, 0.045f);
		glUniform1f(lightQuadratic, 0.0075f);

		// Set material properties
		glUniform1i(matDiffuse, 0);
		glUniform1i(matSpecular, 1);
		glUniform1f(matShine, 64.0f);
		
		//Enable and bind Textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specMap);


		//Bind the Vertex Array
		glBindVertexArray(VAO);

			//Draw the objects(Cubes)
			for (int i = 0; i < 10; i++)
			{
				model = glm::mat4();
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			////Draw the light
			//glUseProgram(basicProgram);

			//glUniformMatrix4fv(viewUniform2, 1, GL_FALSE, glm::value_ptr(view));
			//glUniformMatrix4fv(projUniform2, 1, GL_FALSE, glm::value_ptr(proj));
			//model = glm::mat4();
			//model = glm::translate(model, lightPos);
			//model = glm::scale(model, glm::vec3(0.2f));
			//glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model));
			//glDrawArrays(GL_TRIANGLES, 0, 36);
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