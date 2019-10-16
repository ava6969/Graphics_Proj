#pragma once

#include "Vertex.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

class Material
{

public:
    Material();
    Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* newSRV, ID3D11SamplerState* newSampler);
    DirectX::XMFLOAT4 matColor = DirectX::XMFLOAT4(1, 1, 1, 1);
    SimplePixelShader* GetPixel(){
        return m_pixelShader;
    }
    SimpleVertexShader* GetVertex() {
        return m_vertexShader;
    }
    void setVertex(SimpleVertexShader* vertex) {
        m_vertexShader = vertex;
    }

    void setPixel(SimplePixelShader* pixel) {
        m_pixelShader = pixel;
    }

    ID3D11SamplerState* GetSampler() {
        return m_samplerState;
    }
    ID3D11ShaderResourceView* GetSRV() {
        return m_textureSRV;
    }

   
   
private:
  
    SimplePixelShader* m_pixelShader;
    SimpleVertexShader* m_vertexShader;
    ID3D11ShaderResourceView* m_textureSRV;
    ID3D11SamplerState* m_samplerState;
};

