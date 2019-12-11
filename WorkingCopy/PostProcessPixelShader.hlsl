
cbuffer Data : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	float staticValue;
	float totalTime;
}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D Pixels		: register(t0);
SamplerState Sampler	: register(s0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 totalColor = float4(0,0,0,0);
	uint numSamples = 0;

	float2 uv = input.uv;
	float4 colorSample = Pixels.Sample(Sampler, uv);
	float colorMod = frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453 * frac(totalTime));
	colorMod = -1 + (2 * colorMod);
	float color = colorMod * staticValue;
	colorSample.x += color;
	colorSample.y += color;
	colorSample.z += color;
	totalColor += colorSample;

	return totalColor;
}