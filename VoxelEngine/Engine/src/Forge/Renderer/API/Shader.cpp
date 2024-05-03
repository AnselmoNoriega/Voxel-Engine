#include "pch.h"
#include "Shader.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLShader.h"

namespace Forge
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:   CORE_ASSERT(false, "Renderer API \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return std::make_shared<GLShader>(filepath);
        default:
        {
            CORE_ASSERT(false, "Unknown RenderAPI!");
            return nullptr;
        }
        }

        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:   CORE_ASSERT(false, "Renderer API \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return std::make_shared<GLShader>(name, vertexSrc, fragmentSrc);
        default:
        {
            CORE_ASSERT(false, "Unknown RenderAPI!");
            return nullptr;
        }
        }
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        CORE_ASSERT(!Exists(name), "Shader already exists!");
        mShaders[name] = shader;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        CORE_ASSERT(!Exists(name), "Shader already exists!");
        mShaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        CORE_ASSERT(Exists(name), "Shader does not exists!");
        return mShaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return mShaders.find(name) != mShaders.end();
    }
}