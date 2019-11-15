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
	void CreateBasicGeometry();
	void DrawAText();
	void SpawnLetters(float x,float y ,float z);
    void SpawnTreeGrid(int x, int y, int step);
	void Destroy(shared_ptr<Entity> objectToDestroy);

	std::vector<Light> lights;
	void GenerateLights();
	void DrawLightSources();
	// Letter Stuffs
	int letterCount;

	// Wrappers for DirectX shaders to provide simplified functionality
	shared_ptr < SimpleVertexShader > vertexShader;
	shared_ptr< SimplePixelShader> pixelShader;
    shared_ptr<SimpleVertexShader> skyVS;
	shared_ptr<SimplePixelShader> skyPS;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;


	// array of meshes to load / draw
	vector<shared_ptr<Entity> > entities;

	float frameCounter;

	// Camera pointer
	shared_ptr<Camera> camera;
	bool cameraCanMove;


	shared_ptr<Material> defaultMaterial;
	shared_ptr<Material> floor;
	shared_ptr<Material> sky;
	shared_ptr<Material> paint;
	shared_ptr<Material> brick;
	shared_ptr<Material> cabin;
	shared_ptr<Material> stone;
	shared_ptr<Material> tent;
	shared_ptr<Material> tower;
	shared_ptr<Material> truck;
	shared_ptr<CollisionManager> collisionManager;



	// textures
	ComPtr<ID3D11ShaderResourceView> copperRough;
	ComPtr<ID3D11ShaderResourceView> copperMetallic;
	ComPtr<ID3D11ShaderResourceView> paintAlbedo;
	ComPtr<ID3D11ShaderResourceView> paintNormal;
	ComPtr<ID3D11ShaderResourceView> paintRough;
	ComPtr<ID3D11ShaderResourceView> paintMetallic;

};

