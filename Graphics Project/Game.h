#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include <vector>
class Game
    : public DXCore
{

public:
    Game(HINSTANCE hInstance);

    ~Game();

    // Overridden setup and game loop methods, which
    // will be called automatically
    void Init();
    void OnResize();
    void Update(float deltaTime, float totalTime);
    void Draw(float deltaTime, float totalTime);

    // Overridden mouse input helper methods
    void OnMouseDown(WPARAM buttonState, int x, int y);
    void OnMouseUp(WPARAM buttonState, int x, int y);
    void OnMouseMove(WPARAM buttonState, int x, int y);
    void OnMouseWheel(float wheelDelta, int x, int y);
    static unsigned int s_width;
    static unsigned int s_height;
private:

    // Initialization helper methods - feel free to customize, combine, etc.
    void LoadShaders();
    void CreateMatrices();
    void CreateBasicGeometry();
    void CreateMaterials();
    void RotateCamera(float deltaX, float deltaY);

    const int meshCount = 3;
    Mesh** meshes = 0;

    DirectionalLight ambientLight;
    DirectionalLight secondLight;

    Entity object1;
    Entity object2;
    Entity object3;
    Entity object4;
    Entity object5;

    Camera cam;
    ID3D11ShaderResourceView* tileSRV;
    ID3D11ShaderResourceView* leatherSRV;
    ID3D11ShaderResourceView* metalSRV;
    ID3D11SamplerState* sampleState;
    



    std::vector<Material*> materials;

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
    bool isMouseDown = false;
};

