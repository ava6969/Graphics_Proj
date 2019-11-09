#pragma once


#include <DirectXMath.h>

class Collider
{
public:
	Collider();
	Collider(float r);
	~Collider();

	// getters
	float GetRadius();
	DirectX::XMFLOAT2 GetCenter();

	// setter
	void SetCenter(DirectX::XMFLOAT2 c);


private:
	float radius;
	DirectX::XMFLOAT2 center;
};

