#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include "camera/Camera.h"

class InputProcessor
{
public:
    enum class ModeState
    {
        DEFAULT,
        CAMERA_CONTROL
    };

    void Attach(GLFWwindow *window);
    void Update(float dt, Camera &cam);
    ModeState GetMode() const { return currentMode; }

private:
    ModeState currentMode = ModeState::DEFAULT;
    std::array<bool, GLFW_KEY_LAST + 1> m_keys{}; // false로 초기화

    GLFWwindow *window = nullptr;

    bool firstMouse = true;
    double lastX = 0.0;
    double lastY = 0.0;
    float mouseDX = 0.0f; // 프레임 누적
    float mouseDY = 0.0f;

private:
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto *self = static_cast<InputProcessor *>(glfwGetWindowUserPointer(window));

        if (!self || key < 0 || key > GLFW_KEY_LAST)
            return;

        if (action == GLFW_PRESS)
            self->m_keys[key] = true;
        if (action == GLFW_RELEASE)
            self->m_keys[key] = false;

        // FSM 전환: V 누르면 토글
        if (key == GLFW_KEY_V && action == GLFW_PRESS)
        {
            if (self->currentMode == ModeState::DEFAULT)
                self->EnterCameraMode();
            else
                self->ExitCameraMode();
        }

        // 종료(선택)
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        auto *self = static_cast<InputProcessor *>(glfwGetWindowUserPointer(window));
        if (!self)
            return;
        self->OnMouse(xpos, ypos);
    }

    void OnMouse(double xpos, double ypos)
    {
        if (currentMode != ModeState::CAMERA_CONTROL)
            return;

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        mouseDX += static_cast<float>(xpos - lastX);
        mouseDY += static_cast<float>(lastY - ypos); // y는 뒤집혀있음

        lastX = xpos;
        lastY = ypos;
    }

    void EnterCameraMode()
    {
        currentMode = ModeState::CAMERA_CONTROL;
        firstMouse = true;
        mouseDX = mouseDY = 0.0f;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::puts("[FSM] Enter CameraControl mode");
    }

    void ExitCameraMode()
    {
        currentMode = ModeState::DEFAULT;
        firstMouse = true;
        mouseDX = mouseDY = 0.0f;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::puts("[FSM] Enter Navigation mode");
    }
};