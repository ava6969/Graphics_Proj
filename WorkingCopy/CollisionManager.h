#pragma once
#include <vector>
#include "Collider.h"
#include "Entity.h"
#include "Camera.h"
#include <DirectXMath.h>


class CollisionManager
{
public:

	CollisionManager(shared_ptr<Camera> cam);

	void addCollider(shared_ptr <Entity> ent) {
		if (ent->GetCollider() != nullptr) {
			collidableObjects.push_back(ent);
		}
	}

	bool checkOverlap(shared_ptr < Entity> ent);
	bool checkOverlap(shared_ptr<Camera> cam);
	Collider* checkOverlap(Collider* col, float* radSum, float* distSqr);
	shared_ptr<Entity> HandlePlayerCollisions(const char* tag = nullptr);

	// detection method
	bool CircleToCircleCollision(Collider* col1, Collider* col2);
	bool CircleToCircleCollision(Collider* col1, Collider* col2, float * radSum, float* distSqr);
	bool CircleToSquareCollision(Collider* col1, Collider* col2);

	// collision resolution
	void ResolvePlayerCollision(Collider* other);
private:
	vector<shared_ptr<Entity>> collidableObjects;
	shared_ptr<Camera> player;
};

