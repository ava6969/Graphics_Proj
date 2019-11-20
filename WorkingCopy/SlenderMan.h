#pragma once
#include "Entity.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <algorithm>

using namespace DirectX;

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
	void IncreaseLevel();
	//static AgroLevels levels;


private:
	Camera* player;
	int agroLevel;
	float timer;
	float* staticAlpha;
	float distance;
	bool proximityCheck;
	XMFLOAT3 lastPos;
	float standingTimer;
	XMFLOAT2 boundsMax;
	XMFLOAT2 boundsMin;
	bool isVisible;

	void Teleport();
	bool CheckLineOfSight();
	void CheckForStatic();
	void CheckForProximity(float deltaTime);
	void CheckStandingStill(float deltaTime);
	void GetDistance();
	void TurnToFacePlayer();
};

