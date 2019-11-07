#include "SlenderMan.h"

SlenderMan::SlenderMan(Mesh* m, Material* mat, float rad, Camera* player)
	:Entity(
		m,
		mat,
		rad
	)
{
	this->player = player;
	// Don't think a reference to Game is needed
	agroLevel = 0;
	staticAlpha = 0;
	distance = 0;
	proximityCheck = false;
	lastPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	standingTimer = 0;

	levels.maxRanges[0] = 40.0f;
	levels.minRanges[0] = 50.0f;
	levels.teleportTimers[0] = 7.0f;

	levels.maxRanges[1] = 30.0f;
	levels.minRanges[1] = 40.0f;
	levels.teleportTimers[1] = 5.0f;

	levels.maxRanges[2] = 20.0f;
	levels.minRanges[2] = 30.0f;
	levels.teleportTimers[2] = 3.0f;

	levels.maxRanges[3] = 10.0f;
	levels.minRanges[3] = 20.0f;
	levels.teleportTimers[3] = 1.5f;
}

SlenderMan::~SlenderMan()
{
}

void SlenderMan::Update(float deltaTime)
{
	timer += deltaTime;
	GetDistance();
	CheckForProximity();
	CheckForStatic();
	CheckStandingStill();

	// Increase static alpha here

	if (timer >= levels.teleportTimers[agroLevel] && !proximityCheck && !stopTeleport) {
		if (CheckLineOfSight()) {
			Teleport();
			timer = 0;
		}
	}

	if (*staticAlpha >= 1.0f) {
		// Trigger game over
		// Might just want to check this in Game.cpp
		// or see if there are events in C++ like C#
	}
}

void SlenderMan::Teleport()
{
	// Get forward vector then make inverse
	DirectX::XMFLOAT3 behindMem = player->GetDirection();
	behindMem.x *= -1;
	behindMem.y *= -1;
	behindMem.z *= -1;

	DirectX::XMVECTOR behind = DirectX::XMLoadFloat3(&behindMem);

	// Finds angle behind player
	float angle = rand() % 180 - 90;

	// Get the angle quaternion
	DirectX::XMFLOAT4 angleRotMem;
	XMStoreFloat4(&angleRotMem, DirectX::XMQuaternionIdentity());
	DirectX::XMFLOAT3 axisMem = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR axis = DirectX::XMLoadFloat3(&axisMem);
	DirectX::XMVECTOR angleRot;
	XMStoreFloat4(&angleRotMem, DirectX::XMQuaternionMultiply(XMLoadFloat4(&angleRotMem), DirectX::XMQuaternionRotationAxis(axis, angle)));
	angleRot = XMLoadFloat4(&angleRotMem);

	// Get point on unit circle
	DirectX::XMVECTOR target = DirectX::XMVector3Rotate(behind, angleRot);

	float val = rand() % (int)levels.maxRanges[agroLevel] + (int)levels.minRanges[agroLevel];
	target = DirectX::XMVector3Normalize(target);
	// Multiply each element by val
	//target
}
