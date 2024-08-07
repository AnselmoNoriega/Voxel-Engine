#pragma once

#include "Camera/EditorCamera.h"

namespace Forge
{
    class Texture;

    struct QuadSpecs;

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void ResizeWindow(uint32_t width, uint32_t height);

        static void RenderScene(const EditorCamera& camera);

        static void SaveFace(const QuadSpecs& specs, const Ref<Texture>& texture, const glm::vec4& color);

        static void DrawRectFaces(const QuadSpecs& specs, const glm::vec4& color);
        static void DrawRLRectFaces(const QuadSpecs& specs, const glm::vec4& color);

        static void SetLineWidth(float width);

        struct Statistics
        {
            uint32_t DrawCalls = 1;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static Statistics GetStats();

    private:
        static void DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color);

        static float GetTextureIndex(const Ref<Texture>& texture);

        static void SaveData();

        static void DrawBatches();
    };
}