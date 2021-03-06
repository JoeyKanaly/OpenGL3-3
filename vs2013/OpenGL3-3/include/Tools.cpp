#ifndef TOOLS_H
#include "Tools.h"
#endif // TOOLS_H

GLuint loadTexture(const char* imagePath)
{
	SImage image;
	image.img = SOIL_load_image(imagePath, &image.width, &image.height, 0, SOIL_LOAD_RGBA);
	if (image.img == '\0')
	{
		std::cout << "Unable to load image at location: " << imagePath << std::endl;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//For when you want to clamp to the border
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f};
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.img);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image.img);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

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

GLuint compileShaders(std::string vertex, std::string fragment, std::string geometry)
{
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
	std::string line;
	std::string source;
	std::ifstream file;
	GLint success;
	GLchar infoLog[512];
	const char* sc;

	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
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

	file.open(geometry);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			source.append(line);
			source.append("\n");
		}
		file.close();
	}

	glShaderSource(geometryShader, 1, &(sc = source.c_str()), NULL);
	glCompileShader(geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
		std::cout << "ERROR GEOMETRY SHADER COMPILATION FAILED: " << infoLog << std::endl;
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
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "SHADER PROGRAM ERROR: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint loadCubemap(const std::vector<const char*>& images)
{
	if (images.size() != 6)
	{
		return -1;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	SImage image;

	for (int i = 0; i < images.size(); i++)
	{
		image.img = SOIL_load_image(images[i], &image.width, &image.height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.img);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
