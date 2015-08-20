#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
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
GLuint loadCubemap(const std::vector<const char*>& images);
GLuint compileShaders(std::string vertex, std::string fragment);
GLuint compileShaders(std::string vertex, std::string fragment, std::string geometry);

#endif