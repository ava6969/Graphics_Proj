#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;
	float Intensity;
};

struct PointLight
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Position;
	float Radius;
	float Intensity;
};

struct SpotLight
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Position;
	float Angle;
	DirectX::XMFLOAT3 Direction;
	float MaxLength;
	float Intensity;
};
