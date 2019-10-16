#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "SimpleShader.h"

class Material
{
public:
	// constructors / destructor
	Material();
	Material(SimpleVertexShader* vertex, SimplePixelShader* pixel, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* norm, ID3D11SamplerState* opt, float shine);
	~Material();

	// getters
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetNormalMap();
	ID3D11SamplerState* GetSampler();
	float GetShininess();
private:
	// shader pointers
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;
	ID3D11SamplerState* samplerOptions;

	float shininess;
};

