#pragma once

#include "Camera/EditorCamera.h"

namespace VoxelForge
{
    class Texture;
    class Camera;
    class TextComponent;

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const EditorCamera& camera);
        static void EndScene();

        static void Flush();

        static void DrawCube(const glm::mat4& transform, float textureIndex, const glm::vec4& color);

        static void DrawRect(const glm::mat4& transform, const glm::vec4& color);

        static void SetLineWidth(float width);

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color);

        static float GetTextureIndex(const Ref<Texture>& texture);

        static void StartBatch();
        static void NextBatch();
    };
}