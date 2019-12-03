#pragma once
#include "Entity.h"
#include <cstdlib>

using namespace std;

class SlenderMan2: public Entity
{
	shared_ptr<Camera> player;
	XMFLOAT2 groundWidthHeight;
	int agroLevel;
	float GetDistanceXFromPlayer();
	XMFLOAT2 maxZBoundMap();
	XMFLOAT2 maxXBoundMap();
	XMFLOAT2 minBoundMap();
	void Teleport();
	
public:
	SlenderMan2(shared_ptr<Mesh> m, shared_ptr < Material> mat, float rad, shared_ptr<Camera> player, XMFLOAT2 groundWidthHeight );
	
};

