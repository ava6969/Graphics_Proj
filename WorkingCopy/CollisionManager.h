#pragma once
#include <vector>
#include "DXCore.h"
#include "Collider.h"
#include "Entity.h"
#include "Camera.h"
#include <DirectXMath.h>

class CollisionManager
{
public:
	CollisionManager();

	void addCollider(Entity ent) {
		if (ent.GetCollider() != NULL) {
			collidableObjects.push_back(ent.GetCollider());
		}
	}
	void addCollider(Collider* col) {

		collidableObjects.push_back(col);
	}

	bool checkOverlap(Entity ent);
	bool checkOverlap(Camera* cam);
	Collider* checkOverlap(Collider* col, float* radSum, float* distSqr);

	// detection method
	bool CircleToCircleCollision(Collider* col1, Collider* col2, float * radSum, float* distSqr);

private:
	std::vector<Collider*> collidableObjects;
};

