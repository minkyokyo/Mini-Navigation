
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
#include "shader/ShaderContainer.h"
#include "map/Maploader.h"
#include "map/MapUICreator.h"
#include "map/MapRenderer.h"

struct Rect
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

static Rect ComputeMiniMapRect(int fbW, int fbH)
{
    // right-top minimap
    const int margin = 16;
    const float ratio = 0.30f; // 30% of shorter screen dimension

    const int size = std::max(120, (int)(std::min(fbW, fbH) * ratio));

    Rect r;
    r.w = size;
    r.h = size;
    r.x = fbW - margin - r.w;
    r.y = fbH - margin - r.h; // OpenGL viewport origin is bottom-left
    return r;
}

static int g_fbW = 1280;
static int g_fbH = 720;

static void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    // Don't call glViewport() here; we set multiple viewports every frame.
    g_fbW = std::max(1, width);
    g_fbH = std::max(1, height);
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

    // ---- World resources ----
    Shader worldShader(vertexShaderSource, fragmentShaderSource);
    navi::BoxGeometry boxGeom(0.5f, 0.5f, 0.5f);
    navi::MeshData boxMeshData = boxGeom.generate();
    navi::Mesh boxMesh(boxMeshData);

    Camera camera;
    InputProcessor input;
    input.Attach(window);

    // ---- UI resources (minimap overlay) ----
    Shader uiShader(uiVertexShaderSource, uiFragmentShaderSource);
    GLuint quadVAO = map::CreateQuadVAO();
    GLuint borderVAO = map::CreateBorderVAO();

    float lastTime = static_cast<float>(glfwGetTime());
    float aspect = 1280.0f / 720.0f;

    map::MapRenderer mapRenderer;

    bool mapLoaded = true;
    try
    {
        auto mapData = map::LoadMapFromJsonFile("sample_map.json");
        mapRenderer.upload(mapData);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] Failed to load map: " << e.what() << "\n";
        mapLoaded = false;
    }

    Shader mapShader(mapVertexShaderSource, mapFragmentShaderSource);

    while (!glfwWindowShouldClose(window))
    {
        float now = static_cast<float>(glfwGetTime());
        float deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        input.Update(deltaTime, camera);

        // Keep these updated (window resize safe)
        const float aspect = (float)g_fbW / (float)g_fbH;
        const Rect mini = ComputeMiniMapRect(g_fbW, g_fbH);

        // ==========================
        // PASS 1) WORLD (full screen)
        // ==========================
        glViewport(0, 0, g_fbW, g_fbH);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        worldShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(worldShader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(worldShader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(worldShader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));

        boxMesh.draw();

        // ==============================
        // PASS 2) MINIMAP (UI overlay)
        //   - right-top viewport
        //   - depth off
        // ==============================
        glViewport(mini.x, mini.y, mini.w, mini.h);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // (Optional) clip rendering strictly to minimap rect
        glEnable(GL_SCISSOR_TEST);
        glScissor(mini.x, mini.y, mini.w, mini.h);

        uiShader.use();

        // 1) minimap background (semi-transparent)
        {
            GLint loc = glGetUniformLocation(uiShader.getID(), "uColor");
            glUniform4f(loc, 0.0f, 0.0f, 0.0f, 0.35f);

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // 2) draw map contents (roads + pois)
        // Because we are already in the minimap viewport, the map renders into top-right UI.
        mapRenderer.draw(mapShader.getID());

        // 2) minimap border (optional)
        {
            GLint loc = glGetUniformLocation(uiShader.getID(), "uColor");
            glUniform4f(loc, 1.0f, 1.0f, 1.0f, 0.9f);

            glLineWidth(2.0f);
            glBindVertexArray(borderVAO);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }

        glBindVertexArray(0);

        // Restore state for next frame (good habit)
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
