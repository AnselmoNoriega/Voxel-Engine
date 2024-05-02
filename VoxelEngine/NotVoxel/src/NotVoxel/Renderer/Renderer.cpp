#include "pch.h"
#include "Renderer.h"

#include "Platform/RendererAPI/OpenGL/GLShader.h"

namespace VoxelForge
{
    Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::Shutdown()
    {

    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        sSceneData->ViewProjectionMatrix = camera.GetViewProjection();
        sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(Renderer2DStorage::CameraData));
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(
        const Ref<Shader>& shader,
        const Ref<VertexArray>& vertexArray,
        const glm::mat4& transform
    )
    {
        shader->Bind();
        std::dynamic_pointer_cast<GLShader>(shader)->SetUniformMat4("uViewProjection", sSceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<GLShader>(shader)->SetUniformMat4("uTransform", transform);

        RenderCommand::DrawIndexed(vertexArray);
    }
}