#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>

struct SImage
{
	int width;
	int height;
	unsigned char* img;
};

GLuint loadTexture(const char* imagePath);
GLuint compileShaders(std::string vertex, std::string fragment);

#endif