#include "CollisionManager.h"
using namespace DirectX;
CollisionManager::CollisionManager()
{
    quadtree = new Quadtree();
}

CollisionManager::CollisionManager(Camera* cam)
{
	player = cam;
    quadtree = new Quadtree();
}

CollisionManager::~CollisionManager()
{
    delete quadtree;
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

void CollisionManager::HandlePlayerCollisions()
{
	if (player->GetDebug()) return;

    std::vector<Collider* > treeColliders = quadtree->GetCollidableObjects(player->GetCollider());


	for (int i = 0; i < treeColliders.size(); i++)
	{

		if (CircleToCircleCollision(player->GetCollider(), treeColliders[i]))
		{
			ResolvePlayerCollision(treeColliders[i]);
		}
	}
}

bool CollisionManager::CircleToCircleCollision(Collider* col1, Collider* col2)
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
	rad *= rad;
	return result.x < rad;
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

void CollisionManager::ResolvePlayerCollision(Collider* other)
{
	// load centers
	XMVECTOR myCenter = XMLoadFloat2(&player->GetCollider()->GetCenter());
	XMVECTOR theirCenter = XMLoadFloat2(&other->GetCenter());
	// get normalized direction between them
    
	XMVECTOR result = XMVector2Normalize(myCenter - theirCenter);
	
    // scale by overlap ammount
    XMVECTOR distSqr = XMVector2Length(myCenter - theirCenter);
    XMFLOAT2 dist;
    XMStoreFloat2(&dist, distSqr);

    float rad = (player->GetCollider()->GetRadius() + other->GetRadius());
   
    rad = rad - dist.x;
    result *= rad;
	// unpack result
	XMFLOAT2 unpackedResult;
	XMStoreFloat2(&unpackedResult, result);
	// move camera

	player->Move(XMFLOAT3(unpackedResult.x, 0.0, unpackedResult.y));
}
