#include "Material.h"

<<<<<<< HEAD
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
=======

>>>>>>> DeweBranch

Material::Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11SamplerState> opt, float shine)
{
<<<<<<< HEAD
	texture->Release();
	normalMap->Release();
	roughness->Release();
	metalness->Release();
=======
		vertexShader = vertex;
		pixelShader = pixel;
		texture = tex;
		normalMap = norm;
		samplerOptions = opt;
		shininess = shine;
>>>>>>> DeweBranch
}

shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

<<<<<<< HEAD
void Material::SetVertexShader(SimpleVertexShader* v)
{
	vertexShader = v;
}

void Material::SetPixelShader(SimplePixelShader* p)
{
	pixelShader = p;
}

ID3D11ShaderResourceView* Material::GetTexture()
=======
ComPtr<ID3D11ShaderResourceView> Material::GetTexture()
>>>>>>> DeweBranch
{
	return texture;
}

ComPtr<ID3D11ShaderResourceView> Material::GetNormalMap()
{
	return normalMap;
}

<<<<<<< HEAD
ID3D11ShaderResourceView* Material::GetRoughness()
{
	return roughness;
}

ID3D11ShaderResourceView* Material::GetMetalness()
{
	return metalness;
}

ID3D11SamplerState* Material::GetSampler()
=======
ComPtr<ID3D11SamplerState> Material::GetSampler()
>>>>>>> DeweBranch
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
