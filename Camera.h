#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

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
	DirectX::XMFLOAT3 GetDirection();

	void Update(float deltaTime);
	void CheckForInput(float sensitivity, float dt);
	void ChangeProjection(float width, float height);
	void Rotate(float x, float y);

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
	// rotation floats
	float rotX;
	float rotY;
};

