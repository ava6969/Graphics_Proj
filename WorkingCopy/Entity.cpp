#include "Entity.h"
using namespace DirectX;

Entity::Entity()
{

	// set the initial world matrix
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	// set initial scale and position
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	// set the initial rotation quaternion
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	mesh = make_shared<Mesh>();
	isDirty = true;
}

Entity::Entity(std::shared_ptr<Mesh> m, std::shared_ptr < Material> mat, float rad)
{
	// set the initial world matrix
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	// set initial scale and position
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// set the initial rotation quaternion
	XMStoreFloat4(&rotation, XMQuaternionIdentity());

	// set the mesh pointer
	mesh = m;

	isDirty = true;

	material = mat;

	collider = new Collider(rad);
}

Entity::Entity(shared_ptr<Mesh> m, shared_ptr<Material> mat, DirectX::XMFLOAT2 s)
{
	// set the initial world matrix
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	// set initial scale and position
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// set the initial rotation quaternion
	XMStoreFloat4(&rotation, XMQuaternionIdentity());

	// set the mesh pointer
	mesh = m;

	isDirty = true;

	material = mat;

	collider = new Collider(s);
}

Entity::~Entity()
{
	delete collider;
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return worldMatrix;
}

void Entity::SetTranslation(DirectX::XMFLOAT3 tran)
{
	position = tran;

	SetDirtyMatrix();
}

void Entity::SetScale(DirectX::XMFLOAT3 sc)
{
	scale = sc;
	SetDirtyMatrix();
}

void Entity::SetRotation(DirectX::XMFLOAT4 rot)
{
	rotation = rot;
	SetDirtyMatrix();
}

void Entity::RotateAroundAxis(DirectX::XMFLOAT3 axis, float angle)
{
	XMStoreFloat4(&rotation, XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle)));
	SetDirtyMatrix();
}

void Entity::Move(DirectX::XMFLOAT3 amount)
{
	// convert position and the amount to vectors
	// add them
	// store back into the position float
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), XMLoadFloat3(&amount)));

	SetDirtyMatrix();
}
void Entity::MoveForward(float amount)
{
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, amount, 0), XMLoadFloat4(&rotation));

	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), dir));

	SetDirtyMatrix();
}

void Entity::SetDirtyMatrix()
{
	isDirty = true;
}

void Entity::ComputeWorldMatrix()
{
	if (isDirty == false) return;

	XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);

	world = scaling * rot * translation;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));

	isDirty = false;

	collider->SetCenter(XMFLOAT2(position.x, position.z));
}

ID3D11Buffer* Entity::GetVertexBuffer()
{
	return mesh->GetVertexBuffer();
}

ID3D11Buffer* Entity::GetIndexBuffer()
{
	return mesh->GetIndexBuffer();
}

int Entity::GetIndexCount()
{
	return mesh->GetIndexCount();
}

shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

Collider* Entity::GetCollider()
{
    collider->SetCenter(XMFLOAT2(position.x, position.z));
    
	return collider;
}

void Entity::SendWorldMatrixToGPU(shared_ptr<SimpleVertexShader> vs, const char* name)
{
	vs->SetMatrix4x4(name, worldMatrix);
}

//void PrepareMaterial(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj, SpotLight const& light, PointLight const& light2);

