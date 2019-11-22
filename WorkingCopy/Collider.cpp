#include "Collider.h"
using namespace DirectX;

Collider::Collider()
{
	radius = 1;
	center = XMFLOAT2(0, 0);
	type = (types)0;
}

Collider::Collider(float r)
{
	radius = r;
	center = XMFLOAT2(0, 0);
	type = (types)0;
}

Collider::Collider(DirectX::XMFLOAT2 s)
{
	center = XMFLOAT2(0, 0);
	size = s;
	type = (types)1;
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

int Collider::GetType()
{
	return (int)type;
}

DirectX::XMFLOAT2 Collider::GetSize()
{
	return size;
}

void Collider::SetCenter(DirectX::XMFLOAT2 c)
{
	center = c;
}


