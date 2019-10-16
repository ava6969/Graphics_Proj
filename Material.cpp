#include "Material.h"

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	texture = nullptr;
	normalMap = nullptr;
	samplerOptions = 0;
}

Material::Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* norm, ID3D11SamplerState* opt, float shine)
{
	vertexShader = vertex;
	pixelShader = pixel;
	texture = tex;
	normalMap = norm;
	samplerOptions = opt;
	shininess = shine;
}

Material::~Material()
{
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView* Material::GetTexture()
{
	return texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap()
{
	return normalMap;
}

ID3D11SamplerState* Material::GetSampler()
{
	return samplerOptions;
}

float Material::GetShininess()
{
	return shininess;
}
