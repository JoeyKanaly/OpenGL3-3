#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

GLuint compileShaders(std::string vertex, std::string fragment)
{
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
	std::string line;
	std::string source;
	std::ifstream file;
	GLint success;
	GLchar infoLog[512];
	const char* sc;

	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	file.open(vertex);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			source.append(line);
			source.append("\n");
		}
		file.close();
	}

	glShaderSource(vertexShader, 1, &(sc = source.c_str()), NULL);
	
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR VERTEX SHADER COMPILATION FAILED: " << infoLog << std::endl;
	}

	source = "";

	file.open(fragment);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			source.append(line);
			source.append("\n");
		}
		file.close();
	}

	glShaderSource(fragmentShader, 1, &(sc = source.c_str()), NULL);
	
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED: " << infoLog << std::endl;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM ERROR: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
}

#endif