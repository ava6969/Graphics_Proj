#include "CollisionManager.h"
using namespace DirectX;


CollisionManager::CollisionManager()
{

}

CollisionManager::CollisionManager(shared_ptr<Camera> cam)
{
  
	player = cam;
}

CollisionManager::~CollisionManager()
{
    delete quad;
}


bool CollisionManager::checkOverlap(shared_ptr<Entity> ent)
{
	Collider* entCol = ent->GetCollider();
	if (entCol != NULL) {
		float radSum = 0;
		float distSqr = 0;
		Collider* collision = checkOverlap(ent->GetCollider(), &radSum, &distSqr);
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
		ent->Move(offset);
		return true;
	}
	return false;
}

bool CollisionManager::checkOverlap(shared_ptr<Camera> cam)
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

		
		if (CircleToCircleCollision(collidableObjects[i]->GetCollider(), col, radSum, distSqr)) {
			return collidableObjects[i]->GetCollider();

		}

	}
	return NULL;
}

shared_ptr<Entity> CollisionManager::HandlePlayerCollisions(const char* tag )
{
	if (player->GetDebug()) return nullptr;
	int i = 0;

    std::vector<Collider* > treeColliders = quad->GetCollidableObjects(player->GetCollider());

	for (auto itr : treeColliders)
	{
		// circle collisions
		if (itr->GetType() == 0)
		{
			if (CircleToCircleCollision(player->GetCollider(), itr))
			{
                ResolvePlayerCollision(itr);
                /*
				if (itr->getTag() == tag)
				{
					collidableObjects.erase(collidableObjects.begin() + i);
					return itr;
				}
				else {
					ResolvePlayerCollision(collidableObjects[i]->GetCollider());
				}
                */
			}
		}
		else // AABB collision
		{
			CircleToSquareCollision(player->GetCollider(), itr);
		}
		i++;
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

bool CollisionManager::CircleToSquareCollision(Collider* col1, Collider* col2)
{
	// setup variables
	float testX = col1->GetCenter().x;
	float testY = col1->GetCenter().y;
	float left = col2->GetCenter().x - (col2->GetSize().x / 2.0f);
	float right = col2->GetCenter().x + (col2->GetSize().x / 2.0f);
	float top = col2->GetCenter().y + (col2->GetSize().y / 2.0f);
	float bottom = col2->GetCenter().y - (col2->GetSize().y / 2.0f);
	// get closest edges
	if (col1->GetCenter().x < left) testX = left;
	else if (col1->GetCenter().x > right) testX = right;
	if (col1->GetCenter().y > top) testY = top;
	else if (col1->GetCenter().y < bottom) testY = bottom;
	// get distance between
	XMVECTOR circle = XMLoadFloat2(&col1->GetCenter());
	XMVECTOR square = XMLoadFloat2(&XMFLOAT2(testX, testY));
	XMVECTOR direction = circle - square;
	XMVECTOR dist = XMVector2LengthSq(direction);
	XMFLOAT2 result;
	XMStoreFloat2(&result, dist);
	if (result.x <= col1->GetRadius() * col1->GetRadius())
	{
		direction = XMVector2Normalize(direction);
		direction = direction * col1->GetRadius();
		XMFLOAT2 unpackedResult;
		XMStoreFloat2(&unpackedResult, direction);
		// move camera
		player->SetPosition(XMFLOAT3(unpackedResult.x + testX, 0.0, unpackedResult.y + testY));
		return true;
	}
	return false;
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
