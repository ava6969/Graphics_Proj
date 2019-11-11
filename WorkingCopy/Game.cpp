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


<<<<<<< HEAD
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
=======
>>>>>>> DeweBranch

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
    LoadShaders();
    //CreateMatrices();
    CreateBasicGeometry();

	letterCount = 5;

    // Tell the input assembler stage of the pipeline what kind of
    // geometric primitives (points, lines or triangles) we want to draw.  
    // Essentially: "What kind of shape should the GPU draw with our data?"
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // set up the scene light
    flashlight = {
        XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f),	// ambient
        XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),		// diffuse
        XMFLOAT3(0.0f,0.0f,-3.0f),			// position
        30.0f,								// angle
        XMFLOAT3(0.0f, 0.0f, 1.0f),			// direction
        30.0f,								// length
        1.0f								// intensity
    };
    light2 = {
        XMFLOAT4(0.00f, 0.00f, 0.00f, 1.0f),
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
    vertexShader = make_shared < SimpleVertexShader >(device, context);
    vertexShader->LoadShaderFile(L"VertexShader.cso");

    pixelShader = make_shared < SimplePixelShader >(device, context);
    pixelShader->LoadShaderFile(L"PixelShader.cso");

<<<<<<< HEAD
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    device->CreateSamplerState(&sampDesc, &samplerOptions);

    // load the textures and bump maps
    CreateWICTextureFromFile(
        device,
        context,
        L"Textures/Rock.tif",
        //L"Textures/Brick.tif",
        0,
        &textureSRV
    );
    CreateWICTextureFromFile(
        device,
        context,
        L"Textures/RockN.tif",
        //L"Textures/BrickN.tif",
        0,
        &textureNSRV);

    CreateWICTextureFromFile(device, context, L"Textures/Brick.tif", 0, &floorSRV);
    CreateWICTextureFromFile(device, context, L"Textures/BrickN.tif", 0, &floorNSRV);

    defaultMaterial = new Material(vertexShader, pixelShader, samplerOptions, XMFLOAT3(0.955008f, 0.637427f, 0.538163f));
    paint = new Material(vertexShader, pixelShader, samplerOptions, XMFLOAT3(0.07f, 0.07f, 0.07f));
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
    CreateWICTextureFromFile(device, context, L"Textures/Brick.tif", 0, &floorSRV);
    CreateWICTextureFromFile(device, context, L"Textures/BrickN.tif", 0, &floorNSRV);
    CreateWICTextureFromFile(device, context, L"Textures/Paint.tif", 0, &paint->texture);
    CreateWICTextureFromFile(device, context, L"Textures/PaintN.tif", 0, &paint->normalMap);
    CreateWICTextureFromFile(device, context, L"Textures/PaintR.tif", 0, &paint->roughness);
    CreateWICTextureFromFile(device, context, L"Textures/PaintM.tif", 0, &paint->metalness);
    CreateWICTextureFromFile(device, context, L"Textures/Brick2.tif", 0, &brick->texture);
    CreateWICTextureFromFile(device, context, L"Textures/Brick2N.tif", 0, &brick->normalMap);
    CreateWICTextureFromFile(device, context, L"Textures/Brick2R.tif", 0, &brick->roughness);
    CreateWICTextureFromFile(device, context, L"Textures/NonMetal.png", 0, &brick->metalness);
=======
	skyVS = make_shared< SimpleVertexShader >(device, context);
	skyVS->LoadShaderFile(L"VSSky.cso");

	skyPS = make_shared< SimplePixelShader >(device, context);
	skyPS->LoadShaderFile(L"PSSky.cso");

	defaultMaterial = gameFactory->CreateMaterial(L"Textures/Rock.tif", L"Textures/RockN.tif", vertexShader, pixelShader);
	floor = gameFactory->CreateMaterial(L"Textures/Brick.tif", L"Textures/BrickN.tif", vertexShader, pixelShader);
	sky = gameFactory->CreateSkyBox(L"Textures/cubemap.dds", skyVS, skyPS);
>>>>>>> DeweBranch

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

<<<<<<< HEAD

    const char* filename = "Models/sphere.obj";
    Mesh* mesh1 = new Mesh(filename, device);
    Entity* e1 = new Entity(mesh1, defaultMaterial, 1.0f);
    meshes.push_back(mesh1);
    entities.push_back(e1);
    /*
    Entity* e2 = new Entity(mesh1, paint, 1.0f);
    entities.push_back(e2);
    e2->SetTranslation(XMFLOAT3(1.0f, 0.0f, 0.0f));
    Entity* e3 = new Entity(mesh1, brick, 1.0f);
    entities.push_back(e3);
    e3->SetTranslation(XMFLOAT3(2.0f, 0.0f, 0.0f));
    */

    collisionManager->addCollider(e1->GetCollider());

    Vertex vertices[] =
    {
        { XMFLOAT3(+50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
        { XMFLOAT3(+50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(0,10) },
        { XMFLOAT3(-50.0f, -2.0f, +50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,10) },
        { XMFLOAT3(-50.0f, -2.0f, -50.0f), XMFLOAT3(0,1,0), XMFLOAT2(10,0) }
    };

    //Floor
    unsigned int indices[] = { 2,1,0,2,0,3 };

    Mesh* groundMesh = new Mesh(vertices, 4, indices, 6, device);
    Entity* groundEnt = new Entity(groundMesh, brick, 0.0f);
    meshes.push_back(groundMesh);
=======
	auto groundEnt = gameFactory->CreateFloor(floor, 0.0f);
>>>>>>> DeweBranch
    entities.push_back(groundEnt);

    SpawnTreeGrid(40, 40, 8);
	SpawnLetters(0.0f, -1.0f, 30.0f);
	SpawnLetters(10.0f,-1.0f, 10.0f);
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
				auto tree = gameFactory->CreateTree(defaultMaterial, 1.0f);
				tree->SetTag("tree");
                spawnStrenght = 1;
                float offsetX = (rand() % 10 + 1) / 10.0f;
                float offsetZ = (rand() % 10 + 1) / 10.0f;

                offsetX *= step / 2;
                offsetZ *= step / 3;
                collisionManager->addCollider(tree);

                tree->SetTranslation(i + offsetX, -3, j + offsetZ);				
                entities.push_back(tree);

            }
            else {
                spawnStrenght *= spawnWeight;
            }

        }
    }


}

