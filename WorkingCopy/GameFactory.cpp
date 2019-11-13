#include "GameFactory.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

shared_ptr<Entity> GameFactory::CreateEntityWithFile(const char* file, shared_ptr<Material> material, float pos)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>(file, device);
	return make_shared<Entity>(mesh, material, pos);

}

shared_ptr<Entity> GameFactory::CreateEntityWithFile(const char* file, shared_ptr<Material> material, DirectX::XMFLOAT2 s)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>(file, device);
	return make_shared<Entity>(mesh, material, s);
}

shared_ptr<Entity> GameFactory::CreateEntityWithVertex(Vertex vertices[], unsigned int indices[], shared_ptr<Material> material, float pos)
{
	shared_ptr<Mesh> groundMesh = make_shared<Mesh>(vertices, 4, indices, 6, device);
	return make_shared<Entity>(groundMesh, material, pos);
}

shared_ptr<Entity> GameFactory::CreateFloor(shared_ptr<Material> floorMaterial, float pos)
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(+160.0f, -2.0f, -160.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
		{ XMFLOAT3(+160.0f, -2.0f, +160.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,30) },
		{ XMFLOAT3(-160.0f, -2.0f, +160.0f), XMFLOAT3(0,1,0), XMFLOAT2(30,30) },
		{ XMFLOAT3(-160.0f, -2.0f, -160.0f), XMFLOAT3(0,1,0), XMFLOAT2(30,0) }
	};
	unsigned int indices[] = { 2,1,0,2,0,3 };
	return CreateEntityWithVertex(vertices, indices, floorMaterial, pos);

}

shared_ptr<Entity> GameFactory::CreateTree(shared_ptr<Material> material, float pos)
{
	return CreateEntityWithFile("Models/DeadTree.obj", material, pos);
}

shared_ptr<Entity> GameFactory::CreateLetter(shared_ptr<Material> letterMaterial, float pos)
{
	// temporary implementation
	// create a mesh for letters and push to vector of meshes
	return CreateEntityWithFile("Models/cube.obj", letterMaterial, pos);
}

shared_ptr<Entity> GameFactory::CreateEntity(const char* file, shared_ptr<Material> material, float pos)
{
	return CreateEntityWithFile(file, material, pos);
}

shared_ptr<Entity> GameFactory::CreateEntity(const char* file, shared_ptr<Material> material, DirectX::XMFLOAT2 s)
{
	return CreateEntityWithFile(file, material, s);
}


shared_ptr<CollisionManager> GameFactory::CreateCollisionManager(shared_ptr<Camera> camera)
{
	return make_shared<CollisionManager>(camera);
}



shared_ptr<Camera> GameFactory::CreateCamera(float width, float height)
{
	return make_shared<Camera>(width, height);
}


shared_ptr<Material> GameFactory::CreateMaterial(shared_ptr<SimpleVertexShader> vShader, shared_ptr<SimplePixelShader> pShader, XMFLOAT3 specColor )
{

	ComPtr<ID3D11SamplerState> samplerOptions;
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &samplerOptions);
	return make_shared<Material>(context, device, vShader, pShader,samplerOptions, specColor);

}

	
shared_ptr<Material> GameFactory::CreateSkyBox(const wchar_t* ddsFile, shared_ptr<SimpleVertexShader> vShader, shared_ptr<SimplePixelShader> pShader)
{
	ComPtr<ID3D11ShaderResourceView> skySRV;
	ComPtr<ID3D11SamplerState> samplerOptions;
	// Sky stuff
	ComPtr<ID3D11RasterizerState> skyRastState;
	ComPtr<ID3D11DepthStencilState> skyDepthState;

	// Load the skybox and give us an SRV that
	// points to a Texture Cube resource
	CreateDDSTextureFromFile(device, ddsFile, 0, &skySRV);

	// Create a sampler state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &samplerOptions);

	// Sky resources
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rd, &skyRastState);

	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = true;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&ds, &skyDepthState);

	return make_shared<Material>(vShader, pShader, skySRV, samplerOptions, skyRastState, skyDepthState);

}