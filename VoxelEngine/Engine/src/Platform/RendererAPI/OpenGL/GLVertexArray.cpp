#include "pch.h"
#include "GLVertexArray.h"

#include <glad/glad.h>

namespace Forge
{
    namespace Utils
    {
        static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:   return GL_FLOAT;
            case ShaderDataType::Float2:  return GL_FLOAT;
            case ShaderDataType::Float3:  return GL_FLOAT;
            case ShaderDataType::Float4:  return GL_FLOAT;
            case ShaderDataType::Int:     return GL_INT;
            case ShaderDataType::Mat3:    return GL_FLOAT;
            case ShaderDataType::Mat4:    return GL_FLOAT;
            case ShaderDataType::None:
            default:
            {
                CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
            }
            }
        }
    }

    GLVertexArray::GLVertexArray()
    {
        PROFILE_FUNCTION();

        glCreateVertexArrays(1, &mID);
    }

    void GLVertexArray::Bind() const
    {
        PROFILE_FUNCTION();

        glBindVertexArray(mID);
    }

    void GLVertexArray::Unbind() const
    {
        PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)
    {
        PROFILE_FUNCTION();

        CORE_ASSERT(vertexbuffer->GetLayout().GetElements().size(), "VertexBuffer's layout is empty!");

        glBindVertexArray(mID);
        vertexbuffer->Bind();

        for (const auto& element : vertexbuffer->GetLayout())
        {
            switch (element.Type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                glEnableVertexAttribArray(mVertexBufferIndex);
                glVertexAttribPointer(
                    mVertexBufferIndex,
                    element.GetComponentCount(),
                    Utils::ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    vertexbuffer->GetLayout().GetStride(),
                    (const void*)element.Offset
                );
                ++mVertexBufferIndex;
                break;
            }
            case ShaderDataType::Int:
            {
                glEnableVertexAttribArray(mVertexBufferIndex);
                glVertexAttribIPointer(
                    mVertexBufferIndex,
                    element.GetComponentCount(),
                    Utils::ShaderDataTypeToOpenGLBaseType(element.Type),
                    vertexbuffer->GetLayout().GetStride(),
                    (const void*)element.Offset
                );
                ++mVertexBufferIndex;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                int count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; ++i)
                {
                    glEnableVertexAttribArray(mVertexBufferIndex);
                    glVertexAttribPointer(
                        mVertexBufferIndex,
                        count,
                        Utils::ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        vertexbuffer->GetLayout().GetStride(),
                        (const void*)(element.Offset + sizeof(float) * count * i)
                    );
                    ++mVertexBufferIndex;
                }
                break;
            }
            default:
            {
                CORE_ASSERT(false, "Unknown ShaderDataType!");
                break;
            }
            }
        }

        mVertexBuffers.push_back(vertexbuffer);
    }

    void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        PROFILE_FUNCTION();

        glBindVertexArray(mID);
        indexBuffer->Bind();

        mIndexBuffer = indexBuffer;
    }

    GLVertexArray::~GLVertexArray()
    {
        PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &mID);
    }
}