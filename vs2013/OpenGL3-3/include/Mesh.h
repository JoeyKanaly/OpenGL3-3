#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	void draw(GLuint program);
	GLuint VAO, VBO, EBO;
private:
	void setupMesh();
};

#endif