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
	SlenderMan(shared_ptr<Mesh> m, shared_ptr < Material> mat, float rad, shared_ptr<Camera> player);
	~SlenderMan();
	bool stopTeleport;

	void Update(float deltaTime);
	void IncreaseLevel();
	//static AgroLevels levels;
	inline int getAgroLevel() const { return agroLevel; }
	inline void setAgroLevel(int const& agroLevel) { this->agroLevel = agroLevel; }
	float DistancefromPlayer();

private:

	shared_ptr<Camera> player;
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

