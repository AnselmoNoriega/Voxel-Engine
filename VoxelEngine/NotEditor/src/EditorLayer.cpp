#include "EditorLayer.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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

        mChunk.GenerateChunk();

        auto topVertices = mChunk.GetTopVertices();
        auto frontVertices = mChunk.GetFrontVertices();
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

            Renderer::DrawChunk(GetTransform(), nullptr, { 1.0f, 0.0f, 0.0f, 1.0f });

            Renderer::EndScene();
        }
    }

    void EditorLayer::OnEvent(Event& myEvent)
    {
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