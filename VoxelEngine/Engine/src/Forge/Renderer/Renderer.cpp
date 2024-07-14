#include "pch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "API/VertexArray.h"
#include "API/Shader.h"
#include "API/Texture.h"

#include "API/UniformBuffer.h"

#include "Forge/Renderer/Voxels/Chunk.h"

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
        std::vector<uint32_t> VertexBuffersSize;
        std::vector<QuadVertex*> VertexBuffersBase;
        QuadVertex* VertexBuffersPtr;

        uint32_t LineVertexCount = 0;
        LineVertex* LineVertexBufferBase = nullptr;
        LineVertex* LineVertexBufferPtr = nullptr;

        float LineWidth = 2.0f;

        std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;

        glm::vec3 VertexPositions[5] = { { -0.5,  0.5,  0.5 },
                                         {  0.5,  0.5,  0.5 },
                                         {  0.5, -0.5, -0.5 },
                                         { -0.5, -0.5, -0.5 },
                                         { -0.5,  0.5,  0.5 } };

        glm::vec3 VertexRLPositions[5] = { { 0.0, -0.5,  0.5 },
                                           { 0.0,  0.5,  0.5 },
                                           { 0.0,  0.5, -0.5 },
                                           { 0.0, -0.5, -0.5 },
                                           { 0.0, -0.5,  0.5 } };

        glm::vec2 TextureCoords[4] = { { 1.0f, 1.0f }, { 0.0f, 1.0f },
                                       { 0.0f, 0.0f }, { 1.0f, 0.0f } };

        glm::vec2 TextureRLCoords[4] = { { 0.0f, 0.0f }, { 0.0f, 1.0f },
                                         { 1.0f, 1.0f }, { 1.0f, 0.0f } };

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
                    {ShaderDataType::Float,  "aTexIndex"}
                });
            sData.QuadVertexArray->AddVertexBuffer(sData.QuadVertexBuffer);
            sData.VertexBuffersBase.push_back(new QuadVertex[sData.MaxVertices]);
            sData.VertexBuffersSize.push_back(0);
            sData.VertexBuffersPtr = sData.VertexBuffersBase.back();
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

        for (int i = 0; i < sData.VertexBuffersBase.size(); ++i)
        {
            delete[] sData.VertexBuffersBase[i];
        }
    }

    void Renderer::RenderScene(const EditorCamera& camera)
    {
        PROFILE_FUNCTION();

        sData.CameraBuffer.ViewProjection = camera.GetViewProjection();
        sData.CameraUniformBuffer->SetData(&sData.CameraBuffer, sizeof(RendererStorage::CameraData));

        DrawBatches();
    }

    void Renderer::DrawBatches()
    {
        for (uint32_t i = 0; i < sData.TextureSlotIndex; i++)
        {
            sData.TextureSlots[i]->Bind(i);
        }

        sData.ObjShader->Bind();

        for (int index = 0; index < sData.VertexBuffersBase.size(); ++index)
        {
            sData.QuadVertexBuffer->SetData(sData.VertexBuffersBase[index], sData.VertexBuffersSize[index]);

            RenderCommand::DrawIndexed(sData.QuadVertexArray, sData.IndexCount);
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

    void Renderer::SaveData()
    {
        if (sData.IndexCount)
        {
            ++sData.Stats.DrawCalls;

            sData.VertexBuffersBase.push_back(new QuadVertex[sData.MaxVertices]);
            sData.VertexBuffersSize.push_back(0);
            sData.VertexBuffersPtr = sData.VertexBuffersBase.back();

            sData.IndexCount = 0;
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

    void Renderer::SaveFace(const QuadSpecs& specs, const Ref<Texture>& texture, const glm::vec4& color)
    {
        PROFILE_FUNCTION();

        size_t quadVertexCount = 4;
        float textureIndex = GetTextureIndex(texture);

        if (sData.IndexCount >= RendererStorage::MaxIndices)
        {
            return;
            SaveData();
        }

        for (size_t i = 0; i < quadVertexCount; ++i)
        {
            if (specs.IsRightLeft)
            {
                sData.VertexBuffersPtr->Position = specs.Center + (specs.Distance * sData.VertexRLPositions[i]);
                sData.VertexBuffersPtr->TexCoord = sData.TextureRLCoords[i] * specs.DistanceVec2;
            }
            else
            {
                sData.VertexBuffersPtr->Position = specs.Center + (specs.Distance * sData.VertexPositions[i]);
                sData.VertexBuffersPtr->TexCoord = sData.TextureCoords[i] * specs.DistanceVec2;
            }
            sData.VertexBuffersPtr->Color = color;
            sData.VertexBuffersPtr->TexIndex = textureIndex;
            ++sData.VertexBuffersPtr;
        }

        uint32_t index = sData.VertexBuffersBase.size() - 1;
        sData.VertexBuffersSize[index] = (uint32_t)((uint8_t*)sData.VertexBuffersPtr - (uint8_t*)sData.VertexBuffersBase.back());

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

    void Renderer::DrawRectFaces(const QuadSpecs& specs, const glm::vec4& color)
    {
        glm::vec3 lineVertices[5];
        for (size_t i = 0; i < 5; ++i)
        {
            lineVertices[i] = specs.Center + (specs.Distance * sData.VertexPositions[i]);
        }

        DrawLine(lineVertices[0], lineVertices[1], color);
        DrawLine(lineVertices[1], lineVertices[2], color);
        DrawLine(lineVertices[2], lineVertices[3], color);
        DrawLine(lineVertices[3], lineVertices[0], color);
        DrawLine(lineVertices[4], lineVertices[2], color);
    }

    void Renderer::DrawRLRectFaces(const QuadSpecs& specs, const glm::vec4& color)
    {
        glm::vec3 lineVertices[5];
        for (size_t i = 0; i < 5; ++i)
        {
            lineVertices[i] = specs.Center + (specs.Distance * sData.VertexRLPositions[i]);
        }

        DrawLine(lineVertices[0], lineVertices[1], color);
        DrawLine(lineVertices[1], lineVertices[2], color);
        DrawLine(lineVertices[2], lineVertices[3], color);
        DrawLine(lineVertices[3], lineVertices[0], color);
        DrawLine(lineVertices[4], lineVertices[2], color);
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
                //NextBatch();
                textureIndex = (float)sData.TextureSlotIndex;
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

    Renderer::Statistics Renderer::GetStats()
    {
        return sData.Stats;
    }

    void Renderer::ResizeWindow(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
