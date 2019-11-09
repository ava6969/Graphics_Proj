#pragma once
#include "Entity.h"
#include "CollisionManager.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"


using namespace DirectX;
class GameFactory
{
private:
	ID3D11DeviceContext* context;
	ID3D11Device* device;
	shared_ptr<Entity> CreateEntityWithFile(const char* file, shared_ptr<Material> material, float pos);
	shared_ptr<Entity> CreateEntityWithVertex(Vertex vertices[], unsigned int indices[], shared_ptr<Material> material, float pos);

public:
	explicit GameFactory(ID3D11Device* _device, ID3D11DeviceContext* _context) :context(_context), device(_device) {}
	shared_ptr<Entity> CreateFloor(shared_ptr<Material> floorMaterial, float pos);
	shared_ptr<Entity> CreateTree(shared_ptr<Material> treeMaterial, float pos);
	shared_ptr<Entity> CreateLetter(shared_ptr<Material> letterMaterial, float pos);
	shared_ptr<CollisionManager> CreateCollisionManager(shared_ptr<Camera> camera);
	shared_ptr<Camera> CreateCamera(float width, float height);
	shared_ptr<Material> CreateMaterial(const wchar_t* file, const wchar_t* fileN, SimpleVertexShader* vShader, SimplePixelShader* pShader);


};

