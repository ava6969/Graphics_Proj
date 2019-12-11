#include "Game.h"
#include "Vertex.h"
#include "DXCore.h"
#include "WICTextureLoader.h"
#include <sstream>

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
#if defined(DEBUG) || defined(_DEBUG)
    // Do we want a console window?  Probably only in debug mode
    CreateConsoleWindow(500, 120, 32, 120);
    printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
    camera = gameFactory->CreateCamera((float)width, (float)height);
    collisionManager = gameFactory->CreateCollisionManager(camera);
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
	gameFactory = make_shared<GameFactory>(device, context);
	gameOver = false;
	youWin = false;
	LoadShaders();
	CreateEmitters();
	SetupShadows();
	CreateBasicGeometry();
	lightCount = 0;
	letterCount = 0;
	GenerateLights();
	gameFactory->SetUpPostProcess(postProcessRTV, postProcessSRV, postProcessSS, width, height);
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
    vertexShader = make_shared < SimpleVertexShader >(device, context);
    vertexShader->LoadShaderFile(L"VertexShader.cso");

    pixelShader = make_shared < SimplePixelShader >(device, context);
    pixelShader->LoadShaderFile(L"PixelShader.cso");

	shadowVS = make_shared <SimpleVertexShader>(device, context);
	shadowVS->LoadShaderFile(L"ShadowVS.cso");



	defaultMaterial = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.955008f, 0.637427f, 0.538163f));
	paint = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.07f, 0.07f, 0.07f));
	grass = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.01f, 0.01f, 0.01f));
	cabin = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.02f, 0.02f, 0.02f));
	stone = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	tent = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.02f, 0.02f, 0.02f));
	tower = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	truck = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.04f, 0.04f, 0.04f));
	treeMat = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.01f, 0.01f, 0.01f));
	note = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	lamp = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	slendermanMaterial = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	brick = gameFactory->CreateMaterial(vertexShader, pixelShader, XMFLOAT3(0.05f, 0.05f, 0.05f));
	
	// load the textures and bump maps
	defaultMaterial->AddTextureProperties(L"Textures/Copper.tif", MATERIAL_FEATURES::TEXTURE);
	defaultMaterial->AddTextureProperties(L"Textures/CopperN.tif", MATERIAL_FEATURES::NORMAL_MAP);
	defaultMaterial->AddTextureProperties(L"Textures/CopperR.tif", MATERIAL_FEATURES::ROUGHNESS);
	defaultMaterial->AddTextureProperties(L"Textures/CopperM.tif", MATERIAL_FEATURES::METALNESS);

	
	paint->AddTextureProperties(L"Textures/ContainerA.png", MATERIAL_FEATURES::TEXTURE);
	paint->AddTextureProperties(L"Textures/ContainerN.png", MATERIAL_FEATURES::NORMAL_MAP);
	paint->AddTextureProperties(L"Textures/ContainerR.png", MATERIAL_FEATURES::ROUGHNESS);
	paint->AddTextureProperties(L"Textures/ContainerM.png", MATERIAL_FEATURES::METALNESS);

	grass->AddTextureProperties(L"Textures/GrassA.tif", MATERIAL_FEATURES::TEXTURE);
	grass->AddTextureProperties(L"Textures/GrassN.tif", MATERIAL_FEATURES::NORMAL_MAP);
	grass->AddTextureProperties(L"Textures/MaxRough.tif", MATERIAL_FEATURES::ROUGHNESS);
	grass->AddTextureProperties(L"Textures/NonMetal.tif", MATERIAL_FEATURES::METALNESS);

	cabin->AddTextureProperties(L"Textures/WoodCabinA.jpg", MATERIAL_FEATURES::TEXTURE);
	cabin->AddTextureProperties(L"Textures/WoodCabinN.jpg", MATERIAL_FEATURES::NORMAL_MAP);
	cabin->AddTextureProperties(L"Textures/WoodCabinR.jpg", MATERIAL_FEATURES::ROUGHNESS);
	cabin->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	stone->AddTextureProperties(L"Textures/StoneA.jpg", MATERIAL_FEATURES::TEXTURE);
	stone->AddTextureProperties(L"Textures/StoneN.jpg", MATERIAL_FEATURES::NORMAL_MAP);
	stone->AddTextureProperties(L"Textures/MaxRough.tif", MATERIAL_FEATURES::ROUGHNESS);
	stone->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	tent->AddTextureProperties(L"Textures/TentA.jpg", MATERIAL_FEATURES::TEXTURE);
	tent->AddTextureProperties(L"Textures/TentN.jpg", MATERIAL_FEATURES::NORMAL_MAP);
	tent->AddTextureProperties(L"Textures/TentR.png", MATERIAL_FEATURES::ROUGHNESS);
	tent->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	tower->AddTextureProperties(L"Textures/TowerA.png", MATERIAL_FEATURES::TEXTURE);
	tower->AddTextureProperties(L"Textures/TowerN.jpg", MATERIAL_FEATURES::NORMAL_MAP);
	tower->AddTextureProperties(L"Textures/TowerR.png", MATERIAL_FEATURES::ROUGHNESS);
	tower->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	truck->AddTextureProperties(L"Textures/oil-truck.tif", MATERIAL_FEATURES::TEXTURE);
	truck->AddTextureProperties(L"Textures/oil-truckNrml.jpg", MATERIAL_FEATURES::NORMAL_MAP);
	truck->AddTextureProperties(L"Textures/oil-truckR.tif", MATERIAL_FEATURES::ROUGHNESS);
	truck->AddTextureProperties(L"Textures/oil-truckM.tif", MATERIAL_FEATURES::METALNESS);

	treeMat->AddTextureProperties(L"Textures/BarkA.tif", MATERIAL_FEATURES::TEXTURE);
	treeMat->AddTextureProperties(L"Textures/BarkN.tif", MATERIAL_FEATURES::NORMAL_MAP);
	treeMat->AddTextureProperties(L"Textures/BarkR.tif", MATERIAL_FEATURES::ROUGHNESS);
	treeMat->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	note->AddTextureProperties(L"Textures/Note.jpg", MATERIAL_FEATURES::TEXTURE);
	note->AddTextureProperties(L"Textures/CopprN.tif", MATERIAL_FEATURES::NORMAL_MAP);
	note->AddTextureProperties(L"Textures/BarkR.tif", MATERIAL_FEATURES::ROUGHNESS);
	note->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	lamp->AddTextureProperties(L"Textures/LampA.png", MATERIAL_FEATURES::TEXTURE);
	lamp->AddTextureProperties(L"Textures/LampN.png", MATERIAL_FEATURES::NORMAL_MAP);
	lamp->AddTextureProperties(L"Textures/LampR.png", MATERIAL_FEATURES::ROUGHNESS);
	lamp->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	brick->AddTextureProperties(L"Textures/Brick.tif", MATERIAL_FEATURES::TEXTURE);
	brick->AddTextureProperties(L"Textures/BrickN.tif", MATERIAL_FEATURES::NORMAL_MAP);
	brick->AddTextureProperties(L"Textures/BrickR.png", MATERIAL_FEATURES::ROUGHNESS);
	brick->AddTextureProperties(L"Textures/NonMetal.png", MATERIAL_FEATURES::METALNESS);

	slendermanMaterial->AddTextureProperties(L"Textures/slenderman.png", MATERIAL_FEATURES::TEXTURE);


	skyVS = make_shared< SimpleVertexShader >(device, context);
	skyVS->LoadShaderFile(L"VSSky.cso");

	skyPS = make_shared< SimplePixelShader >(device, context);
	skyPS->LoadShaderFile(L"PSSky.cso");

	sky = gameFactory->CreateSkyBox(L"Textures/NightSky.dds", skyVS, skyPS);

	// Post Processing
	postProcessVS = make_shared<SimpleVertexShader>(device, context);
	postProcessVS->LoadShaderFile(L"PostProcessVertexShader.cso");

	postProcessPS = make_shared<SimplePixelShader>(device, context);
	postProcessPS->LoadShaderFile(L"PostProcessPixelShader.cso");
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	auto groundEnt = gameFactory->CreateEntity("Models/Plane.obj" ,grass, 0.0f);
	entities.push_back(groundEnt);

	// shipping container
	auto e1 = gameFactory->CreateEntity("Models/Container.obj", paint, XMFLOAT2(4.6f, 9.4f));
	e1->SetTranslation(-15.0f, 0.0f, 100.0f);
	e1->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
	collisionManager->addCollider(e1);
    entities.push_back(e1);
    
	// cabin
	auto e2 = gameFactory->CreateEntity("Models/Cabin.obj", cabin, XMFLOAT2(5.5f, 7.8f));
	e2->SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	e2->SetTranslation(100.0f, -2.0f, 70.0f);
	collisionManager->addCollider(e2);
	entities.push_back(e2);

	// rock
	auto e3 = gameFactory->CreateEntity("Models/Stone.obj", stone, 6.0f);
	e3->SetScale(XMFLOAT3(3.0f, 3.0f, 3.0f));
	e3->SetTranslation(70.0f, -1.0f, -25.0f);
	collisionManager->addCollider(e3);
	entities.push_back(e3);

	auto rock = gameFactory->CreateEntity("Models/Stone.obj", stone, 1.5f);
	rock->SetScale(XMFLOAT3(0.75f, 0.75f, 0.75f));
	rock->SetTranslation(99.2f, -1.5f, 66.5f);
	collisionManager->addCollider(rock);
	entities.push_back(rock);

	// tent
	auto e4 = gameFactory->CreateEntity("Models/Tent.obj", tent, XMFLOAT2(5.2f,8.3f));
	e4->SetTranslation(-90.0f, -1.9f, 18.0f);
	e4->SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	collisionManager->addCollider(e4);
	entities.push_back(e4);

	// tower
	auto e5 = gameFactory->CreateEntity("Models/Tower.obj", tower, XMFLOAT2(5.5f, 5.5f));
	e5->SetTranslation(-3.0f, -2.0f, 8.0f);
	collisionManager->addCollider(e5);
	entities.push_back(e5);

	// truck
	auto e6 = gameFactory->CreateEntity("Models/Truck.obj", truck, XMFLOAT2(10.0f, 4.3f));
	e6->SetTranslation(-40.0f, -2.1f, -80.0f);
	collisionManager->addCollider(e6);
	entities.push_back(e6);

	// lamp
	auto e7 = gameFactory->CreateEntity("Models/Lamp.obj", lamp, 0.1f);
	e7->SetTranslation(99.3f, -0.1f, 66.3f);
	e7->SetScale(XMFLOAT3(0.25f, 0.25f, 0.25f));
	collisionManager->addCollider(e7);
	entities.push_back(e7);

	slenderman = gameFactory->CreateSlenderman(slendermanMaterial, 1.0f, camera);
	slenderman->SetTranslation(16.0f, 0.0f, 0.0f);
	collisionManager->addCollider(slenderman);
	entities.push_back(slenderman);

	// Walls
	auto wall = gameFactory->CreateEntity("Models/Wall.obj", brick, 0.0f);
	wall->SetTranslation(XMFLOAT3(1.5, 0.0, -1.5f));
	entities.push_back(wall);



    SpawnTreeGrid(150, 150, 8);
	SpawnLetters(-15.0f, 0.0f, 95.3f, XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 3.1415926f / 2.0f));
	SpawnLetters(97.3f, 0.0f, 70.0f, XMQuaternionRotationRollPitchYaw(0.0f, 3.1415926f / 2.0f, 3.1415926f / 2.0f));
	SpawnLetters(70.0f, 0.0f, -30.8f, XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 3.1415926f / 2.0f));
	SpawnLetters(-45.0f, 0.0f, -79.75f, XMQuaternionRotationRollPitchYaw(0.0f, 3.1415926f / 2.0f, 3.1415926f / 2.0f));
	SpawnLetters(-90.0f, 0.0f, 22.0f, XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 3.1415926f / 2.0f));

	skyMesh = gameFactory->CreateSphereMesh();
}

