#pragma once


#include <DirectXMath.h>

class Collider
{
public:
	Collider();
	Collider(float r);
	Collider(DirectX::XMFLOAT2 s);
	~Collider();
	bool enabled = true;
	// getters
	float GetRadius();
	DirectX::XMFLOAT2 GetCenter();
	int GetType();
	DirectX::XMFLOAT2 GetSize();

	// setter
	void SetCenter(DirectX::XMFLOAT2 c);


private:
	float radius;
	DirectX::XMFLOAT2 center;
    enum types
    {
        circle = 0,
        rect = 1
    }; 
    types type;
    // 0 = circle, 1 = rectangle
	DirectX::XMFLOAT2 size; // x = x, y = z
};

