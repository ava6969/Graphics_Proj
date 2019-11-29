#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "Camera.h"
#include "SimpleShader.h"


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
        SimpleVertexShader* vs,
        SimplePixelShader* ps,
        ID3D11ShaderResourceView* texture
        );
    ~Emitter();

    void Update(float deltaTime);
    void Draw(ID3D11DeviceContext* context, Camera* cam);

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

    Particle* particles;
    int maxParticle;
    int firstDeadIndex;
    int firstAliveIndex;

    ParticleVertex* partVerts;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;

    ID3D11ShaderResourceView* texture;
    SimpleVertexShader* vs;
    SimplePixelShader* ps;

    void UpdateSingleParticle(float dt, int index);
    void SpawnParticle();

    void CopyParticlesToGPU(ID3D11DeviceContext* context, Camera* camera);
    void CopyOneParticle(int index, Camera* camera);
    DirectX::XMFLOAT3 CalcParticleVertexPosition(int particleIndex, int quadCornerIndex, Camera* camera);


};

