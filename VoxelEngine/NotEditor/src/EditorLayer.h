#pragma once

#include <Forge.h>

#include "Forge/Renderer/Camera/EditorCamera.h"
#include "Forge/Renderer/Voxels/Chunk.h"

namespace Forge
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

        void InitChunk(int posX, int posZ);

    private:
        EditorCamera mCamera;

        std::map<Vec2Int, Ref<Chunk>> mChunks;

        int mMaxRenderDistanceSqrd = 16 * 16;
    };
}