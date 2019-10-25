#include "CollisionManager.h"
using namespace DirectX;
CollisionManager::CollisionManager()
{

}

bool CollisionManager::checkOverlap(Entity ent)
{
	Collider* entCol = ent.GetCollider();
	if (entCol != NULL) {
		float radSum = 0;
		float distSqr = 0;
		Collider* collision = checkOverlap(ent.GetCollider(), &radSum, &distSqr);
		if (collision == NULL) {
			return false;

		}
		
		XMVECTOR myCenter = XMLoadFloat2(&entCol->GetCenter());
		XMVECTOR theirCenter = XMLoadFloat2(&collision->GetCenter());
		XMVECTOR dist = myCenter - theirCenter;
		dist = XMVector2Normalize(dist);
		dist *= radSum;
		XMFLOAT3 offset; 
		XMStoreFloat3(&offset, dist);
		ent.Move(offset);
		return true;
	}
	return false;
}

bool CollisionManager::checkOverlap(Camera* cam)
{
	Collider* entCol = cam->GetCollider();
	if (entCol != NULL) {
		float radSum = 0;
		float distSqr = 0;
		Collider* collision = checkOverlap(entCol, &radSum, &distSqr);
		if (collision == NULL) {
			return false;

		}

		XMVECTOR myCenter = XMLoadFloat2(&entCol->GetCenter());
		XMVECTOR theirCenter = XMLoadFloat2(&collision->GetCenter());
		XMVECTOR dist = myCenter - theirCenter;
		dist = XMVector2Normalize(dist);
		dist *= radSum;
		XMFLOAT3 offset;
		XMStoreFloat3(&offset, dist);
		cam->Move(offset);
		return true;
	}
	return false;
}

Collider* CollisionManager::checkOverlap(Collider* col, float* radSum, float* distSqr)
{

	//TODO: OCTREE
	for (int i = 0; i < collidableObjects.size(); i++) {

		
		if (CircleToCircleCollision(collidableObjects[i], col, radSum, distSqr)) {
			return collidableObjects[i];

		}

	}
	return NULL;
}

bool CollisionManager::CircleToCircleCollision(Collider* col1, Collider* col2, float * radSum, float* distSqr)
{
	// load centers
	XMVECTOR myCenter = XMLoadFloat2(&col1->GetCenter());
		XMVECTOR theirCenter = XMLoadFloat2(&col2->GetCenter());
		// get square distance between
		XMVECTOR vLength = XMVector2LengthSq(myCenter - theirCenter);
	// unpack result
	XMFLOAT2 result;
	XMStoreFloat2(&result, vLength);
	// if the distance sqared is less than the sum of the radii squared, there is a collision
	float rad = col1->GetRadius() + col2->GetRadius();
	*radSum = rad;
	rad *= rad;
	*distSqr = result.x;
	return result.x < rad;
}
