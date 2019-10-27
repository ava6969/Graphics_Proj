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
	CollisionManager(Camera* cam);
	~CollisionManager();

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
	void HandlePlayerCollisions();

	// detection method
	bool CircleToCircleCollision(Collider* col1, Collider* col2);
	bool CircleToCircleCollision(Collider* col1, Collider* col2, float * radSum, float* distSqr);

	// collision resolution
	void ResolvePlayerCollision(Collider* other);
private:
	std::vector<Collider*> collidableObjects;
	Camera* player;
};

