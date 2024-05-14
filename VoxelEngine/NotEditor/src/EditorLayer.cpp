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

        for (size_t i = 0; i < 6; i++)
        {
            mQuads[i] = mChunk.GetVertices((QuadPosition)i);
        }

        mTextures[0] = Texture::Create("Assets/Textures/grass_block_top.png");
        for (size_t i = 1; i < 6; i++)
        {
            mTextures[i] = Texture::Create("Assets/Textures/grass_block_bottom.png");
        }
    }

    void EditorLayer::Detach()
    {
    }

    glm::mat4 GetTransform()
    {
        glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f, 0.0f, 0.0f }));

        return glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f })
            * rotation
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });
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

            for (uint8_t i = 0; i < 4; ++i)
            {
                for (const auto& quad : mQuads[i])
                {
                    Renderer::DrawFace(quad, mTextures[i], { 1.0f, 1.0f, 1.0f, 1.0f });
                }
            }
            for (uint8_t i = 4; i < 6; ++i)
            {
                for (const auto& quad : mQuads[i])
                {
                    Renderer::DrawRLFace(quad, mTextures[i], { 1.0f, 1.0f, 1.0f, 1.0f });
                }
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