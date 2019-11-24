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
#include "SlenderMan.h"
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
	void SetupShadows();
	void DrawAText();
	void SpawnLetters(float x,float y ,float z, XMVECTOR rotation);
    void SpawnTreeGrid(int x, int y, int step);
	void Destroy(shared_ptr<Entity> objectToDestroy);
	void ChangeStatic();		// Increases or decreases static based on slenderman proximity to player
	void RenderShadows();
	void FlashlightBob(float deltaTime);

	std::vector<Light> lights;
	void GenerateLights();
	int lightCount;
	// Letter Stuffs
	int letterCount;
	float staticPercent;		// Controls static effect, may change depending on post processing implementation

	// Wrappers for DirectX shaders to provide simplified functionality
	shared_ptr < SimpleVertexShader > vertexShader;
	shared_ptr< SimplePixelShader> pixelShader;
    shared_ptr<SimpleVertexShader> skyVS;
	shared_ptr<SimplePixelShader> skyPS;
	shared_ptr<SimpleVertexShader> shadowVS;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;


	// array of meshes to load / draw
	vector<shared_ptr<Entity> > entities;

	float frameCounter;

	// Camera pointer
	shared_ptr<Camera> camera;
	bool cameraCanMove;

	// Slenderman pointer
	shared_ptr<SlenderMan> slenderman;

	shared_ptr<Material> defaultMaterial;
	shared_ptr<Material> floor;
	shared_ptr<Material> sky;
	shared_ptr<Material> paint;
	shared_ptr<Material> grass;
	shared_ptr<Material> cabin;
	shared_ptr<Material> stone;
	shared_ptr<Material> tent;
	shared_ptr<Material> tower;
	shared_ptr<Material> truck;
	shared_ptr<Material> treeMat;
	shared_ptr<Material> note;
	shared_ptr<Material> lamp;
	shared_ptr<Material> slendermanMaterial;
	shared_ptr<CollisionManager> collisionManager;

	// skybox mesh
	shared_ptr<Mesh> skyMesh;

	// textures
	ComPtr<ID3D11ShaderResourceView> copperRough;
	ComPtr<ID3D11ShaderResourceView> copperMetallic;
	ComPtr<ID3D11ShaderResourceView> paintAlbedo;
	ComPtr<ID3D11ShaderResourceView> paintNormal;
	ComPtr<ID3D11ShaderResourceView> paintRough;
	ComPtr<ID3D11ShaderResourceView> paintMetallic;
	ComPtr<ID3D11ShaderResourceView> slendermanAlbedo;
	ComPtr<ID3D11ShaderResourceView> slendermanNormal;

	// shadows
	int shadowMapSize;
	ComPtr<ID3D11DepthStencilView> shadowDSV;
	ComPtr<ID3D11ShaderResourceView> shadowSRV;
	ComPtr<ID3D11SamplerState> shadowSampler;
	ComPtr<ID3D11RasterizerState> shadowRasterizer;
	DirectX::XMFLOAT4X4 shadowViewMatrix;
	DirectX::XMFLOAT4X4 shadowProjectionMatrix;
};

