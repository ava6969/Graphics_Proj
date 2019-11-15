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
	// new CreateWICTextureFromFileEx doe gamma correction on hardware automaticcaly and faster
	switch (options)
	{
	case MATERIAL_FEATURES::METALNESS:
	{
		CreateWICTextureFromFileEx(device, context, file, 0,D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,0, WIC_LOADER_FORCE_SRGB, nullptr , &metalness);
		break;
	}


	case MATERIAL_FEATURES::NORMAL_MAP:
	{
		CreateWICTextureFromFileEx(device, context, file, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_FORCE_SRGB, nullptr, &normalMap);
		break;
	}

	case MATERIAL_FEATURES::ROUGHNESS:
	{
		CreateWICTextureFromFileEx(device, context, file, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_FORCE_SRGB, nullptr, &roughness);
		break;
	}
	default:
	{
		CreateWICTextureFromFileEx(device, context, file, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_FORCE_SRGB, nullptr, &texture);
		break;
	}

	}
		 
}

void Material::PrepareMaterial()
{
	// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
	// might remove

	if (shininess) pixelShader->SetFloat("shininess", shininess);
	pixelShader->SetFloat3("specularColor", specColor);
	pixelShader->SetShaderResourceView("roughnessMap", roughness.Get());
	pixelShader->SetShaderResourceView("metalnessMap", metalness.Get());
	pixelShader->SetShaderResourceView("diffuseTexture", texture.Get());
	pixelShader->SetShaderResourceView("normalMap", normalMap.Get());
	pixelShader->SetSamplerState("basicSampler", samplerOptions.Get());


	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();
}




