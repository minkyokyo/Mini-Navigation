// src/main.cpp
#include <iostream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // ---- GLFW init ----
    if (!glfwInit())
    {
        throw std::runtime_error("glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(1280, 720, "my_navi - cube", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(window); // 만든 창 context로 지정.

    // ---- GLAD load ----
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("gladLoadGLLoader() failed");
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shaderProgram("../include/navi/shader/vertex.glsl", "../include/navi/shader/fragment.glsl");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         
         0.5f, -0.5f, 0.0f,
         -0.5f, -0.5f, 0.0f, 
         0.0f,  0.5f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 

    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 상태 함수
        glClear(GL_COLOR_BUFFER_BIT);         // 위에서 상태 설정하고 여기서 지움.

        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
