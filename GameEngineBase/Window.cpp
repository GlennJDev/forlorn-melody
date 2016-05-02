//
// Created by Glenn Jacob on 19/12/15.
//

#include "Window.h"
#include "IRenderer.h"
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

Window::Window() {
    this->init();
}

Window::~Window() {
    glfwDestroyWindow(this->pGlfwWindow);
}

void Window::init() {

    if (!glfwInit())
        exit(EXIT_FAILURE);
}

void Window::create(int width, int height, const string &title, WindowFlags flags) {
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Not use old versions of OpenGL
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); // Remove flickering

    this->width = width;
    this->height = height;

    // Open a window and create its OpenGL context
    if ((flags & FULLSCREEN) > 0) {
        this->pGlfwWindow = glfwCreateWindow(this->width, this->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        this->pGlfwWindow = glfwCreateWindow(this->width, this->height, title.c_str(), NULL, NULL);
    }

    if (!this->pGlfwWindow) {
        cerr << "Failed to open GLFW window. If you have an intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
    }

    cout << "GLFW window created" << endl;

    glfwMakeContextCurrent(this->pGlfwWindow);

    glewExperimental = true;
    GLenum retVal = glewInit();
    if (GLEW_OK != retVal) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
    }
    cout << "glew initialized with version: " << glewGetString(GLEW_VERSION) << endl;

    this->printVersionInfo();

    this->setCallbackFunctions();

    // Ensure we capture every pressed key
    glfwSetInputMode(this->pGlfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
}

void Window::enableVsync() {
    glfwSwapInterval(1);
}

void Window::disableVsync() {
    glfwSwapInterval(0);
}

void Window::printVersionInfo() const {
    // VERSION INFORMATION
    int glVersionMajor;
    int glVersionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &glVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glVersionMinor);
    printf("\n=== OpenGL Implementation ===\n");
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GL Version (Strict): %d.%d\n", glVersionMajor, glVersionMinor);
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(this->pGlfwWindow);
}

bool Window::shouldWindowClose() {
    return glfwWindowShouldClose(this->pGlfwWindow);
}

void Window::setCallbackFunctions()
{
    glfwSetKeyCallback(this->pGlfwWindow, InputManager::keyCallback);
    glfwSetCursorPosCallback(this->pGlfwWindow, InputManager::cursorPositionCallback);
    glfwSetMouseButtonCallback(this->pGlfwWindow, InputManager::mouseButtonCallback);
}

int Window::getWidth() const
{
    return this->width;
}

int Window::getHeight() const
{
    return this->height;
}

const InputManager &Window::getInputManager() const
{
    return this->inputManager;
}

double Window::getTime() const
{
    return glfwGetTime() * 1000;
}

} // Base
} // GameEngine
} // ForlornMelody

