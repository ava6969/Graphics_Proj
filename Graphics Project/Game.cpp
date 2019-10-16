#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"


// For the DirectX Math library
using namespace DirectX;

unsigned int Game::s_width = 0;
unsigned int Game::s_height = 0;
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
    prevMousePos = { 0,0 };
    meshes = 0;
    Game::s_height = height;
    Game::s_width = width;


    ambientLight.AmbientColor = XMFLOAT4(.1,.1,.1,1);
    ambientLight.DiffuseColor= XMFLOAT4(1, 1, 1, 1);
    ambientLight.Direction = XMFLOAT3(1, -1, 0);


    secondLight.AmbientColor = XMFLOAT4(.1, .1, .1, 1);
    secondLight.DiffuseColor = XMFLOAT4(.3, .7, .1, 1);
    secondLight.Direction = XMFLOAT3(-1, 0, .3);
#if defined(DEBUG) || defined(_DEBUG)
        // Do we want a console window?  Probably only in debug mode
        CreateConsoleWindow(500, 120, 32, 120);
    printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

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
    for (int i = 0; i < meshCount; i++) {
        delete meshes[i];
    }

    delete[] meshes;
    if (meshes != 0) {
       
    }
    for (int i = 0; i < materials.size(); i++) {
        delete materials[i];
    }
 
    tileSRV->Release();
    leatherSRV->Release();
    sampleState->Release();

    metalSRV->Release();

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
    CreateMatrices();
    


     CreateWICTextureFromFile(device, context, L"Assets/Textures/Tile.png", 0, &tileSRV);
     CreateWICTextureFromFile(device, context, L"Assets/Textures/Leather.jpg", 0, &leatherSRV);
     CreateWICTextureFromFile(device, context, L"Assets/Textures/Metal.jpg", 0, &metalSRV);
    D3D11_SAMPLER_DESC samplerDesc = {};

    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
    device->CreateSamplerState(&samplerDesc, &sampleState);
    CreateMaterials();
    CreateBasicGeometry();
    
    // Tell the input assembler stage of the pipeline what kind of
    // geometric primitives (points, lines or triangles) we want to draw.  
    // Essentially: "What kind of shape should the GPU draw with our data?"
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

    cam = Camera();
    cam.SetPosition(0, 0, -5);
    cam.SetCameraMatricies(vertexShader);

}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
    // Create some temporary variables to represent colors
    // - Not necessary, just makes things more readable
    XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

    XMFLOAT3 normal = XMFLOAT3(0, 0, 1);
    XMFLOAT2 uv = XMFLOAT2(0, 0);

    // Set up the vertices of the triangle we would like to draw
    // - We're going to copy this array, exactly as it exists in memory
    //    over to a DirectX-controlled data structure (the vertex buffer)
    Vertex vertices[] =
    {
        { XMFLOAT3(-1.0f, +1.0f, +0.25f), normal, uv },
        { XMFLOAT3(+1.0f, +1.0f, +0.0f), normal, uv},
        { XMFLOAT3(+1.0f, -1.0f, +0.0f), normal, uv },
        { XMFLOAT3(-1.0f, -1.0f, +0.0f), normal, uv },

    };



    Vertex vertices2[] =
    {
        { XMFLOAT3(+1.5f, +1.0f, +0.0f),normal, uv },
        { XMFLOAT3(+2.5f, -1.0f, +0.0f), normal, uv },
        { XMFLOAT3(+0.5f, -1.0f, +0.0f), normal, uv },
        { XMFLOAT3(+0.5f, -0.5f, +0.0f), normal, uv },
    };

    Vertex vertices3[] =
    {
        { XMFLOAT3(-1.0f, +1.0f, +0.0f), normal, uv },
        { XMFLOAT3(+0.5f, +1.0f, +0.0f), normal, uv },
        { XMFLOAT3(-1.0f, -1.8f, +0.0f), normal, uv },
    };




    // Set up the indices, which tell us which vertices to use and in which order
    // - This is somewhat redundant for just 3 vertices (it's a simple example)
    // - Indices are technically not required if the vertices are in the buffer 
    //    in the correct order and each one will be used exactly once
    // - But just to see how it's done...
    int indices[] = { 0, 1, 2, 0,2,1, 0,2,3,0,3,2 };
    int indices2[] = { 0, 1, 2, 2,3,0, 0,2,1,2,0,1 };

    int indices3[] = { 0, 1, 2,0,2,1 };



    meshes = new Mesh * [meshCount];
    //meshes[0] = new Mesh(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(int), device);
    //Tehere is a warning here, but it seems like it is only an issue because meshCount could be set to zero, but it is a stored value that has already been derrived.


    meshes[1] = new Mesh("Assets/Models/cone.obj", device);
    meshes[0] = new Mesh("Assets/Models/cylinder.obj", device);
    meshes[2] = new Mesh("Assets/Models/cube.obj", device);

    

    object1 = Entity(meshes[0], materials[0]);
    object2 = Entity(meshes[1], materials[1]);

    object3 = Entity(meshes[0], materials[1]);

    object4 = Entity(meshes[0], materials[2]);
    object5 = Entity(meshes[2], materials[2]);
    object2.SetPosition(1, 1, 0);
    object4.SetPosition(-2, -2, 0);
    object5.SetPosition(2, -1, 1);
    object3.SetPosition(1, -1, 0);
    
    
    

}

