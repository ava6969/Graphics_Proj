#include "Collider.h"
using namespace DirectX;

Collider::Collider()
{
	radius = 1;
	center = XMFLOAT2(0, 0);
}

Collider::Collider(float r)
{
	radius = r;
	center = XMFLOAT2(0, 0);
}

Collider::~Collider()
{
}

float Collider::GetRadius()
{
	return radius;
}

DirectX::XMFLOAT2 Collider::GetCenter()
{
	return center;
}

void Collider::SetCenter(DirectX::XMFLOAT2 c)
{
	center = c;
}

bool Collider::CircleToCircleCollision(Collider* other)
{
	// load centers
	XMVECTOR myCenter = XMLoadFloat2(&center);
	XMVECTOR theirCenter = XMLoadFloat2(&other->GetCenter());
	// get square distance between
	XMVECTOR vLength = XMVector2LengthSq(myCenter - theirCenter);
	// unpack result
	XMFLOAT2 result;
	XMStoreFloat2(&result, vLength);
	// if the distance sqared is less than the sum of the radii squared, there is a collision
	return result.x < (radius + other->GetRadius()) * (radius + other->GetRadius());
}
