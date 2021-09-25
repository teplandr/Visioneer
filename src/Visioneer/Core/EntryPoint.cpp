#include "Visioneer/Core/Application.h"

int main()
{
    Visioneer::Logger::init();

    VSR_INFO("Begin");

    auto app = new Visioneer::Application;
    app->run();
    delete app;

    VSR_INFO("End");
}
