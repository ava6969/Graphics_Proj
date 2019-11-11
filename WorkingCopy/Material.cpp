#include "Material.h"




Material::Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11ShaderResourceView> rough,
	ComPtr<ID3D11ShaderResourceView> metal, ComPtr<ID3D11SamplerState> opt, float shine)
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

Material::Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11SamplerState> opt, float shine)
{
		vertexShader = vertex;
		pixelShader = pixel;
		texture = tex;
		normalMap = norm;
		samplerOptions = opt;
		shininess = shine;

}



void Material::SetVertexShader(shared_ptr<SimpleVertexShader> v)
{
	vertexShader = v;
}



void Material::SetPixelShader(shared_ptr<SimplePixelShader> p)
{
	pixelShader = p;
}

void Material::AddTextureProperties(const wchar_t* file, MATERIAL_FEATURES options)
{
	switch (options)
	{
	case MATERIAL_FEATURES::METALNESS:
	{
		CreateWICTextureFromFile(device, context, file, 0, &metalness);
		break;
	}


	case MATERIAL_FEATURES::NORMAL_MAP:
	{
		CreateWICTextureFromFile(device, context, file, 0, &normalMap);
		break;
	}

	case MATERIAL_FEATURES::ROUGHNESS:
	{
		CreateWICTextureFromFile(device, context, file, 0, &roughness);
		break;
	}
	default:
	{
		CreateWICTextureFromFile(device, context, file, 0, &texture);
		break;
	}

	}
		 
}




