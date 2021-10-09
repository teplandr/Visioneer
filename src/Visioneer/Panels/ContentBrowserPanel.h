#pragma once

#include <filesystem>

namespace Visioneer
{

class ContentBrowserPanel
{
public:
    ContentBrowserPanel();

    void onImGuiRender();

private:
    std::filesystem::path mCurrentDirectory;
};

}
