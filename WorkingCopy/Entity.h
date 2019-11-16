#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "Lights.h"
#include "Collider.h"
#include <memory>

using namespace std;
class Entity
{
public:
	Entity();
	Entity(shared_ptr<Mesh> m, shared_ptr < Material> mat, float rad);
	Entity(shared_ptr<Mesh> m, shared_ptr < Material> mat, DirectX::XMFLOAT2 s);
	~Entity();
	
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	void SetTranslation(DirectX::XMFLOAT3 tran);
    void SetTranslation(float x, float y, float z) {
        SetTranslation(DirectX::XMFLOAT3(x, y, z));

    }
	void SetScale(DirectX::XMFLOAT3 sc);
	void SetRotation(DirectX::XMFLOAT4 rot);
	void RotateAroundAxis(DirectX::XMFLOAT3 axis, float angle);
	void Move(DirectX::XMFLOAT3 amount);
	void MoveForward(float amount);
	void SetDirtyMatrix();
	void ComputeWorldMatrix();
	inline void SetTag(const char* tag) { this->tag = tag; };
	void CheckForDraw(std::shared_ptr<Camera> cam, float renderDistance);

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	shared_ptr<Material> GetMaterial();
	Collider* GetCollider();

	void SendWorldMatrixToGPU(shared_ptr<SimpleVertexShader> vs, const char* name);
	inline const char* getTag() { return tag; }
	inline bool GetDraw() { return doRender; }
	inline DirectX::XMFLOAT3 GetPosition() { return position; }


private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotation;
	shared_ptr<Mesh> mesh;
	bool isDirty;
	shared_ptr<Material> material;
	Collider* collider;
	// tag that describes entity
	const char* tag;
	bool doRender;
	
};

