#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include "camera/Camera.h"
#include "input/InputProcessor.h"

void InputProcessor::Attach(GLFWwindow *window)
{
    this->window = window;
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, &InputProcessor::KeyCallback);
    glfwSetCursorPosCallback(window, &InputProcessor::CursorPosCallback);

    // 기본은 커서 보이게
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputProcessor::Update(float dt, Camera &cam)
{
    if (InputProcessor::GetMode() != ModeState::CAMERA_CONTROL)
        return;

    if (m_keys[GLFW_KEY_W])
        cam.ProcessKeyboard(Camera_Movement::FORWARD, dt);
    if (m_keys[GLFW_KEY_S])
        cam.ProcessKeyboard(Camera_Movement::BACKWARD, dt);
    if (m_keys[GLFW_KEY_A])
        cam.ProcessKeyboard(Camera_Movement::LEFT, dt);
    if (m_keys[GLFW_KEY_D])
        cam.ProcessKeyboard(Camera_Movement::RIGHT, dt);

    if (mouseDX != 0.0f || mouseDY != 0.0f)
    {
        cam.ProcessMouseMovement(mouseDX, mouseDY);
        mouseDX = 0.0f;
        mouseDY = 0.0f;
    }
}
