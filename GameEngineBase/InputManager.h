//
// Created by Glenn Jacob on 19/12/15.
//

#ifndef MUSICBOX_INPUTMANAGER_H
#define MUSICBOX_INPUTMANAGER_H

#include <unordered_map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using std::unordered_map;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class InputManager
{
public:
    InputManager();
    virtual ~InputManager();

    bool isKeyPressed(int glfwKeyCode) const;
    bool isShiftPressed() const;
    bool isControlPressed() const;
    bool isAltPressed() const;

    glm::dvec2 getCursorPosition() const;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
    static void setModifierKeys(int mods);
    static unordered_map<int, bool> keysPressed;
    static glm::dvec2 cursorPosition;
    static bool shiftPressed;
    static bool controlPressed;
    static bool altPressed;
};
} // Base
} // GameEngine
} // ForlornMelody




#endif //MUSICBOX_INPUTMANAGER_H
