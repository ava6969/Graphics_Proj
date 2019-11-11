#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "SimpleShader.h"
#include <memory>

using namespace std;

class Material
{
public:
	// constructors / destructor
<<<<<<< HEAD
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
=======
	Material() = default;

	explicit Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11SamplerState> opt, float shine);

	explicit Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> _skyRSV, ComPtr<ID3D11SamplerState> opt,
		ComPtr<ID3D11RasterizerState> RastState, ComPtr<ID3D11DepthStencilState> DepthState):
		vertexShader(vertex), pixelShader(pixel), skySRV(_skyRSV),skySamplerOpt(opt), skyRastState(RastState),skyDepthState(DepthState),
		texture(nullptr),normalMap(nullptr),samplerOptions(nullptr),shininess(0){}



	// getters
	shared_ptr<SimpleVertexShader> GetVertexShader();
	shared_ptr<SimplePixelShader> GetPixelShader();
	ComPtr<ID3D11ShaderResourceView> GetTexture();
	ComPtr<ID3D11ShaderResourceView> GetNormalMap();
	ComPtr<ID3D11SamplerState> GetSampler();
	float GetShininess();
	inline ComPtr<ID3D11ShaderResourceView> GetSkySRV() { return skySRV; }
	inline ComPtr<ID3D11RasterizerState> GetSkyRastState() { return skyRastState; }
	inline ComPtr<ID3D11DepthStencilState> GetSkyDepthState() { return skyDepthState; }
	inline ComPtr< ID3D11SamplerState> GetSkySamplerState() { return skySamplerOpt; }

protected:
>>>>>>> DeweBranch
	// shader pointers
	shared_ptr<SimpleVertexShader> vertexShader;
	shared_ptr<SimplePixelShader> pixelShader;

	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11ShaderResourceView> normalMap;
	ComPtr<ID3D11SamplerState> samplerOptions;

<<<<<<< HEAD
	
	ID3D11SamplerState* samplerOptions;
=======
	// Sky stuff
	ComPtr<ID3D11SamplerState> skySamplerOpt;
	ComPtr<ID3D11ShaderResourceView> skySRV;
	ComPtr<ID3D11RasterizerState> skyRastState;
	ComPtr<ID3D11DepthStencilState> skyDepthState;
>>>>>>> DeweBranch

	float shininess;
	DirectX::XMFLOAT3 specColor;
};

