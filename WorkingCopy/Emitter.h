#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Camera.h"
#include "SimpleShader.h"
#include <wrl/client.h>
using namespace Microsoft::WRL;

struct Particle {
    DirectX::XMFLOAT4 Color;
    DirectX::XMFLOAT3 StartPosition;
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 StartVelocity;
    float Size;
    float Age;
    float RotationStart;
    float RotationEnd;
    float Rotation;

};

struct ParticleVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 UV;
    DirectX::XMFLOAT4 Color;
};

class Emitter
{
public:
    Emitter(int maxParticles,
        float timeBetweenSpawn,
        float particleLifetime,
        DirectX::XMFLOAT4 startColor,
        DirectX::XMFLOAT4 endColor,
        DirectX::XMFLOAT3 startVelocity,
        DirectX::XMFLOAT3 velocityRandomRange,
        DirectX::XMFLOAT3 emitterPosition,
        DirectX::XMFLOAT3 positionRandomRange,
        DirectX::XMFLOAT4 rotationRandomRanges,
        DirectX::XMFLOAT3 emitterAcceleration,
        ID3D11Device* device,
        std::shared_ptr<SimpleVertexShader> vs,
        std::shared_ptr<SimplePixelShader> ps,
        ComPtr<ID3D11ShaderResourceView> texture
        );
    ~Emitter();

    void Update(float deltaTime);
    void Draw(ID3D11DeviceContext* context, std::shared_ptr<Camera> cam);

private:
    float spawnTime;
    float timeSinceSpawn;

    int liveParticleCount;
    float maxParticleLifetime;

    DirectX::XMFLOAT2 DefaultUVs[4];

    DirectX::XMFLOAT3 emitterAcceleration;
    DirectX::XMFLOAT3 emitterPosition;
    DirectX::XMFLOAT3 startVelocity;

    DirectX::XMFLOAT3 positionRandomRange;
    DirectX::XMFLOAT3 velocityRandomRange;
    DirectX::XMFLOAT4 rotationRandomRanges; // Min start, max start, min end, max end

    DirectX::XMFLOAT4 startColor;
    DirectX::XMFLOAT4 endColor;
    float startSize;
    float endSize;

    std::unique_ptr<Particle[]> particles;
    int maxParticle;
    int firstDeadIndex;
    int firstAliveIndex;

	std::unique_ptr<ParticleVertex[]> partVerts;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;

    ComPtr<ID3D11ShaderResourceView> texture;
    std::shared_ptr<SimpleVertexShader> vs;
    std::shared_ptr<SimplePixelShader> ps;

    void UpdateSingleParticle(float dt, int index);
    void SpawnParticle();

    void CopyParticlesToGPU(ID3D11DeviceContext* context, std::shared_ptr<Camera> camera);
    void CopyOneParticle(int index, std::shared_ptr<Camera> camera);
    DirectX::XMFLOAT3 CalcParticleVertexPosition(int particleIndex, int quadCornerIndex, std::shared_ptr<Camera> camera);


};

