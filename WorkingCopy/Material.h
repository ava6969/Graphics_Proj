#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include "SimpleShader.h"
#include "WICTextureLoader.h"
#include <memory>
#include "Camera.h"
#include "Lights.h"

using namespace std;
using namespace DirectX;
enum class MATERIAL_FEATURES{ TEXTURE, NORMAL_MAP, ROUGHNESS, METALNESS };

class Material
{
public:
	// constructors / destructor
	Material() = default;

	Material(ID3D11DeviceContext* _context, ID3D11Device* _device, shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11SamplerState> opt, DirectX::XMFLOAT3 spec):
		context(_context),device(_device),vertexShader(vertex),pixelShader(pixel),samplerOptions(opt),specColor(spec),shininess(NULL) {}


	Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11ShaderResourceView> rough,
		ComPtr<ID3D11ShaderResourceView> metal, ComPtr<ID3D11SamplerState> opt, float shine);


	Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> tex, ComPtr<ID3D11ShaderResourceView> norm, ComPtr<ID3D11SamplerState> opt, float shine);


	Material(shared_ptr<SimpleVertexShader> vertex, shared_ptr<SimplePixelShader> pixel, ComPtr<ID3D11ShaderResourceView> _skyRSV, ComPtr<ID3D11SamplerState> opt,
		ComPtr<ID3D11RasterizerState> RastState, ComPtr<ID3D11DepthStencilState> DepthState) :
		vertexShader(vertex), pixelShader(pixel), skySRV(_skyRSV), skySamplerOpt(opt), skyRastState(RastState), skyDepthState(DepthState),
		texture(nullptr), normalMap(nullptr), samplerOptions(nullptr), shininess(0), specColor(DirectX::XMFLOAT3(0.0, 0.0, 0.0)) {}

	// setters // todo chnage to smartPtrs and ComPtrs
	void SetVertexShader(shared_ptr<SimpleVertexShader> v);
	void SetPixelShader(shared_ptr<SimplePixelShader> p);

	void AddTextureProperties(const wchar_t* file, MATERIAL_FEATURES options);


	// getters
	inline shared_ptr<SimpleVertexShader> GetVertexShader() const { return vertexShader; }
	inline shared_ptr<SimplePixelShader> GetPixelShader() const { return pixelShader; }
	inline ComPtr<ID3D11ShaderResourceView> GetTexture() const { return texture; }
	inline ComPtr<ID3D11ShaderResourceView> GetNormalMap() const { return normalMap; }
	inline ComPtr<ID3D11SamplerState> GetSampler() const { return samplerOptions; }
	inline float GetShininess() const { return shininess; }
	inline ComPtr<ID3D11ShaderResourceView> GetSkySRV() const { return skySRV; }
	inline ComPtr<ID3D11RasterizerState> GetSkyRastState() const { return skyRastState; }
	inline ComPtr<ID3D11DepthStencilState> GetSkyDepthState() const { return skyDepthState; }
	inline ComPtr< ID3D11SamplerState> GetSkySamplerState() const { return skySamplerOpt; }
	inline ComPtr< ID3D11ShaderResourceView> GetRoughness() const { return roughness; }
	inline ComPtr<ID3D11ShaderResourceView> GetMetalness() const { return metalness; }
	inline DirectX::XMFLOAT3 GetSpecularColor() const { return specColor; }
	void PrepareMaterial();


protected:

	// shader pointers
	shared_ptr<SimpleVertexShader> vertexShader;
	shared_ptr<SimplePixelShader> pixelShader;
	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11ShaderResourceView> normalMap;
	ComPtr<ID3D11SamplerState> samplerOptions;
	ComPtr<ID3D11ShaderResourceView> roughness;
	ComPtr<ID3D11ShaderResourceView> metalness;
	// Sky stuff
	ComPtr<ID3D11SamplerState> skySamplerOpt;
	ComPtr<ID3D11ShaderResourceView> skySRV;
	ComPtr<ID3D11RasterizerState> skyRastState;
	ComPtr<ID3D11DepthStencilState> skyDepthState;

	float shininess;
	DirectX::XMFLOAT3 specColor;

	ID3D11DeviceContext* context;
	ID3D11Device* device;
};

