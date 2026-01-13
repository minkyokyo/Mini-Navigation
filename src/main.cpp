
#include <iostream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "navi/geometry/BoxGeometry.h"
#include "navi/gfx/Mesh.h"
#include "camera/Camera.h"
#include "input/InputProcessor.h"

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f,1.0f,1.0f, 1.0f);\n"
                                   "}\n\0";

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

    Shader shaderProgram(vertexShaderSource, fragmentShaderSource);
    navi::BoxGeometry boxGeom(0.5f, 0.5f, 0.5f);
    navi::MeshData boxMeshData = boxGeom.generate();
    navi::Mesh boxMesh(boxMeshData);

    Camera camera;
    InputProcessor input;
    input.Attach(window);

    float lastTime = static_cast<float>(glfwGetTime());
    float aspect = 1280.0f / 720.0f;

    while (!glfwWindowShouldClose(window))
    {
        float now = static_cast<float>(glfwGetTime());
        float deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        input.Update(deltaTime, camera);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 상태 함수
        glClear(GL_COLOR_BUFFER_BIT);         // 위에서 상태 설정하고 여기서 지움.

        shaderProgram.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));

        boxMesh.draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
