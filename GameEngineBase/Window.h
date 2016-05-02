//
// Created by Glenn Jacob on 19/12/15.
//

#ifndef MUSICBOX_WINDOW_H
#define MUSICBOX_WINDOW_H

#include <memory>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "IRenderer.h"

using std::shared_ptr;
using std::string;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Camera;

enum WindowFlags {
    NONE = 0x0,
    INVISIBLE = 0x1,
    FULLSCREEN = 0x2,
};

class Window {
public:
    Window();
    ~Window();

    int getWidth() const;
    int getHeight() const;
    const InputManager& getInputManager() const;
    void create(int width, int height, const string& title, WindowFlags flags = WindowFlags::NONE);

    double getTime() const;

    void pollEvents();
    void swapBuffers();
    bool shouldWindowClose();

    void enableVsync();
    void disableVsync();

private:
    void init();
    void setCallbackFunctions();
    void printVersionInfo() const;

    GLFWwindow* pGlfwWindow = nullptr;
    shared_ptr<GameEngine::Base::IRenderer> pSceneRenderer = nullptr;
    int width = 0;
    int height = 0;
    InputManager inputManager;
};

} // Base
} // GameEngine
} // ForlornMelody

#endif //MUSICBOX_WINDOW_H
