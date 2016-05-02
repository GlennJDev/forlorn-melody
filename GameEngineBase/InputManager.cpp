//
// Created by Glenn Jacob on 19/12/15.
//

#include "InputManager.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

unordered_map<int, bool> InputManager::keysPressed;

glm::dvec2 InputManager::cursorPosition;

bool InputManager::shiftPressed = false;

bool InputManager::controlPressed = false;

bool InputManager::altPressed = false;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keysPressed[key] = true;
    } else if (action == GLFW_RELEASE) {
        keysPressed[key] = false;
    }
    setModifierKeys(mods);
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    cursorPosition.x = xpos;
    cursorPosition.y = ypos;
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    keysPressed[button] = action == GLFW_PRESS;
    setModifierKeys(mods);
}

void InputManager::setModifierKeys(int mods)
{
    shiftPressed = mods & GLFW_MOD_SHIFT;
    controlPressed = mods & GLFW_MOD_CONTROL;
    altPressed = mods & GLFW_MOD_ALT;
}

bool InputManager::isKeyPressed(int glfwKeyCode) const
{
    if (keysPressed.find(glfwKeyCode) != keysPressed.end()) {
        return keysPressed[glfwKeyCode];
    }
    return false;
}

bool InputManager::isShiftPressed() const
{
    return shiftPressed;
}

bool InputManager::isControlPressed() const
{
    return controlPressed;
}

bool InputManager::isAltPressed() const
{
    return altPressed;
}

glm::dvec2 InputManager::getCursorPosition() const
{
    return cursorPosition;
}
} // Base
} // GameEngine
} // ForlornMelody