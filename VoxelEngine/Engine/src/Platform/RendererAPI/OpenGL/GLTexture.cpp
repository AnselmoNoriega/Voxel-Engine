#include "pch.h"
#include "GLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace Forge
{
    namespace Utils
    {
        static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
        {
            switch (format)
            {
            case ImageFormat::RGB8:  return GL_RGB8;
            case ImageFormat::RGBA8: return GL_RGBA8;
            default:
            {
                CORE_ASSERT(false, "Invalid Format");
                return 0;
            }
            }
        }

        static GLenum ImageFormatToGLDataFormat(ImageFormat format)
        {
            switch (format)
            {
            case ImageFormat::RGB8:  return GL_RGB;
            case ImageFormat::RGBA8: return GL_RGBA;
            default:
            {
                CORE_ASSERT(false, "Invalid Format");
                return 0;
            }
            }
        }
    }

    GLTexture::GLTexture(const TextureSpecification& specification)
        :mSpecification(specification), mWidth(mSpecification.Width), mHeight(mSpecification.Height)
    {
        PROFILE_FUNCTION();

        GLenum glFormat = Utils::ImageFormatToGLInternalFormat(mSpecification.Format);
        mDataFormat = Utils::ImageFormatToGLDataFormat(mSpecification.Format);

        glCreateTextures(GL_TEXTURE_2D, 1, &mID);
        glTextureStorage2D(mID, 1, glFormat, mWidth, mHeight);

        glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    GLTexture::GLTexture(const  std::filesystem::path& path)
        :mPath(path)
    {
        PROFILE_FUNCTION();

        int width, height, channnels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = nullptr;
        {
            PROFILE_SCOPE("stbi_load - GLTexture::GLTexture(const std::string&)");
            data = stbi_load(path.string().c_str(), &width, &height, &channnels, 0);
        }

        CORE_ASSERT(data, "Failed to load image!");
        mWidth = width;
        mHeight = height;

        GLenum glFormat = 0;
        mDataFormat = 0;
        switch (channnels)
        {
        case 4:
        {
            glFormat = GL_RGBA8;
            mDataFormat = GL_RGBA;
            break;
        }
        case 3:
        {
            glFormat = GL_RGB8;
            mDataFormat = GL_RGB;
            break;
        }
        }

        CORE_ASSERT(glFormat & mDataFormat, "Not supported format!");

        glCreateTextures(GL_TEXTURE_2D, 1, &mID);
        glTextureStorage2D(mID, 1, glFormat, mWidth, mHeight);

        glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    GLTexture::~GLTexture()
    {
        PROFILE_FUNCTION();

        glDeleteTextures(1, &mID);
    }

    void GLTexture::SetData(void* data, uint32_t size)
    {
        PROFILE_FUNCTION();

        uint32_t bpp = mDataFormat == GL_RGBA ? 4 : 3;
        CORE_ASSERT(size == mWidth * mHeight * bpp, "Data must be entire texture!");
        glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
    }

    void GLTexture::Bind(uint32_t slot) const
    {
        PROFILE_FUNCTION();

        glBindTextureUnit(slot, mID);
    }
}