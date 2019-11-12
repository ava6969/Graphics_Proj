#include "SlenderManFactory.h"

std::shared_ptr<Entity> SlenderManFactory::CreateEntityWithFile(const char* file, shared_ptr<Material> material, float pos)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>(file, device);
	return make_shared<Entity>(mesh, material, pos);

}

std::shared_ptr<Entity> SlenderManFactory::CreateEntityWithVertex(Vertex vertices[], unsigned int indices[], shared_ptr<Material> material, float pos)
{
	std::shared_ptr<Mesh> groundMesh = make_shared<Mesh>(vertices, 4, indices, 6, device);
	return make_shared<Entity>(groundMesh, material, pos);
}

std::shared_ptr<Entity> SlenderManFactory::CreateFloor(shared_ptr<Material> floorMaterial, float pos)
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(+50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
		{ XMFLOAT3(+50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,10) },
		{ XMFLOAT3(-50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,10) },
		{ XMFLOAT3(-50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,0) }
	};
	unsigned int indices[] = { 2,1,0,2,0,3 };
	return CreateEntityWithVertex(vertices, indices, floorMaterial, pos);

}

std::shared_ptr<Entity> SlenderManFactory::CreateTree(shared_ptr<Material> material, float pos)
{
	return CreateEntityWithFile("Models/DeadTree.obj", material, pos);
}

std::shared_ptr<Entity> SlenderManFactory::CreateLetter(shared_ptr<Material> letterMaterial, float pos)
{
	// temporary implementation
	// create a mesh for letters and push to vector of meshes
	return CreateEntityWithFile("Models/cube.obj", letterMaterial, pos);
}


std::shared_ptr<CollisionManager> SlenderManFactory::CreateCollisionManager(shared_ptr<Camera> camera)
{
	return make_shared<CollisionManager>(camera);
}



shared_ptr<Camera> SlenderManFactory::CreateCamera(float width, float height)
{
	return make_shared<Camera>(width, height);
}


shared_ptr<Material> SlenderManFactory::CreateMaterial(const wchar_t* file, const wchar_t* fileN, SimpleVertexShader* vShader, SimplePixelShader* pShader)
{

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &samplerOptions);

	// load the textures and bump maps
	CreateWICTextureFromFile(
		device,
		context,
		file,
		0,
		&SRV
	);

	CreateWICTextureFromFile(
		device,
		context,
		fileN,
		0,
		&NSRV);

	// make the material
	return make_shared<Material>(vShader, pShader, SRV, NSRV, samplerOptions, 256.0);
}
