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