void Game::SetupShadows()
{
	// setup size of the shadow texture
	shadowMapSize = 1024;

	// create the texture for the shadow map
	D3D11_TEXTURE2D_DESC shadowDesc = {};
	shadowDesc.Width = shadowMapSize;
	shadowDesc.Height = shadowMapSize;
	shadowDesc.ArraySize = 1;
	shadowDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowDesc.CPUAccessFlags = 0;
	shadowDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowDesc.MipLevels = 1;
	shadowDesc.MiscFlags = 0;
	shadowDesc.SampleDesc.Count = 1;
	shadowDesc.SampleDesc.Quality = 0;
	shadowDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* shadowTexture;
	device->CreateTexture2D(&shadowDesc, 0, &shadowTexture);

	// create the depth stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDSV);

	// create the srv for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &srvDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();

	// Create the special "comparison" sampler state for shadows
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSampler);

	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000;
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shadowRastDesc, &shadowRasterizer);

	XMStoreFloat4x4(&shadowProjectionMatrix, XMMatrixTranspose(XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,			// Field of View Angle
		(float)width / height,			// Aspect ratio
		0.1f,							// Near clip plane distance
		100.0f)));						// Far clip plane distance
}



//Spawns the trees for the game in a grid
//TODO Random Rotation
/*
@param
int x - neg  bound
int y - pos  bound
int steps - intervals between range [-x,x] and [-z,z] to spawn trees
*/
void Game::SpawnTreeGrid(int x, int y, int step)
{
    float spawnWeight = 1.2;
    float spawnStrenght = 1;
    float spawnChance = 40.0f;
	int count = 5;

    for (int i = -x; i <= x; i += step) {
        for (int j = -y; j <= y; j += step) {
            float spawnSeed = (rand() % 100 + 1) / spawnStrenght;
            if (spawnSeed <= spawnChance) {
				auto tree = gameFactory->CreateTree(treeMat, 1.5f);
				tree->SetTag("tree");
                spawnStrenght = 1;
                float offsetX = (rand() % 10 + 1) / 10.0f;
                float offsetZ = (rand() % 10 + 1) / 10.0f;

                offsetX *= step / 2;
                offsetZ *= step / 3;

				float randAngle = rand() % 360;
				randAngle = XMConvertToRadians(randAngle);
				XMFLOAT4 rot;
				XMStoreFloat4(&rot, XMQuaternionRotationAxis(XMVectorSet(0.0, 1.0, 0.0, 1.0), randAngle));
				tree->SetRotation(rot);
               
                tree->SetTranslation(i + offsetX, -3, j + offsetZ);			
                collisionManager->addCollider(tree);
                entities.push_back(tree);

            }
            else {
                spawnStrenght *= spawnWeight;
            }

        }
    }


}

