#include "ModelsControlPanel.h"
#include "ViewerPanel.h"

#include "Visioneer/Core/Base.h"
#include "Visioneer/Models/UltraFace.h"
#include "Visioneer/Models/YOLOX.h"

#include "Visioneer/ImGui/CustomWidgets.h"

#include <imgui.h>

namespace Visioneer
{

ModelsControlPanel::ModelsControlPanel()
{
    mModels.emplace_back(new UltraFace);
    mModels.emplace_back(new YOLOX);
}

ModelsControlPanel::~ModelsControlPanel()
{
    for (auto model : mModels)
        delete model;
}

void ModelsControlPanel::onImGuiRender()
{
    ImGui::Begin("Models Control Panel", nullptr, ImGuiWindowFlags_NoMove);

    if (ImGui::Button("Select model"))
        ImGui::OpenPopup("ModelSelectionPopup");

    if (ImGui::BeginPopup("ModelSelectionPopup"))
    {
        for (const auto model : mModels)
        {
            if (ImGui::Selectable(model->getName().c_str()))
            {
                if (mSelectedModel && mSelectedModel->isAttached())
                {
                    mSelectedModel->onDetach();
                    if (mListener)
                        mListener->onAnnotationUpdate(EmptyAnnotation());
                }

                mSelectedModel = model;
            }
        }

        ImGui::EndPopup();
    }

    drawSelectedModel();

    ImGui::End();
}

void ModelsControlPanel::onImageUpdate(const cv::Mat &image)
{
    VSR_TRACE("");

    mInputImage = image;
    runSelectedModel();
}

void ModelsControlPanel::runSelectedModel()
{
    VariantAnnotation variantAnnotation = EmptyAnnotation();

    if (mSelectedModel && mSelectedModel->isAttached() && !mInputImage.empty())
        variantAnnotation = mSelectedModel->forward(mInputImage);

    if (mListener)
        mListener->onAnnotationUpdate(variantAnnotation);
}

void ModelsControlPanel::drawSelectedModel()
{
    if (mSelectedModel)
    {
        bool isUpdated = false;

        isUpdated |= drawModel<UltraFace>([](UltraFace *modelPtr)
        {
            bool isUpdated = false;
            isUpdated |= ImGui::SliderFloat("Confidence", &modelPtr->ConfidenceThreshold, 0.f, 1.f);
            isUpdated |= ImGui::SliderFloat("Supperession", &modelPtr->SupperessionThreshold, 0.f, 1.f);
            return isUpdated;
        });

        isUpdated |= drawModel<YOLOX>([](YOLOX *modelPtr)
        {
            bool isUpdated = false;
            isUpdated |= ImGui::SliderFloat("Confidence", &modelPtr->ConfidenceThreshold, 0.f, 1.f);
            isUpdated |= ImGui::SliderFloat("Supperession", &modelPtr->SupperessionThreshold, 0.f, 1.f);

            // TODO: get names from model and change version
            // static int currentIndex = 0;
            // static const char* names[2] = {"Tiny", "Medium"};
            // ImGui::BeginDisabled(modelPtr->isAttached());
            // ImGui::Combo("Version", &currentIndex, names, 2);
            // ImGui::EndDisabled();

            return isUpdated;
        });

        if (isUpdated)
            runSelectedModel();
    }
}

template<typename ModelType, typename DrawFuncPtr>
bool ModelsControlPanel::drawModel(DrawFuncPtr drawFuncPtr)
{
    bool isUpdated = false;

    ModelType *modelPtr = dynamic_cast<ModelType *>(mSelectedModel);
    if (modelPtr)
    {
        ImGui::Text("%s", modelPtr->getName().c_str());
        ImGui::SameLine();

        static bool mIsModelEnabled = false;
        ToggleButton(modelPtr->getName().c_str(), &mIsModelEnabled);
        isUpdated |= drawFuncPtr(modelPtr);

        if (mIsModelEnabled && !modelPtr->isAttached())
        {
            modelPtr->onAttach();
            isUpdated = true;
        }
        if (!mIsModelEnabled && modelPtr->isAttached())
        {
            modelPtr->onDetach();
            isUpdated = true;
        }
    }

    return isUpdated;
}

}
