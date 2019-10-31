#include "Game.h"
#include "Vertex.h"
#include "DXCore.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	frameCounter = 0;
	prevMousePos = { 0,0 };
	cameraCanMove = false;
	textureSRV = 0;
	
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	camera = new Camera((float)width, (float)height);
	collisionManager = new CollisionManager(camera);
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}

	delete camera;
	delete defaultMaterial;
	delete floor;
	delete paint;
	delete brick;
	delete collisionManager;
	//textureSRV->Release();
	//textureNSRV->Release();
	floorSRV->Release();
	floorNSRV->Release();
	/*copperMetallic->Release();
	copperRough->Release();*/
	samplerOptions->Release();
	/*paintAlbedo->Release();
	paintNormal->Release();
	paintRough->Release();
	paintMetallic->Release();*/
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	//CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set up the scene light
	flashlight  = {
		XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f),	// ambient
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),		// diffuse
		XMFLOAT3(0.0f,0.0f,-3.0f),			// position
		30.0f,								// angle
		XMFLOAT3(0.0f, 0.0f, 1.0f),			// direction
		30.0f,								// length
		1.0f								// intensity
	};
	light2 = {
		XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f),
		XMFLOAT4(1.0f,1.0f,1.0f, 1.0f),
		XMFLOAT3(1.0f,-1.0f,1.0f),
		1.0f
	};
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &samplerOptions);

	defaultMaterial = new Material(vertexShader, pixelShader, samplerOptions, XMFLOAT3(0.955008f, 0.637427f, 0.538163f));
	paint = new Material(vertexShader, pixelShader, samplerOptions, XMFLOAT3(0.07f,0.07f,0.07f));
	brick = new Material(vertexShader, pixelShader, samplerOptions, XMFLOAT3(0.04f, 0.04f, 0.04f));
	// load the textures and bump maps
	CreateWICTextureFromFile(
		device,
		context,
		L"Textures/Copper.tif",
		//L"Textures/Brick.tif",
		0,
		&defaultMaterial->texture
	);
	CreateWICTextureFromFile(
		device,
		context,
		L"Textures/CopperN.tif",
		//L"Textures/BrickN.tif",
		0,
		&defaultMaterial->normalMap);
	CreateWICTextureFromFile(device, context, L"Textures/CopperR.tif", 0, &defaultMaterial->roughness);
	CreateWICTextureFromFile(device, context, L"Textures/CopperM.tif", 0, &defaultMaterial->metalness);
	CreateWICTextureFromFile(device, context,L"Textures/Brick.tif",0,&floorSRV);
	CreateWICTextureFromFile(device, context, L"Textures/BrickN.tif", 0, &floorNSRV);
	CreateWICTextureFromFile(device, context, L"Textures/Paint.tif", 0, &paint->texture);
	CreateWICTextureFromFile(device, context, L"Textures/PaintN.tif", 0, &paint->normalMap);
	CreateWICTextureFromFile(device, context, L"Textures/PaintR.tif", 0, &paint->roughness);
	CreateWICTextureFromFile(device, context, L"Textures/PaintM.tif", 0, &paint->metalness);
	CreateWICTextureFromFile(device, context, L"Textures/Brick2.tif", 0, &brick->texture);
	CreateWICTextureFromFile(device, context, L"Textures/Brick2N.tif", 0, &brick->normalMap);
	CreateWICTextureFromFile(device, context, L"Textures/Brick2R.tif", 0, &brick->roughness);
	CreateWICTextureFromFile(device, context, L"Textures/NonMetal.png", 0, &brick->metalness);
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	const char* filename = "Models/sphere.obj";
	Mesh* mesh1 = new Mesh(filename, device);
	Entity* e1 = new Entity(mesh1, defaultMaterial, 1.0f);
	meshes.push_back(mesh1);
	entities.push_back(e1);
	Entity* e2 = new Entity(mesh1, paint, 1.0f);
	entities.push_back(e2);
	e2->SetTranslation(XMFLOAT3(1.0f, 0.0f, 0.0f));
	Entity* e3 = new Entity(mesh1, brick, 1.0f);
	entities.push_back(e3);
	e3->SetTranslation(XMFLOAT3(2.0f, 0.0f, 0.0f));

	//collisionManager->addCollider(e1->GetCollider());

	/*Vertex vertices[] =
	{
		{ XMFLOAT3(+50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
		{ XMFLOAT3(+50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,10) },
		{ XMFLOAT3(-50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,10) },
		{ XMFLOAT3(-50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,0) }
	};

	//Floor
	unsigned int indices[] = { 2,1,0,2,0,3 };
	
	Mesh* mesh2 = new Mesh(vertices, 4, indices, 6, device);
	Entity* e2 = new Entity(mesh2, floor, 0.0f);
	meshes.push_back(mesh2);
	entities.push_back(e2);*/
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->ChangeProjection((float)width, (float)height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();


	camera->Update(deltaTime);

	flashlight.Position = camera->GetPosition();
	flashlight.Direction = camera->GetDirection();

	frameCounter = frameCounter + deltaTime;

	collisionManager->HandlePlayerCollisions();

	//float val = sin(frameCounter);
	//entities[0]->SetTranslation(XMFLOAT3(val - 0.5, 0, 0));
	//entities[1]->SetScale(XMFLOAT3(val + 1, val + 1, val + 1));
	//entities[0]->RotateAroundAxis(XMFLOAT3(0.0, 1.0, 0.0), deltaTime * 0.5f);
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->ComputeWorldMatrix();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	//const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// loop through each mesh
	for (int i = 0; i < entities.size(); i++) {
		// prepare the material by setting the matrices and shaders
		entities[i]->GetMaterial()->GetPixelShader()->SetFloat3("cameraPos", camera->GetPosition());
		entities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), &flashlight , &light2);
		
		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		// get a temp variable to access the buffer
		ID3D11Buffer* vTemp = entities[i]->GetVertexBuffer();

		// set the buffers
		context->IASetVertexBuffers(0, 1, &vTemp, &stride, &offset);
		context->IASetIndexBuffer(entities[i]->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			entities[i]->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain effect,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	cameraCanMove = true;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	cameraCanMove = false;
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	float dx = prevMousePos.x - (float)x;
	float dy = prevMousePos.y - (float)y;
	if(cameraCanMove)
		camera->Rotate(-dx, -dy);
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion