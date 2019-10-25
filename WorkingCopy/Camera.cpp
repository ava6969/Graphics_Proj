#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	// initialize fields
	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0, 0, -5);
	direction = XMFLOAT3(0, 0, 1);
	rotX = 0;
	rotY = 0;
	debug = false;
}

Camera::Camera(float width, float height)
{
	// initialize fields
	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(
		XMVectorSet(0, 0, -5, 0),		// The position of the "camera"
		XMVectorSet(0, 0, 1, 0),		// Direction the camera is looking
		XMVectorSet(0, 1, 0, 0)));		 // "Up" direction in 3D space (prevents roll));
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,			// Field of View Angle
		(float)width / height,			// Aspect ratio
		0.1f,							// Near clip plane distance
		100.0f)));						// Far clip plane distance
	position = XMFLOAT3(0, 0, -5);
	direction = XMFLOAT3(0, 0, 1);
	rotX = 0;
	rotY = 0;
	debug = false;

	collider = new Collider(0.2f);
}

Camera::~Camera()
{
	delete collider;
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Camera::GetDirection()
{
	XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&direction)));
	return direction;
}

Collider* Camera::GetCollider()
{
	return collider;
}

void Camera::Update(float deltaTime)
{
	// get user input
	CheckForInput(2.5f, deltaTime);

	// calculate the view quaternion
	XMVECTOR view = XMQuaternionRotationRollPitchYaw(rotX, rotY, 0);
	// apply to the forward
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0,0,1,0), view);
	// keep the Y value on the same plane when playing
	if (!debug)
	{
		position.y = 0.0f;
	}
	// get the resulting view matrix
	XMMATRIX result = XMMatrixLookToLH(
		XMLoadFloat3(&position),  // position
		dir,                      // direction
		XMVectorSet(0, 1, 0, 0)); // up
	// store the values
	XMStoreFloat3(&direction, dir);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(result));

	collider->SetCenter(XMFLOAT2(position.x, position.z));
}

void Camera::CheckForInput(float sensitivity, float dt)
{
	// scale the movement unit by delta time for smooth movement
	sensitivity = sensitivity * dt;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		sensitivity *= 2.5f;
	}
	// forward
	if (GetAsyncKeyState('W') & 0x8000)
	{
		XMStoreFloat3(&position, MoveForwardBackwards(sensitivity));
	}
	// backward
	if (GetAsyncKeyState('S') & 0x8000)
	{
		XMStoreFloat3(&position, MoveForwardBackwards(-sensitivity));
	}
	// left
	if (GetAsyncKeyState('A') & 0x8000)
	{
		XMStoreFloat3(&position, MoveLeftRight(sensitivity));
	}
	// right
	if (GetAsyncKeyState('D') & 0x8000)
	{
		XMStoreFloat3(&position, MoveLeftRight(-sensitivity));
	}
	// up
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		XMStoreFloat3(&position, MoveUpDown(sensitivity));
	}
	// down
	if (GetAsyncKeyState('X') & 0x8000)
	{
		XMStoreFloat3(&position, MoveUpDown(-sensitivity));
	}
	// toggle debug cam
	if (GetAsyncKeyState('P') & 0x01)
	{
		debug = !debug;
	}
}

void Camera::ChangeProjection(float width, float height)
{
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,			// Field of View Angle
		(float)width / height,			// Aspect ratio
		0.1f,							// Near clip plane distance
		100.0f)));						// Far clip plane distance
}

void Camera::Rotate(float x, float y)
{
	float sensitivity = 0.005f;
	rotY += (x * sensitivity);
	rotX += (y * sensitivity);
	float pi = 3.1415926535f;
	if (rotX < -pi / 2) rotX = -pi / 2;
	else if (rotX > pi / 2) rotX = pi / 2;
}

void Camera::Move(DirectX::XMFLOAT3 amount)
{

	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), XMLoadFloat3(&amount)));
}

DirectX::XMVECTOR Camera::MoveForwardBackwards(float amount)
{
	// convert to vectors
	XMVECTOR posV = XMLoadFloat3(&position);
	XMVECTOR dirV = XMLoadFloat3(&direction);
	// normalize and reduce direction
	dirV = XMVector3Normalize(dirV) * amount;
	// add direction to position
	return posV + dirV;
}

DirectX::XMVECTOR Camera::MoveLeftRight(float amount)
{
	// convert to vectors
	XMVECTOR posV = XMLoadFloat3(&position);
	XMVECTOR dirV = XMLoadFloat3(&direction);
	// calculate right vector
	XMVECTOR right = XMVector3Cross(dirV, XMVectorSet(0, 1, 0, 0));
	right = XMVector3Normalize(right) * amount;
	// add side direction to position
	return posV + right;
}

DirectX::XMVECTOR Camera::MoveUpDown(float amount)
{
	// add a world up vector to position
	return XMVectorAdd(XMLoadFloat3(&position), XMVectorSet(0, amount, 0, 0));
}
