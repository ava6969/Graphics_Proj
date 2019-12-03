#include "SlenderMan2.h"

float SlenderMan2::GetDistanceXFromPlayer()
{
	XMVECTOR SlenderManPosition = XMLoadFloat3(&position);
	XMVECTOR playerPositionTemp = XMLoadFloat3(&player->GetPosition());
	XMVECTOR distanceFromPlayerV = SlenderManPosition - playerPositionTemp;
	distanceFromPlayerV = XMVector3Length(distanceFromPlayerV);
	XMFLOAT3 distanceFromPlayer;
	XMStoreFloat3(&distanceFromPlayer, distanceFromPlayerV);
	return distanceFromPlayer.x;
}

XMFLOAT2 SlenderMan2::maxZBoundMap()
{
	return XMFLOAT2(groundWidthHeight.y / 2, -groundWidthHeight.y / 2);
}

XMFLOAT2 SlenderMan2::maxXBoundMap()
{
	return XMFLOAT2(groundWidthHeight.x / 2, -groundWidthHeight.x / 2);
}


XMFLOAT2 SlenderMan2::minBoundMap()
{
	return XMFLOAT2();
}

void SlenderMan2::Teleport()
{
	// Teleport in a circle and halt
}
