#pragma once

#include <NotVoxel.h>

#include "NotVoxel/Renderer/Camera/EditorCamera.h"

namespace VoxelForge
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override = default;

        void Attach() override;
        void Detach() override;

        void Update(float deltaTime) override;
        void OnEvent(Event& myEvent) override;
        void ImGuiRender() override;

    private:
        bool KeyPressed(KeyPressedEvent& e);
        bool MouseButtonPressed(MouseButtonPressedEvent& e);

    private:
    };
}