// Affects the amount of static on the screen
void Game::ChangeStatic(float amount)
{
	staticValue = amount;
}

void Game::Destroy(shared_ptr<Entity> objectToDestroy)
{
	// gets a pointer to an object and compares position of the resource to an object in the entity vector, removes object at that position
	int count = 0;
	for (auto itr : entities)
	{
		if (itr == objectToDestroy)
		{
			auto toDelete = entities[count];
			entities.erase(entities.begin() + count);
			letterCount++;
		}
		++count;
	}
}

void Game::GenerateLights()
{
	// Reset
	lights.clear();

	// set up the scene light
	Light flashlight = gameFactory->CreateSpotlight(XMFLOAT3(0.0f, 0.0f, -3.0f),
													XMFLOAT3(0.0f, 0.0f, 1.0f),
													XMFLOAT3(1.0f, 1.0f, 1.0f),	30.0f,2.0f,30.f);
	lights.push_back(flashlight);

	Light lampLight = gameFactory->CreatePointLight(XMFLOAT3(99.3f, 0.0f, 65.8f), XMFLOAT3(1.0f, 1.0f, 0.0f), 10.0f, 1.0f);
	lights.push_back(lampLight);

	lightCount = lights.size();
	lights.resize(MAX_LIGHTS);


}


void Game::SpawnLetters(float x, float y, float z, XMVECTOR rotation)
{

	const float y_axis = 20.0f;
	// create a mesh for letters and push to vector of meshes
	auto letter = gameFactory->CreateLetter(note, 1.0f);
	letter->SetTag("letter");
	XMFLOAT3 SCALE = XMFLOAT3(0.5f, 0.35f, 0.02f);
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotation);
	letter->SetScale(SCALE);
	letter->SetTranslation(x, y, z);
	letter->SetRotation(rot);
	collisionManager->addCollider(letter);


	entities.push_back(letter);

}

