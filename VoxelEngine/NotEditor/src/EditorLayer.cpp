#include "EditorLayer.h"

#include "imgui/imgui.h"

namespace Forge
{
    EditorLayer::EditorLayer() 
        :Layer("EditorLayer")
    {
    }

    void EditorLayer::Attach()
    {
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
    }
    
    void EditorLayer::OnEvent(Event& myEvent)
    {
    }
    
    void EditorLayer::ImGuiRender()
    {
        ImGui::ShowDemoWindow();
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