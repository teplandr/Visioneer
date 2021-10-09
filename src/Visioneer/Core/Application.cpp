#include "Application.h"

namespace Visioneer
{

Application* Application::sInstance = nullptr;

Application::Application()
{
    assert(!sInstance && "Application already exists!");
    sInstance = this;

    glfwSetErrorCallback([](int code, const char* message)
    {
        VSR_CRITICAL("[GLFW] {0}: {1}", code, message);
    });

    int glfwInitStatus = glfwInit();
    assert(glfwInitStatus && "Could not initialize GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    mWindow = glfwCreateWindow(1600, 900, "Visioneer", nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    int gladInitStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    assert(gladInitStatus && "Could not initialize Glad!");

    VSR_INFO("OpenGL Info:");
    VSR_INFO("- Vendor:   {0}", glGetString(GL_VENDOR));
    VSR_INFO("- Renderer: {0}", glGetString(GL_RENDERER));
    VSR_INFO("- Version:  {0}", glGetString(GL_VERSION));

    mImGuiLayer = new ImGuiLayer;
    mImGuiLayer->onAttach();

    mViewerPanel            = new ViewerPanel;
    mContentBrowserPanel    = new ContentBrowserPanel;
    mModelsControlPanel     = new ModelsControlPanel;
    mImageAugmentationPanel = new ImageAugmentationPanel;

    mViewerPanel->setOnImageLoadListener(mImageAugmentationPanel);
    mImageAugmentationPanel->setOnImageUpdateListener1(mViewerPanel);
    mImageAugmentationPanel->setOnImageUpdateListener2(mModelsControlPanel);
    mModelsControlPanel->setOnAnnotationUpdateListener(mViewerPanel);
}

Application::~Application()
{
    mImGuiLayer->onDetach();
    delete mImGuiLayer;

    delete mViewerPanel;
    delete mContentBrowserPanel;
    delete mModelsControlPanel;
    delete mImageAugmentationPanel;

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Application::run()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        mImGuiLayer->begin();

        mViewerPanel->onImGuiRender();
        mContentBrowserPanel->onImGuiRender();
        mModelsControlPanel->onImGuiRender();
        mImageAugmentationPanel->onImGuiRender();

        mImGuiLayer->end();

        glfwPollEvents();
        glfwSwapBuffers(mWindow);
    }
}

}
