
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

	int UsePBR;
};


// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
	float4 posForShadow : SHADOWPOS;
};


// texture related variables
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
Texture2D roughnessMap		: register(t2);
Texture2D metalnessMap		: register(t3);
Texture2D shadowMap			: register(t4);
SamplerState basicSampler	: register(s0);
SamplerComparisonState shadowSampler  : register(s1);



float4 main(VertexToPixel input) : SV_TARGET
{
	// Fix for poor normals: re-normalizing interpolated normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Sample texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	// gamma correct
	surfaceColor = pow(surfaceColor, 2.2f);
	// Use normal mapping
	float3 mapColor = NormalMapping(normalMap, basicSampler, input.uv, input.normal, input.tangent);
	// Sample the roughness map
	float roughness = roughnessMap.Sample(basicSampler, input.uv).r;
	// Sample the metal map
	float metalness = metalnessMap.Sample(basicSampler, input.uv).r;

	// Specular color - Assuming albedo texture is actually holding specular color if metal == 1
	float3 specColor = surfaceColor.rgb;

	// shadows
	float2 shadowUV = input.posForShadow.xy / input.posForShadow.w * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;

	// Calculate this pixel's depth from the light
	float depthFromLight = input.posForShadow.z / input.posForShadow.w;

	// Sample the shadow map using a comparison sampler, which
	// will compare the depth from the light and the value in the shadow map
	float shadowAmount = shadowMap.SampleCmpLevelZero(shadowSampler, shadowUV, depthFromLight);

	// Total color for this pixel
	float3 totalColor = float3(0, 0, 0);
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
			if (UsePBR) {
				totalColor += SpotLightPBR(Lights[i], input.normal, input.worldPos, CameraPosition, roughness, metalness, surfaceColor.rgb, specColor) * shadowAmount;
			}
			else {
				totalColor += SpotLight(Lights[i], input.normal, input.worldPos, CameraPosition, 1.0f, roughness, surfaceColor.rgb);
			}
			break;
		}
	}
	// final gamma correct
	totalColor = pow(totalColor, 1.0f / 2.2f);
	return float4(totalColor, 1);
}

