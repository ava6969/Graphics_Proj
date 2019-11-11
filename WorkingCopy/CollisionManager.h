#pragma once
#include <vector>
#include "DXCore.h"
#include "Collider.h"
#include "Entity.h"
#include "Camera.h"
#include "Quadtree.h"
#include <DirectXMath.h>

class CollisionManager
{

private:
    
    std::vector<Collider*> collidableObjects;
    Camera* player;

public:
    Quadtree* quadtree;
	CollisionManager();
	CollisionManager(Camera* cam);
	~CollisionManager();

	void addCollider(Entity ent) {
        if (ent.GetCollider() != NULL) {
            collidableObjects.push_back(ent.GetCollider());
            if (quadtree != NULL) {
                quadtree->CreateQuadtree(collidableObjects);
            }
        }
	}
	void addCollider(Collider* col) {

		collidableObjects.push_back(col);
        if (quadtree != NULL) {
            if (collidableObjects.size() == 1) {
                quadtree->CreateQuadtree(collidableObjects);
            }
            else {
                quadtree->addToTree(col);
            }
            
        }
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

};

