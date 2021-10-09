#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Visioneer/Core/Base.h"
#include "Visioneer/ImGui/ImGuiLayer.h"

#include "Visioneer/Panels/ViewerPanel.h"
#include "Visioneer/Panels/ContentBrowserPanel.h"
#include "Visioneer/Panels/ModelsControlPanel.h"
#include "Visioneer/Panels/ImageAugmentationPanel.h"

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

    ViewerPanel            *mViewerPanel;
    ContentBrowserPanel    *mContentBrowserPanel;
    ModelsControlPanel     *mModelsControlPanel;
    ImageAugmentationPanel *mImageAugmentationPanel;

    static Application* sInstance;
};

}
