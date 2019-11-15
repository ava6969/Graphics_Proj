#pragma once
#include "Entity.h"
#include "CollisionManager.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include "Lights.h"
#include <wrl/client.h>


using namespace Microsoft::WRL;


using namespace DirectX;
class GameFactory
{
private:
	ID3D11DeviceContext* context;
	ID3D11Device* device;
	shared_ptr<Entity> CreateEntityWithFile(const char* file, shared_ptr<Material> material, float pos);
	shared_ptr<Entity> CreateEntityWithFile(const char* file, shared_ptr<Material> material, DirectX::XMFLOAT2 s);
	shared_ptr<Entity> CreateEntityWithVertex(Vertex vertices[], unsigned int indices[], shared_ptr<Material> material, float pos);

public:
	explicit GameFactory(ID3D11Device* _device, ID3D11DeviceContext* _context) :context(_context), device(_device) {}
	shared_ptr<Entity> CreateFloor(shared_ptr<Material> floorMaterial, float pos);
	shared_ptr<Entity> CreateTree(shared_ptr<Material> treeMaterial, float pos);
	shared_ptr<Entity> CreateLetter(shared_ptr<Material> letterMaterial, float pos);
	shared_ptr<Entity> CreateEntity(const char* file, shared_ptr<Material> material, float pos);
	shared_ptr<Entity> CreateEntity(const char* file, shared_ptr<Material> material, DirectX::XMFLOAT2 s);

	shared_ptr<CollisionManager> CreateCollisionManager(shared_ptr<Camera> camera);
	shared_ptr<Camera> CreateCamera(float width, float height);

	shared_ptr<Material> CreateMaterial(shared_ptr<SimpleVertexShader> vShader, shared_ptr<SimplePixelShader> pShader, XMFLOAT3 specColor);
	shared_ptr<Material> CreateSkyBox(const wchar_t* ddsFile, shared_ptr<SimpleVertexShader> vShader, shared_ptr<SimplePixelShader> pShader);
	inline shared_ptr<Mesh> CreateCubeMesh() { return make_shared<Mesh>("Models/cube.obj", device); }
	inline shared_ptr<Mesh> CreateSphereMesh() { return make_shared<Mesh>("Models/sphere.obj", device); }

	Light CreateSpotlight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction , DirectX::XMFLOAT3 color, float range, float intensity, float spotFalloff );
	Light CreatePointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, float range, float intensity);
	Light CreateDirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 color, float intensity);


}; 