void Game::CreateMaterials()
{
    materials.push_back(new Material(vertexShader, pixelShader,tileSRV, sampleState));
    materials.push_back(new Material(vertexShader, pixelShader,metalSRV, sampleState));
    materials.push_back(new Material(vertexShader, pixelShader,leatherSRV, sampleState));
}

void Game::RotateCamera(float deltaX, float deltaY)
{
    cam.Rotate(deltaX / width, -deltaY / height, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
    // Handle base-level DX resize stuff
    DXCore::OnResize();
    Game::s_width = width;
    Game::s_height = height;
    cam.UpdateProjectionMatrix();

}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
    // Quit if the escape key is pressed
    if (GetAsyncKeyState(VK_ESCAPE))
        Quit();



    object2.LookAt(cam);
    object2.Rotate(90, 0, 0);
    object3.Rotate(deltaTime, deltaTime * .5, deltaTime * .25);


    // object5.moveReltave(deltaTime, 0, 0);

     //object1.moveReltave(deltaTime, 0, 0);
     //Forward
    if (GetAsyncKeyState('W') & 0x8000) { cam.moveReltave(deltaTime, 0, 0); }
    if (GetAsyncKeyState('S') & 0x8000) { cam.moveReltave(-deltaTime, 0, 0); }
    //Straif
    if (GetAsyncKeyState('A') & 0x8000) { cam.moveReltave(0, 0, deltaTime); }
    if (GetAsyncKeyState('D') & 0x8000) { cam.moveReltave(0, 0, -deltaTime); }
    //Move up/down
    if (GetAsyncKeyState('E') & 0x8000) { cam.Translate(0, deltaTime, 0); }
    if (GetAsyncKeyState('Q') & 0x8000) { cam.Translate(0, -deltaTime, 0); }

    if (GetAsyncKeyState('I') & 0x8000) {
        cam.UpdateFov(deltaTime * 5);

    }
    if (GetAsyncKeyState('K') & 0x8000) {
        cam.UpdateFov(-deltaTime * 5);
    }

    //cam.Rotate(deltaTime, 0, 0);


}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
    // Background color (Cornflower Blue in this case) for clearing
    const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

    // Clear the render target and depth buffer (erases what's on the screen)
    //  - Do this ONCE PER FRAME
    //  - At the beginning of Draw (before drawing *anything*)
    context->ClearRenderTargetView(backBufferRTV, color);
    context->ClearDepthStencilView(
        depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0);

    // Send data to shader variables
    //  - Do this ONCE PER OBJECT you're drawing
    //  - This is actually a complex process of copying data to a local buffer
    //    and then copying that entire buffer to the GPU.  
    //  - The "SimpleShader" class handles all of that for you.
    vertexShader->SetMatrix4x4("world", worldMatrix);

    cam.SetCameraMatricies(vertexShader);
  

    pixelShader->SetData("light",
        &ambientLight,
        sizeof(DirectionalLight));

    pixelShader->SetData("secondLight",
        &secondLight,
        sizeof(DirectionalLight));

    
   // pixelShader->SetShader();
    // Once you've set all of the data you care to change for
    // the next draw call, you need to actually send it to the GPU
    //  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!


    // Set the vertex and pixel shaders to use for the next Draw() command
    //  - These don't technically need to be set every frame...YET
    //  - Once you start applying different shaders to different objects,
    //    you'll need to swap the current shaders before each draw



    object1.Draw(context);
    object2.Draw(context);
    object3.Draw(context);
    object4.Draw(context);
    object5.Draw(context);



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
    isMouseDown = true;

    // Save the previous mouse position, so we have it for the future
    prevMousePos.x = x;
    prevMousePos.y = y;
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
    isMouseDown = false;
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
    // Add any custom code here...
    if (isMouseDown) {
        RotateCamera((float)(x - prevMousePos.x), (float)(y - prevMousePos.y));
    }
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