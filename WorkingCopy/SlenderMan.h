#pragma once
#include "Entity.h"
#include "Camera.h"

struct AgroLevel {
	float minRange;
	float maxRange;
	float teleportTimer;
};

class SlenderMan :
	public Entity
{
public:
	SlenderMan(Mesh* m, Material* mat, float rad);
	~SlenderMan();
	bool stopTeleport;

	void IncrementTimer();
	void Teleport();



private:
	Camera* player;
	AgroLevel level[4];
	int agroLevel;
	float timer;
	float staticAlpha;
	float distance;
	bool proximityCheck;
	DirectX::XMFLOAT3 lastPos;
	float standingTimer;
};