void Game::DrawAText()
{
	// create FPS information text layout
	DXCore::InitializeTextFormats(D2D1::ColorF::ForestGreen, 15);
	std::wostringstream outFPS;
	outFPS.precision(6);
	outFPS << "Letters Collected : " << letterCount << " of 5" << std::endl;

	writeFactory->CreateTextLayout(outFPS.str().c_str(), (UINT32)outFPS.str().size(), textFormatFPS.Get(), width, height, &textLayoutFPS);

	 if (textLayoutFPS)
	 {
		 d2Context->BeginDraw();
		 d2Context->DrawTextLayout(D2D1::Point2F(width - 300 ,height - 50.0f), textLayoutFPS.Get(), Brush.Get()); // drawtextlayout first param: what point should text be drawn
		 d2Context->EndDraw();

	 }
}

void Game::GameOver()
{
	DXCore::InitializeTextFormats(D2D1::ColorF::YellowGreen, 100);
	// create FPS information text layout
	std::wostringstream outFPS;
	outFPS.precision(20);
	outFPS << "GameOver!!!" << std::endl;

	writeFactory->CreateTextLayout(outFPS.str().c_str(), (UINT32)outFPS.str().size(), textFormatFPS.Get(), width, height, &textLayoutFPS);

	if (textLayoutFPS)
	{
		d2Context->BeginDraw();
		d2Context->DrawTextLayout(D2D1::Point2F(width / 2 - 100, height / 2), textLayoutFPS.Get(), Brush.Get()); // drawtextlayout first param: what point should text be drawn
		d2Context->EndDraw();

	}

}

