#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "Collider.h"
#include "SimpleShader.h"
#include <memory>

class Camera
{
public:
	// constructor / destructor
	Camera();
	Camera(float width, float height);
	~Camera();

	// getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	DirectX::XMFLOAT3 GetPosition();
	void SetPosition(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetDirection();
	Collider * GetCollider();
	bool GetDebug();
	inline DirectX::XMFLOAT3 GetRight() { return right; }

	void Update(float deltaTime);
	void CheckForInput(float sensitivity, float dt);
	void ChangeProjection(float width, float height);
	void Rotate(float x, float y);
	void Move(DirectX::XMFLOAT3 amount);

	// wrapper for sending GPU DATA
	void SendViewMatrixToGPU(shared_ptr<SimpleVertexShader> vs, const char* name);
	void SendProjectionMatrixToGPU(shared_ptr<SimpleVertexShader> vs, const char* name);
	void SendPositionToGPU(shared_ptr<SimplePixelShader> ps, const char* name);


	// helpers
	DirectX::XMVECTOR MoveForwardBackwards(float amount);
	DirectX::XMVECTOR MoveLeftRight(float amount);
	DirectX::XMVECTOR MoveUpDown(float amount);
private:
	// matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	// vectors
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 right;
	// rotation floats
	float rotX;
	float rotY;
	// debug flag
	bool debug;
	// Collider
	Collider* collider;
};

