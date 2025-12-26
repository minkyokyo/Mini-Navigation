// src/main.cpp
#include <iostream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/navi/geometry/CubeGeometry.h"
#include "../include/navi/gfx/Mesh.h"

// ---------- tiny shader helpers (main.cpp 안에 최소로 넣음) ----------
static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetShaderInfoLog(s, len, nullptr, log.data());
        glDeleteShader(s);
        throw std::runtime_error("Shader compile failed:\n" + log);
    }
    return s;
}

static GLuint linkProgram(GLuint vs, GLuint fs) {
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetProgramInfoLog(p, len, nullptr, log.data());
        glDeleteProgram(p);
        throw std::runtime_error("Program link failed:\n" + log);
    }
    return p;
}

static void framebufferSizeCallback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

// 아주 간단한 입력 처리 (ESC 종료)
static void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    try {
        // ---- GLFW init ----
        if (!glfwInit()) {
            throw std::runtime_error("glfwInit() failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        GLFWwindow* window = glfwCreateWindow(1280, 720, "my_navi - cube", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow() failed");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSwapInterval(1); // vsync (원하면 0)

        // ---- GLAD load ----
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            throw std::runtime_error("gladLoadGLLoader() failed");
        }

        // ---- GL state (ES 스타일) ----
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // 디버그 편의: 면 컬링은 일단 끄고 시작해도 됨
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // ---- Shaders (location 고정: 0 pos / 1 normal / 2 uv) ----
        const char* vsSrc = R"GLSL(
            #version 330 core
            layout(location=0) in vec3 aPos;
            layout(location=1) in vec3 aNormal;
            layout(location=2) in vec2 aUV;

            uniform mat4 uMVP;
            uniform mat4 uModel;

            out vec3 vNormal;
            out vec3 vWorldPos;

            void main() {
                vec4 world = uModel * vec4(aPos, 1.0);
                vWorldPos = world.xyz;
                vNormal = mat3(uModel) * aNormal; // 스케일이 비정상적이면 inverse-transpose 쓰기
                gl_Position = uMVP * vec4(aPos, 1.0);
            }
        )GLSL";

        const char* fsSrc = R"GLSL(
            #version 330 core
            in vec3 vNormal;
            in vec3 vWorldPos;

            uniform vec3 uColor;

            out vec4 FragColor;

            void main() {
                vec3 N = normalize(vNormal);
                vec3 L = normalize(vec3(1.0, 1.5, 0.8));  // 간단한 고정 라이트
                float ndl = max(dot(N, L), 0.0);

                vec3 ambient = 0.15 * uColor;
                vec3 diffuse = ndl * uColor;

                FragColor = vec4(ambient + diffuse, 1.0);
            }
        )GLSL";

        GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
        GLuint prog = linkProgram(vs, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint locMVP   = glGetUniformLocation(prog, "uMVP");
        GLint locModel = glGetUniformLocation(prog, "uModel");
        GLint locColor = glGetUniformLocation(prog, "uColor");

        if (locMVP < 0 || locModel < 0 || locColor < 0) {
            std::cerr << "[Warn] uniform location not found (optimized out?)\n";
        }

        // ---- 여기서부터: 네가 만든 클래스 사용 ----
        // 1) CPU에서 큐브 지오메트리 생성
        navi::CubeGeometry cubeGeom(1.0f); // 한 변 길이 1.0
        navi::MeshData cubeData = cubeGeom.generate();

        // 2) GPU로 업로드 (VAO/VBO/EBO)
        navi::Mesh cubeMesh(cubeData);

        // ---- Render loop ----
        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            float aspect = (h == 0) ? 1.0f : (float)w / (float)h;

            glViewport(0, 0, w, h);
            glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 간단한 카메라 (내비 느낌으로 살짝 위에서)
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * 0.7f, glm::vec3(0, 1, 0));

            glm::mat4 view = glm::lookAt(
                glm::vec3(2.2f, 1.6f, 2.2f),  // eye
                glm::vec3(0.0f, 0.0f, 0.0f),  // center
                glm::vec3(0.0f, 1.0f, 0.0f)   // up
            );

            glm::mat4 proj = glm::perspective(glm::radians(55.0f), aspect, 0.1f, 100.0f);
            glm::mat4 mvp = proj * view * model;

            glUseProgram(prog);
            if (locMVP >= 0)   glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
            if (locModel >= 0) glUniformMatrix4fv(locModel, 1, GL_FALSE, &model[0][0]);
            if (locColor >= 0) glUniform3f(locColor, 0.2f, 0.85f, 0.55f);

            cubeMesh.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteProgram(prog);

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[Fatal] " << e.what() << "\n";
        glfwTerminate();
        return 1;
    }
}
