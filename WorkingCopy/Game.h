#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include "CollisionManager.h"
#include "GameFactory.h"




class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();
	shared_ptr<GameFactory> gameFactory;
	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void DrawAText();
	void SpawnLetters(float x,float y ,float z);
    void SpawnTreeGrid(int x, int y, int step);
	void Destroy(shared_ptr<Entity> objectToDestroy);


	// Letter Stuffs
	int letterCount;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;


	// array of meshes to load / draw
	vector<shared_ptr<Entity> > entities;

	float frameCounter;

	// Camera pointer
	shared_ptr<Camera> camera;
	bool cameraCanMove;

	// materials
	shared_ptr<Material> defaultMaterial;
	shared_ptr<Material> floor;

	shared_ptr<CollisionManager> collisionManager;

	// lights
	SpotLight dirLight;
	PointLight light2;


};

