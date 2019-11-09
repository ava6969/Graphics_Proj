#include "Material.h"



Material::Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11SamplerState> opt, float shine)
{
		vertexShader = vertex;
		pixelShader = pixel;
		texture = tex;
		normalMap = norm;
		samplerOptions = opt;
		shininess = shine;
}

shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

ComPtr<ID3D11ShaderResourceView> Material::GetTexture()
{
	return texture;
}

ComPtr<ID3D11ShaderResourceView> Material::GetNormalMap()
{
	return normalMap;
}

ComPtr<ID3D11SamplerState> Material::GetSampler()
{
	return samplerOptions;
}

float Material::GetShininess()
{
	return shininess;
}
