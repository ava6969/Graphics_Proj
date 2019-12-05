#include "SlenderMan.h"

static AgroLevels levels;

SlenderMan::SlenderMan(shared_ptr<Mesh> m, shared_ptr < Material> mat, float rad, shared_ptr<Camera> player)
	:Entity(
		m,
		mat,
		rad
	)
{
	this->player = player;
	// Don't think a reference to Game is needed
	agroLevel = 0;
	staticAlpha = 0;
	distance = 0;
	proximityCheck = false;
	lastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	standingTimer = 0;

	levels.maxRanges.push_back(40.0f);
	levels.minRanges.push_back(50.0f);
	levels.teleportTimers.push_back(7.0f);

	levels.maxRanges.push_back(30.0f);
	levels.minRanges.push_back(40.0f);
	levels.teleportTimers.push_back(5.0f);

	levels.maxRanges.push_back(20.0f);
	levels.minRanges.push_back(30.0f);
	levels.teleportTimers.push_back(3.0f);

	levels.maxRanges.push_back(10.0f);
	levels.minRanges.push_back(20.0f);
	levels.teleportTimers.push_back(1.5f);

	levels.maxRanges.push_back(10.0f);
	levels.minRanges.push_back(20.0f);
	levels.teleportTimers.push_back(1.5f);

	levels.maxRanges.push_back(10.0f);
	levels.minRanges.push_back(20.0f);
	levels.teleportTimers.push_back(1.5f);

	// Should be done with a parameter, but we can change this later
	// Just set it to the worlds bounds
	boundsMax = XMFLOAT2(160.0f, 160.0f);
	boundsMin = XMFLOAT2(-160.0f, -160.0f);

	isVisible = false;

	timer = 0.0f;
	stopTeleport = false;

	SetTranslation(0.0f, -2.0f, 0.0f);
}

SlenderMan::~SlenderMan()
{
}

void SlenderMan::Update(float deltaTime)
{
	timer += deltaTime;
	GetDistance();
	CheckForProximity(deltaTime);
	CheckForStatic();
	CheckStandingStill(deltaTime);

	// Increase static alpha here

	if (timer >= levels.teleportTimers[agroLevel] && !proximityCheck && !stopTeleport) {
		if (CheckLineOfSight()) {
			Teleport();
			timer = 0;
		}
	}

	/*if (*staticAlpha >= 1.0f) {
		// Trigger game over
		// Might just want to check this in Game.cpp
		// or see if there are events in C++ like C#
	}*/
}

void SlenderMan::Teleport()
{
	// Get forward vector then make inverse
	XMFLOAT3 behindTemp = player->GetDirection();
	behindTemp.x *= -1;
	behindTemp.y = 0.0f;
	behindTemp.z *= -1;

	XMVECTOR behind = XMLoadFloat3(&behindTemp);

	// Finds angle behind player
	// Issue where numbers are consistent each run
	float angle = rand() % 180 - 90;

	angle = XMConvertToRadians(angle);

	// Get the angle quaternion
	XMFLOAT4 angleRotTemp;
	XMStoreFloat4(&angleRotTemp, XMQuaternionIdentity());
	XMFLOAT3 axisTemp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR axis = XMLoadFloat3(&axisTemp);
	XMVECTOR angleRot;
	XMStoreFloat4(&angleRotTemp, XMQuaternionMultiply(XMLoadFloat4(&angleRotTemp), XMQuaternionRotationAxis(axis, angle)));
	angleRot = XMLoadFloat4(&angleRotTemp);

	// Get point on unit circle
	XMVECTOR target = XMVector3Rotate(behind, angleRot);

	float val = rand() % 10 + (int)levels.minRanges[agroLevel];
	target = XMVector3Normalize(target);
	target *= val;

	XMVECTOR newPosTemp =  XMLoadFloat3(&player->GetPosition()) + target;

	float distanceFromEdge = 10;

	// Need to clamp magnitude
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, newPosTemp);
	newPos.x = std::clamp(newPos.x, boundsMin.x + distanceFromEdge, boundsMax.x - distanceFromEdge);
	newPos.z = std::clamp(newPos.z, boundsMin.y + distanceFromEdge, boundsMax.y - distanceFromEdge);
	newPos.y = -2.0f;
	SetTranslation(newPos);

	//printf("slenderMan teleported to (%f, %f, %f) \n", position.x, position.y, position.z);
	printf("distance btw CAM and SM is %f \n", sqrt( pow(player->GetPosition().x - position.x, 2) +
													pow(player->GetPosition().y - position.y, 2) +
													pow(player->GetPosition().z - position.z, 2)));
	

}

bool SlenderMan::CheckLineOfSight()
{
	if (isVisible) {
		/*
		Raycast stuff goes here
		*/
		
	}
	return true;
}

void SlenderMan::CheckForStatic()
{
	if (CheckLineOfSight()) {
		// Change alpha's static
		//staticAlpha = 
		//return
	}

	XMVECTOR temp = XMVector3Dot(XMVector3Normalize(XMLoadFloat3(&player->GetDirection())), XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&player->GetPosition())));
	XMFLOAT3 temp2;
	XMStoreFloat3(&temp2, temp);
	float dotVal = temp2.x;

	float val = (20.0f / distance) / 200.0f * dotVal;

	// Clamp the magnitude between 0 and 1
	//staticAlpha = 
}

void SlenderMan::CheckForProximity(float deltaTime)
{
	if (distance < 8.0f) {
		proximityCheck = true;

		float val = (1.0f / distance);
		val *= deltaTime;
	}
	else {
		proximityCheck = false;
	}
}

void SlenderMan::CheckStandingStill(float deltaTime)
{
	XMVECTOR lastPosTemp = XMLoadFloat3(&lastPos);
	XMVECTOR playerPositionTemp = XMLoadFloat3(&player->GetPosition());
	if (XMVector3Equal(lastPosTemp, playerPositionTemp)) {
		standingTimer += deltaTime;

		// Causes some weird errors with staticAlpha being null so it's commented out
		/*if (standingTimer >= 5.0f) {
			*staticAlpha += 0.5f * deltaTime;
		}
		else {
			standingTimer = 0.0f;
		}*/
	}
	XMStoreFloat3(&lastPos, playerPositionTemp);
}

void SlenderMan::GetDistance()
{
	XMVECTOR posTemp = XMLoadFloat3(&position);
	XMVECTOR playerPositionTemp = XMLoadFloat3(&player->GetPosition());
	XMVECTOR distanceFromPlayer = posTemp - playerPositionTemp;
	distanceFromPlayer = XMVector3Length(distanceFromPlayer);
	XMFLOAT3 distTemp;
	XMStoreFloat3(&distTemp, distanceFromPlayer);
	distance = distTemp.x;
}

void SlenderMan::TurnToFacePlayer()
{
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotation));
}

void SlenderMan::IncreaseLevel()
{
	agroLevel++;
}


