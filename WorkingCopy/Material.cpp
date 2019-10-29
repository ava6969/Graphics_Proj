#include "Material.h"

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	texture = nullptr;
	normalMap = nullptr;
	metalness = nullptr;
	roughness = nullptr;
	shininess = 0;
	samplerOptions = 0;
}

Material::Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11SamplerState* opt, DirectX::XMFLOAT3 spec)
{
	vertexShader = vertex;
	pixelShader = pixel;
	samplerOptions = opt;
	texture = nullptr;
	normalMap = nullptr;
	metalness = nullptr;
	roughness = nullptr;
	shininess = 0;
	specColor = spec;
}

Material::Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* norm, ID3D11ShaderResourceView* rough, ID3D11ShaderResourceView* metal, ID3D11SamplerState* opt, float shine)
{
	vertexShader = vertex;
	pixelShader = pixel;
	texture = tex;
	normalMap = norm;
	samplerOptions = opt;
	shininess = shine;
	roughness = rough;
	metalness = metal;
}

Material::~Material()
{
	texture->Release();
	normalMap->Release();
	roughness->Release();
	metalness->Release();
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

void Material::SetVertexShader(SimpleVertexShader* v)
{
	vertexShader = v;
}

void Material::SetPixelShader(SimplePixelShader* p)
{
	pixelShader = p;
}

ID3D11ShaderResourceView* Material::GetTexture()
{
	return texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap()
{
	return normalMap;
}

ID3D11ShaderResourceView* Material::GetRoughness()
{
	return roughness;
}

ID3D11ShaderResourceView* Material::GetMetalness()
{
	return metalness;
}

ID3D11SamplerState* Material::GetSampler()
{
	return samplerOptions;
}

float Material::GetShininess()
{
	return shininess;
}

DirectX::XMFLOAT3 Material::GetSpecularColor()
{
	return specColor;
}
