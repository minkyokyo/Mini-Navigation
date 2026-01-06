#include "shader.h"

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
    createShaderProgram((char*)vertexPath, (char*)fragmentPath);
}

char* Shader::readShader(const char* path)
{
    if (path == NULL)
	{
		std::cerr << "ERROR: Fail in readShader(" << path << ")" << std::endl;
		return NULL;
	}

	FILE* fp = fopen(path, "r");
	if (fp == NULL)
	{
		std::cerr << "ERROR: Fail in readShader(" << path << ")" << std::endl;
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	int count = ftell(fp);
	rewind(fp);

	char* content = NULL;
	if (count > 0)
	{
		content = new char[count + 1];		// +1 for null termination
		count = fread(content, sizeof(char), count, fp);
		content[count] = 0;					// Null-termination
	}
	fclose(fp);

	return content;
}

//? shader id가 꼭 GLuint일 필요가 있나?
GLuint Shader::createShader(GLenum shaderType, char * path)
{
    char* shaderSource = readShader(path);
    if(shaderSource == NULL)
    {
        std::cerr << "ERROR: Fail in createShaderProgram(" << path << ")" << std::endl;
        return 0;
    }

    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    return shaderID;
}

void Shader::createShaderProgram(char* vertexPath, char* fragmentPath)
{   
        GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexPath);
        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentPath);

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        setID(programID);

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

}   

GLuint Shader::getID()
{
    return ID;
}

void Shader::setID(GLuint id)
{
    ID = id;
}

void Shader::use() 
{ 
    glUseProgram(ID); 
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