void Game::YouWin()
{
	DXCore::InitializeTextFormats(D2D1::ColorF::YellowGreen, 100);
	// create FPS information text layout
	std::wostringstream outFPS;
	outFPS.precision(20);
	outFPS << "YouWin!!!" << std::endl;

	writeFactory->CreateTextLayout(outFPS.str().c_str(), (UINT32)outFPS.str().size(), textFormatFPS.Get(), width, height, &textLayoutFPS);

	if (textLayoutFPS)
	{
		d2Context->BeginDraw();
		d2Context->DrawTextLayout(D2D1::Point2F(width / 2 - 100, height / 2), textLayoutFPS.Get(), Brush.Get()); // drawtextlayout first param: what point should text be drawn
		d2Context->EndDraw();

	}
}

void Game::CreateEmitters()
{


    // A depth state for the particles
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    device->CreateDepthStencilState(&dsDesc, &particleDepthState);



	//TODO check blend dest for flashlight color
    D3D11_BLEND_DESC blend = {};
    blend.AlphaToCoverageEnable = false;
    blend.IndependentBlendEnable = false;
    blend.RenderTarget[0].BlendEnable = true;
    blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
    blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; 
    blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blend, &particleBlendState_Slender);



	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;


	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	


	device->CreateBlendState(&blend, &particleBlendState_Campfire);

	blend.RenderTarget[0].BlendEnable = false;
	device->CreateBlendState(&blend, &particleBlendState_Ember);




	emitterVS = make_shared < SimpleVertexShader >(device, context);
	emitterVS->LoadShaderFile(L"ParticleVS.cso");

	emitterPS = make_shared < SimplePixelShader >(device, context);
	emitterPS->LoadShaderFile(L"ParticlePS.cso");


    CreateWICTextureFromFileEx(device, context, L"Textures/particle.jpg", 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_FORCE_SRGB, nullptr, &EmitterTexture);

	emitter_Slender = gameFactory->CreateEmitter(200, .05f, 2, XMFLOAT4(.03, .03, .03, 1), XMFLOAT4(.01, .01, .01, 1), XMFLOAT3(0, .1, 0),
		XMFLOAT3(.5, .5, .5), XMFLOAT3(0, .5f, 0), XMFLOAT3(.1f, 0, .1f), XMFLOAT4(.3, .2, .1, 0),XMFLOAT3(0, .3, 0), emitterVS, emitterPS, EmitterTexture);

    emitter_Slender->SetScale(3, 4);

	emitter_Campfire = gameFactory->CreateEmitter(500, .01, 1.5, XMFLOAT4(.63, .32, .27, 1), XMFLOAT4(.83, .52, .27, .7), XMFLOAT3(0, .5, 0),
		XMFLOAT3(.2, .05, .2),XMFLOAT3(0, 0, 0), XMFLOAT3(.02, .02, .02), XMFLOAT4(0, 0, 0, 1), XMFLOAT3(0, .5, 0), emitterVS, emitterPS, EmitterTexture);

	emitter_Campfire->SetScale(.5, .04);

    


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

	if ( slenderman->DistancefromPlayer() <= 10.0f )
	{
		gameOver = true;
	}

	if (letterCount == 1)
	{
		youWin = true;
	}
    // Quit if the escape key is pressed
    if (GetAsyncKeyState(VK_ESCAPE))
        Quit();


    camera->Update(deltaTime);

	frameCounter = frameCounter + deltaTime;

	// Update lights
	FlashlightBob(frameCounter);
	lights[0].Direction = camera->GetDirection();
	if (camera->GetDebug())
	{
		lights[0].Range = 200.0f;
		lights[0].SpotFalloff = 5.0f;
	}
	else
	{
		lights[0].Range = 30.0f;
		lights[0].SpotFalloff = 50.0f;
	}
    //emitter_Slender->UpdatePosition(XMFLOAT3(.01, 0, 0));
	emitter_Slender->Update(deltaTime);
	XMFLOAT3 partPos = slenderman->GetPosition();
	partPos.y += 2.0f;
	emitter_Slender->SetPosition(partPos);


	emitter_Campfire->Update(deltaTime);

	// update the shadow view matrix
	XMMATRIX viewShadow = XMMatrixLookToLH(
		XMLoadFloat3(&lights[0].Position),  // position
		XMLoadFloat3(&lights[0].Direction), // direction
		XMVectorSet(0, 1, 0, 0)); // up
	XMStoreFloat4x4(&shadowViewMatrix, XMMatrixTranspose(viewShadow));

    

    // if letter is found
	auto objToRemove = collisionManager->HandlePlayerCollisions("letter");
	if (objToRemove != nullptr)
	{
		Destroy(objToRemove);
		slenderman->setAgroLevel(slenderman->getAgroLevel() + 1); // increment 
		printf("Agrolevel is now %d", slenderman->getAgroLevel());
	}

    //float val = sin(frameCounter);
    //entities[0]->SetTranslation(XMFLOAT3(val - 0.5, 0, 0));
    //entities[1]->SetScale(XMFLOAT3(val + 1, val + 1, val + 1));
    //entities[0]->RotateAroundAxis(XMFLOAT3(0.0, 1.0, 0.0), deltaTime * 0.5f);
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->ComputeWorldMatrix();
		entities[i]->CheckForDraw(camera, 4000.0f);
    }

	slenderman->Update(deltaTime);

	XMFLOAT3 playerPos = camera->GetPosition();
	playerPos.x = clamp(playerPos.x, -159.0f, 159.0f);
	playerPos.z = clamp(playerPos.z, -159.0f, 159.0f);
	camera->SetPosition(playerPos);
}

