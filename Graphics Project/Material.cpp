#include "Material.h"

Material::Material()
{
    m_vertexShader = 0;
    m_pixelShader = 0;

}

Material::Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* newSRV, ID3D11SamplerState* newSampler)
{
    m_vertexShader = vertex;
    m_pixelShader = pixel;
    m_textureSRV = newSRV;
    m_samplerState = newSampler;
}

