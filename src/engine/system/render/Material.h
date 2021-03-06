#pragma once

#include <vector>

#include "engine/system/render/ConstantBuffer.h"
#include "engine/system/render/RenderCommon.h"
#include "engine/system/render/Texture.h"

namespace ds_render
{
class Material
{
public:
    /**
     * Get the shader program this material uses.
     *
     * @return  ProgramHandle, shader program this material uses.
     */
    ProgramHandle GetProgram() const;

    /**
     * Set the shader program this material uses.
     *
     * @param  program  ProgramHandle, shader program to set.
     */
    void SetProgram(ProgramHandle program);

    /**
     * Add a texture to this material.
     *
     * @param  samplerName  const std::string &, name of sampler in shader to
     * bind this texture to.
     * @param  texture      const Texture &, texture to add to this material.
     */
    void AddTexture(const std::string &samplerName, const Texture &texture);

    /**
     * Get a list sampler names and their corresponding texture.
     *
     * @return  const std::vector<std::pair<std::string, Texture>> &, list of
     * sampler names and their corresponding textures.
     */
    const std::vector<std::pair<std::string, Texture>> &GetTextures();

    /**
     * Add a constant buffer to this material.
     *
     * @param  constantBufferName  const std::string &, name of the constant
     * buffer in shader to bind given constant buffer to.
     * @param  constantBuffer      const ConstantBuffer &, constant buffer to
     * add to material.
     */
    void AddConstantBuffer(const std::string &constantBufferName,
                           const ConstantBuffer &constantBuffer);

private:
    /** Shader program */
    ProgramHandle m_program;
    /** Material textures */
    std::vector<std::pair<std::string, Texture>> m_textures;
    /** Material constant buffer */
    std::vector<std::pair<std::string, ConstantBuffer>> m_constantBuffers;
};
}