void Game::FlashlightBob(float deltaTime)
{
	float x = sin(deltaTime * 1.5f) * 0.2f;
	float y = sin(deltaTime * 3.0f) * 0.2f;
	XMStoreFloat3(&lights[0].Position, XMLoadFloat3(&camera->GetPosition()) + (XMLoadFloat3(&camera->GetRight()) * 0.5f) + XMVectorSet(x, y, 0, 0));
}

void Game::RenderShadows()
{
	// Initial pipeline setup - No RTV necessary - Clear shadow map
	context->OMSetRenderTargets(0, 0, shadowDSV.Get());
	context->ClearDepthStencilView(shadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(shadowRasterizer.Get());

	// Need to create a viewport that matches the shadow map resolution
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = shadowMapSize;
	viewport.Height = shadowMapSize;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Turn on our shadow map Vertex Shader
	// and turn OFF the pixel shader entirely
	shadowVS->SetShader();
	shadowVS->SetMatrix4x4("view", shadowViewMatrix);
	shadowVS->SetMatrix4x4("projection", shadowProjectionMatrix);
	context->PSSetShader(nullptr, nullptr, 0); // No PS
	

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < entities.size(); i++) {
		// don't try to draw shadows on objects that aren't rendered
		if (!entities[i]->GetDraw()) continue;

		ID3D11Buffer* vb = entities[i]->GetVertexBuffer();
		ID3D11Buffer* ib = entities[i]->GetIndexBuffer();

		// Set buffers in the input assembler
		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);


		shadowVS->SetMatrix4x4("world", entities[i]->GetWorldMatrix());
		shadowVS->CopyAllBufferData();


		// Finally do the actual drawing
		context->DrawIndexed(entities[i]->GetIndexCount(), 0, 0);
	}

	// After rendering the shadow map, go back to the screen
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	viewport.Width = this->width;
	viewport.Height = this->height;
	context->RSSetViewports(1, &viewport);
	context->RSSetState(0);
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Render shadows first
	RenderShadows();

	// Background color (Cornflower Blue in this case) for clearing
	//const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearRenderTargetView(postProcessRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set render target for post processing
	context->OMSetRenderTargets(1, &postProcessRTV, depthStencilView);

	if (gameOver)
	{
		GameOver();
	}
	else if (youWin)
	{
		YouWin();
	}
	else
	{
		// Set lights in the shader
		pixelShader->SetData("Lights", (void*)(&lights[0]), sizeof(Light) * MAX_LIGHTS);
		pixelShader->SetInt("LightCount", lightCount);
		camera->SendPositionToGPU(pixelShader, "CameraPosition");
		pixelShader->CopyBufferData("perFrame");


		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;


		int drawn = 0;
		// loop through each mesh
		for (int i = 0; i < entities.size(); i++) {
			// prepare the material by setting the matrices and shaders in these order
			if (!entities[i]->GetDraw() && i > 10) continue;
			drawn++;
			entities[i]->SendWorldMatrixToGPU(vertexShader, "world");
			camera->SendViewMatrixToGPU(vertexShader, "view");
			camera->SendProjectionMatrixToGPU(vertexShader, "projection");
			camera->SendPositionToGPU(pixelShader, "CameraPosition");
			entities[i]->GetMaterial()->PrepareMaterial(shadowViewMatrix, shadowProjectionMatrix, shadowSRV, shadowSampler);


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
		//printf("Drawn Entities: %i\n", drawn);
		// === Sky box drawing ======================
	// Draw the sky AFTER everything else to prevent overdraw

	// Set up sky states
		context->RSSetState(sky->GetSkyRastState().Get());
		context->OMSetDepthStencilState(sky->GetSkyDepthState().Get(), 0);

		// Grab the data from the box mesh
		ID3D11Buffer* skyVB = skyMesh->GetVertexBuffer();
		ID3D11Buffer* skyIB = skyMesh->GetIndexBuffer();

		// Set buffers in the input assembler
		context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
		context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

		// Set up the new sky shaders
		skyVS->SetMatrix4x4("view", camera->GetViewMatrix());
		skyVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());

		skyVS->CopyAllBufferData();
		skyVS->SetShader();

		skyPS->SetShader();
		skyPS->SetShaderResourceView("skyTexture", sky->GetSkySRV().Get());
		skyPS->SetSamplerState("samplerOptions", sky->GetSkySamplerState().Get());

		// Finally do the actual drawing
		context->DrawIndexed(skyMesh->GetIndexCount(), 0, 0);

		// Reset states for next frame
		context->RSSetState(0);
		context->OMSetDepthStencilState(0, 0);

		//Draw Emitters
		float blend[4] = { 1,1,1,1 };
		context->OMSetBlendState(particleBlendState_Slender.Get(), blend, 0xffffffff);
		context->OMSetDepthStencilState(particleDepthState.Get(), 0);

		emitterPS->CopyAllBufferData();
		emitter_Slender->Draw(context, camera);

		context->OMSetBlendState(particleBlendState_Campfire.Get(), blend, 0xffffffff);
		emitter_Campfire->Draw(context, camera);

		// Reset to default states for next frame
		context->OMSetBlendState(0, blend, 0xffffffff);
		context->OMSetDepthStencilState(0, 0);
		context->RSSetState(0);

		DrawAText();
		// Present the back buffer to the user
		//  - Puts the final frame we're drawing into the window so the user can see it
		//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)

		// Set up for Post Proccessing and send data to Post Processing shaders
		context->OMSetRenderTargets(1, &backBufferRTV, 0);

		// Set up post process shaders
		postProcessVS->SetShader();

		postProcessPS->SetShaderResourceView("Pixels", postProcessSRV.Get());
		postProcessPS->SetSamplerState("Sampler", postProcessSS.Get());
		postProcessPS->SetShader();

		postProcessPS->SetFloat("pixelWidth", 1.0f / width);
		postProcessPS->SetFloat("pixelHeight", 1.0f / height);
		postProcessPS->SetFloat("staticValue", 15.0f);
		postProcessPS->SetFloat("totalTime", totalTime);
		postProcessPS->CopyAllBufferData();

		// Turn OFF vertex and index buffers
		ID3D11Buffer* nothing = 0;
		context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);

		// Draw exactly 3 vertices for our "full screen triangle"
		context->Draw(3, 0);


		// Unbind shader resource views at the end of the frame,
		// since we'll be rendering into one of those textures
		// at the start of the next
		ID3D11ShaderResourceView* nullSRVs[16] = {};
		context->PSSetShaderResources(0, 16, nullSRVs);
	}



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
    if (cameraCanMove)
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