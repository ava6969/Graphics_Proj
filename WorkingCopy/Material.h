#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "SimpleShader.h"

class Material
{
public:
	// constructors / destructor
	Material();
	Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11SamplerState* opt, DirectX::XMFLOAT3 spec);
	Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* norm, ID3D11ShaderResourceView* rough, ID3D11ShaderResourceView* metal, ID3D11SamplerState* opt, float shine);
	~Material();

	// getters
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	void SetVertexShader(SimpleVertexShader* v);
	void SetPixelShader(SimplePixelShader* p);
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetNormalMap();
	ID3D11ShaderResourceView* GetRoughness();
	ID3D11ShaderResourceView* GetMetalness();
	ID3D11SamplerState* GetSampler();
	float GetShininess();
	DirectX::XMFLOAT3 GetSpecularColor();

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* roughness;
	ID3D11ShaderResourceView* metalness;
private:
	// shader pointers
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	
	ID3D11SamplerState* samplerOptions;

	float shininess;
	DirectX::XMFLOAT3 specColor;
};

