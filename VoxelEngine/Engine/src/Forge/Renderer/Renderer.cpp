#include "pch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "API/VertexArray.h"
#include "API/Shader.h"
#include "API/Texture.h"

#include "API/UniformBuffer.h"

#include "RenderCommand.h"

namespace Forge
{
    struct QuadVertex
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec2 TexCoord = { 0.0f, 0.0f };
        glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 0.0f };
        float TexIndex = 0.0f;
    };

    struct LineVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };

    struct RendererStorage
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> ObjShader;
        Ref<Texture> EmptyTexture;

        Ref<VertexArray> LineVertexArray;
        Ref<VertexBuffer> LineVertexBuffer;
        Ref<Shader> LineShader;

        uint32_t IndexCount = 0;
        QuadVertex* VertexBufferBase = nullptr;
        QuadVertex* VertexBufferPtr = nullptr;

        uint32_t LineVertexCount = 0;
        LineVertex* LineVertexBufferBase = nullptr;
        LineVertex* LineVertexBufferPtr = nullptr;

        float LineWidth = 2.0f;

        std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;

        glm::vec4 VertexPositions[4] = { { -0.5, -0.5, 0.0, 1.0f },
                                         {  0.5, -0.5, 0.0, 1.0f },
                                         {  0.5,  0.5, 0.0, 1.0f },
                                         { -0.5,  0.5, 0.0, 1.0f } };

        glm::vec2 TextureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f },
                                       { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        Renderer::Statistics Stats;


        struct CameraData
        {
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static RendererStorage sData;

    void Renderer::Init()
    {
        PROFILE_FUNCTION();

        RenderCommand::Init();

        {
            sData.QuadVertexArray = VertexArray::Create();

            sData.QuadVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(QuadVertex));
            sData.QuadVertexBuffer->SetLayout(
                {
                    {ShaderDataType::Float3, "aPosition"},
                    {ShaderDataType::Float2, "aTexCoord"},
                    {ShaderDataType::Float4, "aColor"   },
                    {ShaderDataType::Float,  "aTexIndex"},
                    {ShaderDataType::Int,    "aEntityID"}
                });
            sData.QuadVertexArray->AddVertexBuffer(sData.QuadVertexBuffer);
            sData.VertexBufferBase = new QuadVertex[sData.MaxVertices];
        }
        {
            sData.LineVertexArray = VertexArray::Create();

            sData.LineVertexBuffer = VertexBuffer::Create(sData.MaxVertices * sizeof(LineVertex));
            sData.LineVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "aPosition" },
                { ShaderDataType::Float4, "aColor"    }
                });
            sData.LineVertexArray->AddVertexBuffer(sData.LineVertexBuffer);
            sData.LineVertexBufferBase = new LineVertex[sData.MaxVertices];
        }
        {
            uint32_t* indices = new uint32_t[sData.MaxIndices];

            uint32_t offset = 0;
            for (uint32_t i = 0; i < sData.MaxIndices; i += 6)
            {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 0;

                offset += 4;
            }

            Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sData.MaxIndices);
            sData.QuadVertexArray->SetIndexBuffer(indexBuffer);
            delete[] indices;

            sData.EmptyTexture = Texture::Create(TextureSpecification());
            uint32_t emptyTextureData = 0xffffffff;
            sData.EmptyTexture->SetData(&emptyTextureData, sizeof(uint32_t));
        }

        sData.ObjShader = Shader::Create("Assets/Shaders/Base");
        sData.LineShader = Shader::Create("Assets/shaders/Color");

        sData.TextureSlots[0] = sData.EmptyTexture;

        sData.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererStorage::CameraData), 0);
    }

    void Renderer::Shutdown()
    {
        PROFILE_FUNCTION();

        delete[] sData.VertexBufferBase;
    }

    void Renderer::BeginScene(const EditorCamera& camera)
    {
        PROFILE_FUNCTION();

        sData.CameraBuffer.ViewProjection = camera.GetViewProjection();
        sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(RendererStorage::CameraData));

        StartBatch();
    }

    void Renderer::EndScene()
    {
        PROFILE_FUNCTION();

        Flush();
    }

    void Renderer::Flush()
    {
        if (sData.IndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)sData.VertexBufferPtr - (uint8_t*)sData.VertexBufferBase);
            sData.QuadVertexBuffer->SetData(sData.VertexBufferBase, dataSize);

            for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
            {
                sData.TextureSlots[i]->Bind(i);
            }

            sData.ObjShader->Bind();
            RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.IndexCount);
            ++sData.Stats.DrawCalls;
        }

        if (sData.LineVertexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)sData.LineVertexBufferPtr - (uint8_t*)sData.LineVertexBufferBase);
            sData.LineVertexBuffer->SetData(sData.LineVertexBufferBase, dataSize);

            sData.LineShader->Bind();
            RenderCommand::SetLineWidth(sData.LineWidth);
            RenderCommand::DrawLines(sData.LineVertexArray, sData.LineVertexCount);
            ++sData.Stats.DrawCalls;
        }
    }

    void Renderer::StartBatch()
    {
        sData.Stats.QuadCount = 0;

        sData.IndexCount = 0;
        sData.VertexBufferPtr = sData.VertexBufferBase;

        sData.LineVertexCount = 0;
        sData.LineVertexBufferPtr = sData.LineVertexBufferBase;

        sData.TextureSlotIndex = 1;
    }

    void Renderer::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer::DrawCube(const glm::mat4& transform, float textureIndex, const glm::vec4& color)
    {
        PROFILE_FUNCTION();

        size_t quadVertexCount = 4;

        if (sData.IndexCount >= RendererStorage::MaxIndices)
        {
            NextBatch();
        }

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            sData.VertexBufferPtr->Position = transform * sData.VertexPositions[i];
            sData.VertexBufferPtr->TexCoord = sData.TextureCoords[i];
            sData.VertexBufferPtr->Color = color;
            sData.VertexBufferPtr->TexIndex = textureIndex;
            ++sData.VertexBufferPtr;
        }

        sData.IndexCount += 6;

        ++sData.Stats.QuadCount;
    }

    void Renderer::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color)
    {
        sData.LineVertexBufferPtr->Position = p0;
        sData.LineVertexBufferPtr->Color = color;
        ++sData.LineVertexBufferPtr;

        sData.LineVertexBufferPtr->Position = p1;
        sData.LineVertexBufferPtr->Color = color;
        ++sData.LineVertexBufferPtr;

        sData.LineVertexCount += 2;
    }

    void Renderer::DrawRect(const glm::mat4& transform, const glm::vec4& color)
    {
        glm::vec3 lineVertices[4];
        for (size_t i = 0; i < 4; i++)
        {
            lineVertices[i] = transform * sData.VertexPositions[i];
        }

        DrawLine(lineVertices[0], lineVertices[1], color);
        DrawLine(lineVertices[1], lineVertices[2], color);
        DrawLine(lineVertices[2], lineVertices[3], color);
        DrawLine(lineVertices[3], lineVertices[0], color);
    }

    float Renderer::GetTextureIndex(const Ref<Texture>& texture)
    {
        if (!texture)
        {
            return 0.0f;
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < sData.TextureSlotIndex; ++i)
        {
            if (*sData.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (sData.TextureSlotIndex >= RendererStorage::MaxTextureSlots)
            {
                NextBatch();
            }

            textureIndex = (float)sData.TextureSlotIndex;
            sData.TextureSlots[sData.TextureSlotIndex] = texture;
            ++sData.TextureSlotIndex;
        }

        return textureIndex;
    }

    void Renderer::SetLineWidth(float width)
    {
        sData.LineWidth = width;
    }

    void Renderer::ResetStats()
    {
        memset(&sData.Stats, 0, sizeof(Statistics));
    }

    Renderer::Statistics Renderer::GetStats()
    {
        return sData.Stats;
    }

    void Renderer::ResizeWindow(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
