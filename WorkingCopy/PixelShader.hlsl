
#include "Lighting.hlsli"

// How many lights could we handle?
#define MAX_LIGHTS 128

// Data that can change per material
cbuffer perMaterial : register(b0)
{
	// Surface color
	float4 Color;
};

// Data that only changes once per frame
cbuffer perFrame : register(b1)
{
	// An array of light data
	Light Lights[MAX_LIGHTS];

	// The amount of lights THIS FRAME
	int LightCount;

	// Needed for specular (reflection) calculation
	float3 CameraPosition;

};


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION; // The world position of this PIXEL
	float3 tangent		: TANGENT;
};


// texture related variables
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
Texture2D roughnessMap		: register(t2);
Texture2D metalnessMap		: register(t3);
SamplerState basicSampler	: register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Fix for poor normals: re-normalizing interpolated normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Sample texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	// Use normal mapping
	float3 mapColor = NormalMapping(normalMap, basicSampler, input.uv, input.normal, input.tangent);
	// Sample the roughness map
	float roughness = roughnessMap.Sample(basicSampler, input.uv).r;
	// Sample the metal map
	float metalness = metalnessMap.Sample(basicSampler, input.uv).r;

	
	// Actually using texture?
	//surfaceColor.rgb = lerp(Color.rgb, surfaceColor.rgb, UseTexture);

	// Specular color - Assuming albedo texture is actually holding specular color if metal == 1
	//float3 specColor = lerp(F0_NON_METAL.rrr, surfaceColor.rgb, metal);

	float3 specColor = surfaceColor.rgb;

	// Total color for this pixel
	float3 totalColor = float3(0,0,0);

	// Loop through all lights this frame
	for (int i = 0; i < LightCount; i++)
	{
		// Which kind of light?
		switch (Lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			totalColor += DirLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metalness, surfaceColor.rgb, specColor);
			break;

		case LIGHT_TYPE_POINT:
			totalColor += PointLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metalness, surfaceColor.rgb, specColor);
			break;

		case LIGHT_TYPE_SPOT:
			totalColor += SpotLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metalness, surfaceColor.rgb, specColor);
			break;
		}
	}

	// Adjust the light color by the light amount
	float3 gammaCorrect = pow(totalColor, 1.0 / 2.2);
	return float4(gammaCorrect, 1);
}