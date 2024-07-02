#include "EditorLayer.h"

#include "Forge/Renderer/Voxels/TextureManager.h"

namespace Forge
{
    EditorLayer::EditorLayer()
        :Layer("EditorLayer")
    {
    }

    void EditorLayer::Attach()
    {
        auto [width, height] = WindowInfo::GetWindowSize();
        mCamera = EditorCamera(30.0f, width, height, 0.1f, 1000.0f);

        TextureManager::Initialize();
        mChunks.insert({ 0, CreateRef<Chunk>() });
        mChunks.insert({ 1, CreateRef<Chunk>() });
        mChunks.insert({ 2, CreateRef<Chunk>() });
        mChunks.insert({ 3, CreateRef<Chunk>() });
        mChunks.insert({ 4, CreateRef<Chunk>() });

        mChunks[0]->GenerateChunk({0, 0});

        mChunks[1]->GenerateChunk({0, 16});
        mChunks[2]->GenerateChunk({0, -16});
        mChunks[3]->GenerateChunk({16, 0});
        mChunks[4]->GenerateChunk({-16, 0});

        mChunks[0]->ConnectWithNeighbor(&mChunks[1]);
    }

    void EditorLayer::Detach()
    {
    }

    void EditorLayer::Update(float deltaTime)
    {
        {
            PROFILE_SCOPE("Render Start");
            Renderer::ResetStats();

            RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.8f, 1 });
            RenderCommand::Clear();
        }
        {
            PROFILE_SCOPE("Rendering");

            mCamera.Update(deltaTime);

            if (WindowInfo::GetWindowSize() != mCamera.GetViewportSize())
            {
                auto [width, height] = WindowInfo::GetWindowSize();
                mCamera.SetViewportSize(width, height);
            }

            Renderer::BeginScene(mCamera);

            for (auto& chunk : mChunks)
            {
                chunk.second->Render();
            }

            Renderer::EndScene();
        }
    }

    void EditorLayer::OnEvent(Event& myEvent)
    {
        mCamera.OnEvent(myEvent);
    }

    void EditorLayer::ImGuiRender()
    {
        //ImGui::DragFloat3("Position", &Translation.x, 0.01, -10.0f, 10.0f);
    }

    bool EditorLayer::KeyPressed(KeyPressedEvent& e)
    {
        return false;
    }

    bool EditorLayer::MouseButtonPressed(MouseButtonPressedEvent& e)
    {
        return false;
    }
}