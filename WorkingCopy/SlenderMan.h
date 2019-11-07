#pragma once
#include "Entity.h"
#include "Camera.h"
#include <DirectXMath.h>

struct AgroLevels {
	std::vector<float> minRanges;
	std::vector<float> maxRanges;
	std::vector<float> teleportTimers;
};

class SlenderMan :
	public Entity
{
public:
	SlenderMan(Mesh* m, Material* mat, float rad, Camera* player);
	~SlenderMan();
	bool stopTeleport;

	void Update(float deltaTime);



private:
	Camera* player;
	AgroLevels levels;
	int agroLevel;
	float timer;
	float* staticAlpha;
	float distance;
	bool proximityCheck;
	DirectX::XMFLOAT3 lastPos;
	float standingTimer;

	void Teleport();
	bool CheckLineOfSight();
	void CheckForStatic();
	void CheckForProximity();
	void CheckStandingStill();
	void GetDistance();
};

