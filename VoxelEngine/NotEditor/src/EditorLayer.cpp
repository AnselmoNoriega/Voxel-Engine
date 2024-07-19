#include "EditorLayer.h"

#include "imgui/imgui.h"

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
    }

    void EditorLayer::Detach()
    {
    }

    void EditorLayer::Update(float deltaTime)
    {
        {
            PROFILE_SCOPE("Render Start");

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

            Renderer::RenderScene(mCamera);
        }
    }

    void EditorLayer::OnEvent(Event& myEvent)
    {
        mCamera.OnEvent(myEvent);
    }

    void EditorLayer::ImGuiRender()
    {
        ImGui::Begin("Stats");

        auto stats = Renderer::GetStats();
        ImGui::Text("Renderer Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::End();
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