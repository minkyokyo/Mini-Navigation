#include <GLFW/glfw3.h>

// #include "../../../include/navi/shader/glshader.h"

// Create and delete the shaders and the program
void	createShaders(const char* vertexShaderFile, const char* fragmentShaderFile,
	GLuint& program, GLuint& vertexShader, GLuint& fragmentShader)
    {
   unsigned int vertexShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    }
char* readShader(const char* filename);


GLuint	createShaderFromFile(GLenum shaderType, const char* filename)
{
    GLuint	shader = glCreateShader(shaderType);
    if (isOK("glCreateShader()", __FILE__, __LINE__) == false)	return	0;

    if (shader == 0)
	{
		cerr << "ERROR: Fail in creating the shader for " << filename << endl;
		return 0;
	}

    // Read the shader file into a string
	const char* shaderSource = readShader(filename);
	if (shaderSource == NULL)	return	0;

	// Set the shader source
	glShaderSource(shader, 1, &shaderSource, NULL);

	// Delete the string read from the shader file
	delete[]	shaderSource;
	// Compile the shader
	glCompileShader(shader);

     // check for shader compile errors
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	

	// Print the compile error if exists
	printShaderInfoLog(shader, filename);

	return	shader;

}
void	printShaderInfoLog(GLuint obj, const char* shaderFilename);
void	printProgramInfoLog(GLuint obj);
void	deleteShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader);
