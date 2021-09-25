#pragma once

namespace Visioneer
{

class ImGuiLayer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void onAttach();
    void onDetach();

    void begin();
    void end();
};

}
