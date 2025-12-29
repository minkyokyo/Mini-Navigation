
#pragma once

// #include <GL/glew.h>				// OpenGL Extension Wrangler Libary
#include <GLFW/glfw3.h>

// Create and delete the shaders and the program
void	createShaders(const char* vertexShaderFile, const char* fragmentShaderFile,
	GLuint& program, GLuint& vertexShader, GLuint& fragmentShader);
char* readShader(const char* filename);
GLuint	createShaderFromFile(GLenum shaderType, const char* filename);
void	printShaderInfoLog(GLuint obj, const char* shaderFilename);
void	printProgramInfoLog(GLuint obj);
void	deleteShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader);

