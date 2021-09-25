#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Visioneer/Core/Base.h"
#include "Visioneer/ImGui/ImGuiLayer.h"

namespace Visioneer
{

class Application
{
public:
    Application();
    ~Application();

    void run();

    GLFWwindow *getWindow() { return mWindow; }

    static Application& get() { return *sInstance; }

    Application(const Application&) = delete;
    Application& operator=(Application&) = delete;

private:
    GLFWwindow *mWindow;
    ImGuiLayer *mImGuiLayer;

    static Application* sInstance;
};

}
