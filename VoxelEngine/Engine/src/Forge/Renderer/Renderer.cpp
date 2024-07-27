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
        QuadVertex* VertexBufferBase = nullptr;
        QuadVertex* VertexBufferPtr = nullptr;

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

    void Renderer::NextBatch()
    {
        Flush();
        StartBatch();
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

    void Renderer::PackageChunk(
        const glm::vec3& distance,
        const glm::vec3& center,
        const glm::vec2& size,
        Ref<Texture> texture,
        bool isRightSide,
        std::vector<std::byte>& vertices
    )
    {
        uint8_t quadVertexCount = 4;
        float textureIndex = GetTextureIndex(texture);

        for (uint8_t i = 0; i < quadVertexCount; ++i)
        {
            QuadVertex vertex;

            if (isRightSide)
            {
                vertex.Position = center + (distance * sData.VertexRLPositions[i]);
                vertex.TexCoord = sData.TextureRLCoords[i] * size;
            }
            else
            {
                vertex.Position = center + (distance * sData.VertexPositions[i]);
                vertex.TexCoord = sData.TextureCoords[i] * size;
            }
            vertex.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            vertex.TexIndex = textureIndex;

            const std::byte* rawData = reinterpret_cast<const std::byte*>(&vertex);
            vertices.insert(vertices.end(), rawData, rawData + sizeof(QuadVertex));
        }
    }

    void Renderer::DrawChunk(const std::vector<std::byte>& vertices)
    {
        PROFILE_FUNCTION();

        uint32_t count = uint32_t(vertices.size() / sizeof(QuadVertex));
        uint32_t indices = count * 1.5f;

        if ((indices + sData.IndexCount) > RendererStorage::MaxIndices) 
        {
            uint32_t extraIndices = (indices + sData.IndexCount) - RendererStorage::MaxIndices;
            uint32_t extraBytes = (extraIndices / 1.5f) * sizeof(QuadVertex);
            uint32_t copyBytes = vertices.size() - extraBytes;
            
            std::memcpy(sData.VertexBufferPtr, vertices.data(), copyBytes);
            
            NextBatch();
            
            std::memcpy(sData.VertexBufferPtr, vertices.data() + copyBytes, extraBytes);
            sData.VertexBufferPtr += extraBytes / sizeof(QuadVertex);
            
            sData.IndexCount += extraIndices;
            sData.Stats.QuadCount += uint32_t(extraIndices / 1.5f);
        }
        else 
        {
            sData.IndexCount += indices;
            std::memcpy(sData.VertexBufferPtr, vertices.data(), vertices.size());
            sData.VertexBufferPtr += count;
            sData.Stats.QuadCount += uint32_t(count / 1.5f);
        }
    }

    void Renderer::DrawRectChunk(const std::vector<std::byte>& vertices)
    {
        std::memcpy(sData.VertexBufferPtr, vertices.data(), vertices.size());
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
            ASSERT(sData.TextureSlotIndex <= RendererStorage::MaxTextureSlots, "Too many textures");

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