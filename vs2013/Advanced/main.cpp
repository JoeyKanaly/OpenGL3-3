#define GLEW_STATIC
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.hpp>
#include <Model.h>
#include <Tools.h>

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
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
	enableSettings();

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	
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
	GLFWwindow* window = initWindow();
	if (window == nullptr)
	{
		std::cout << "Unable to initalize window." << std::endl;
		return -1;
	}

	// Start the timer
	GLfloat deltaTime, lastFrame, currentFrame;
	lastFrame = 0.0f;
	currentFrame = 0.0f;

	// Setup the viewport;
	int width = 800, height = 600;
	glViewport(0, 0, width, height);

	// Initalize the VAO and VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Compile the shaders
	GLuint program = compileShaders("../OpenGL3-3/shaders/advanced/template.vert.glsl", "../OpenGL3-3/shaders/advanced/template.frag.glsl");
	GLuint program2 = compileShaders("../OpenGL3-3/shaders/advanced/template.vert.glsl", "../OpenGL3-3/shaders/advanced/outline.frag.glsl");

	// Initalize and setup camera
	Camera cam = initCamera();
	glfwSetWindowUserPointer(window, &cam);

	// Load any models here
	//Model someModel = Model("../Model/models/");

	// Load any textures to be used
	GLuint wall = loadTexture("../OpenGL3-3/images/wall.jpg");
	GLuint container = loadTexture("../OpenGL3-3/images/container.jpg");
	GLuint grassTexture = loadTexture("../OpenGL3-3/images/grass.png");
	GLuint windowTexture = loadTexture("../OpenGL3-3/images/window.png");

#pragma region Verts
	GLfloat verts[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right         
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right     
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f  // bottom-left        
	};
#pragma endregion

	// Setup the VAO and VBO data
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setup the Model, View, and Projection matrices
	glm::mat4 model, view, proj;
	model = glm::mat4();
	view = cam.GetViewMatrix();
	proj = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	// Get the uniform locations
	GLint modelUniform = glGetUniformLocation(program, "model");
	GLint modelUniform2 = glGetUniformLocation(program2, "model");
	GLint viewUniform = glGetUniformLocation(program, "view");
	GLint viewUniform2 = glGetUniformLocation(program2, "view");
	GLint projUniform = glGetUniformLocation(program, "proj");
	GLint projUniform2 = glGetUniformLocation(program2, "proj");

	// Setup the initial data for the uniforms
	glUseProgram(program);
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(proj));

	glUseProgram(program2);
	glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewUniform2, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projUniform2, 1, GL_FALSE, glm::value_ptr(proj));

	// Setup the vegitation vector
	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3( 1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3( 0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	std::map<GLfloat, glm::vec3> sorted;
	for (int i = 0; i < vegetation.size(); i++)
	{
		GLfloat distance = glm::length(cam.Position - vegetation[i]);
		sorted[distance] = vegetation[i];
	}

#pragma region Plane
	// Setup Plane Verticies
	GLfloat planePoints[] =
	{
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,


		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};
#pragma endregion

	// Setup grass VAO
	GLuint GrassVAO, GrassVBO;
	glGenVertexArrays(1, &GrassVAO);
	glGenBuffers(1, &GrassVBO);

	glBindVertexArray(GrassVAO);
		glBindBuffer(GL_ARRAY_BUFFER, GrassVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planePoints), planePoints, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5.0f * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5.0f * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	GLfloat scale = 1.1f;
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glfwPollEvents();
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container);
		glUniform1i(glGetUniformLocation(program, "tex"), 0);

		handleMovement(window, deltaTime);
		view = cam.GetViewMatrix();
		glUseProgram(program);
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);
			
			// Draw the Boxes
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
			//glStencilFunc(GL_ALWAYS, 1, 0xFF);
			//glStencilMask(0xFF);
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(1.5f, 0.1f, -1.0f));
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Draw the floor
			//glStencilMask(0x00);
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, -5.5f, 0.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			glBindTexture(GL_TEXTURE_2D, wall);
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			model = glm::mat4();/*
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glDisable(GL_DEPTH_TEST);
			glUseProgram(program2);
			glUniformMatrix4fv(viewUniform2, 1, GL_FALSE, glm::value_ptr(view));
			model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
			model = glm::scale(model, glm::vec3(scale));
			glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(1.5f, 0.1f, -1.0f));
			model = glm::scale(model, glm::vec3(scale));
			glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);*/

		glBindVertexArray(0);

		glBindVertexArray(GrassVAO);
		glBindTexture(GL_TEXTURE_2D, windowTexture);

		for (std::map<GLfloat, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++)
		{
			/*
			model = glm::mat4();
			model = glm::translate(model, vegetation[i]);
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			for (int r = 1; r < 6; r++)
			{
				model = glm::mat4();
				model = glm::translate(model, vegetation[i]);
				model = glm::rotate(model, glm::radians(r * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}*/

			model = glm::mat4();
			model = glm::translate(model, it->second);
			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	glfwTerminate();
	return 0;
}