void Game::Destroy(shared_ptr<Entity> objectToDestroy)
{
	// gets a pointer to an object and compares position of the resource to an object in the entity vector, removes object at that position
	int count = 0;
	for (auto itr : entities)
	{
		if (objectToDestroy->GetPosition().x == itr->GetPosition().x &&
			objectToDestroy->GetPosition().y == itr->GetPosition().y &&
			objectToDestroy->GetPosition().z == itr->GetPosition().z)
		{
			auto toDelete = entities[count];
			entities.erase(entities.begin() + count);
			--letterCount;
		}
		++count;
	}
}




void Game::SpawnLetters(float x, float y, float z)
{

	const float y_axis = 20.0f;
	// create a mesh for letters and push to vector of meshes

	auto letter = gameFactory->CreateLetter(defaultMaterial, 1.0f);
	letter->SetTag("letter");
	collisionManager->addCollider(letter);
	XMFLOAT3 SCALE = XMFLOAT3(0.5f, 0.5f, 0.02f);
	letter->SetScale(SCALE);
	letter->SetTranslation(x, y, z);
	entities.push_back(letter);

}

void Game::DrawAText()
{

	// create FPS information text layout
	std::wostringstream outFPS;
	outFPS.precision(6);
	outFPS << "Letters Collected : " << letterCount << " of 5" << std::endl;

	writeFactory->CreateTextLayout(outFPS.str().c_str(), (UINT32)outFPS.str().size(), textFormatFPS.Get(), width, height, &textLayoutFPS);

	 if (textLayoutFPS)
	 {
		 d2Context->BeginDraw();
		 d2Context->DrawTextLayout(D2D1::Point2F(width - 200.0f ,height - 50.0f), textLayoutFPS.Get(), yellowBrush.Get()); // drawtextlayout first param: what point should text be drawn
		 d2Context->EndDraw();

	 }

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

    float x = cos(frameCounter);
    float z = sin(frameCounter);
    //light2.Position = XMFLOAT3(x * 3.0f, 0.0f, z * 3.0f);

    flashlight.Position = camera->GetPosition();

   

    frameCounter = frameCounter + deltaTime;

    // if letter is found
	auto objToRemove = collisionManager->HandlePlayerCollisions("letter");
	if (objToRemove != nullptr)
	{
		Destroy(objToRemove);
	
	}


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
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

    // loop through each mesh
    for (int i = 0; i < entities.size(); i++) {
        // prepare the material by setting the matrices and shaders
        entities[i]->GetMaterial()->GetPixelShader()->SetFloat3("cameraPos", camera->GetPosition());
        entities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), &flashlight, &light2);


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

	// === Sky box drawing ======================
// Draw the sky AFTER everything else to prevent overdraw

// Set up sky states
	context->RSSetState(sky->GetSkyRastState().Get());
	context->OMSetDepthStencilState(sky->GetSkyDepthState().Get(), 0);

	// Grab the data from the box mesh
	auto skyMesh = gameFactory->CreateCubeMesh();
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
	skyPS->SetShaderResourceView("skyTexture", sky->GetSkySRV().Get() );
	skyPS->SetSamplerState("samplerOptions", sky->GetSkySamplerState().Get() );

	// Finally do the actual drawing
	context->DrawIndexed(skyMesh->GetIndexCount(), 0, 0);

	// Reset states for next frame
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	DrawAText();
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