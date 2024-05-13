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

    private:
        EditorCamera mCamera;

        Chunk mChunk;

        Ref<Texture> mTopTexture;
        Ref<Texture> mFrontTexture;
        std::vector<QuadSpecs> mTopQuads;
        std::vector<QuadSpecs> mFrontQuads;
        std::vector<QuadSpecs> mBackQuads;
        std::vector<QuadSpecs> mRightQuads;
        std::vector<QuadSpecs> mLeftQuads;
        std::vector<QuadSpecs> mBottomQuads;
    };
}