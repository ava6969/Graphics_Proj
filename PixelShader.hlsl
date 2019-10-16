
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
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
	float Intensity;
};

struct PointLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Position;
	float Radius;
	float Intensity;
};

struct SpotLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Position;
	float Angle;
	float3 Direction;
	float MaxLength;
	float Intensity;
};

cbuffer lightData : register(b1)
{
	SpotLight light;
	PointLight light2;
	float3 cameraPos;
	float shininess;
};

Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
SamplerState basicSampler	: register(s0);

float4 calculateDLight(DirectionalLight l, float3 norm, float shine, float3 toCam, float4 tex)
{
	// get the direction to the light
	float3 lightNormal = normalize(-l.Direction);
	// calculate the brightness based on normals
	float brightness = saturate(dot(norm, lightNormal));

	// calculate specular
	float3 H = normalize(-l.Direction + toCam);
	float NdotH = dot(norm, H);
	float specDir = saturate(pow(NdotH, shine));
	// return the combined color
	return l.AmbientColor * tex + l.DiffuseColor * tex * brightness + specDir * l.Intensity;
}

float4 calculatePLight(PointLight l, float3 norm, float3 pos, float shine, float3 toCam, float4 tex)
{
	// calculate the brightness
	float3 toLight = normalize(l.Position - pos);
	float NdotL = saturate(dot(norm, toLight));

	// calculate specular
	float3 H = normalize(toLight + toCam);
	float NdotH = dot(norm, H);
	float spec = saturate(pow(NdotH, shininess));

	// calculate attenuation
	float dist = distance(l.Position, pos);
	float range = l.Radius;
	float att = saturate(1.0f - (dist * dist / (range * range)));

	// return the final color
	return ((l.AmbientColor * tex + l.DiffuseColor * tex) * NdotL + spec) * att * l.Intensity;
}

float4 calculateSLight(SpotLight l, float3 norm, float3 pos, float3 shine, float3 toCam, float4 tex)
{
	// first do point light calculation
	float3 toLight = normalize(l.Position - pos);
	float NdotL = saturate(dot(norm, toLight));

	// calculate the spot light falloff
	float angleFromCenter = max(dot(-toLight, l.Direction), 0.0f);
	float spotAmount = pow(angleFromCenter, l.Angle);

	// calculate specular
	float3 H = normalize(toLight + toCam);
	float NdotH = dot(norm, H);
	float spec = saturate(pow(NdotH, shininess));

	// calculate attenuation
	float dist = distance(l.Position, pos);
	float range = l.MaxLength;
	float att = saturate(1.0f - (dist * dist / (range * range)));

	// return the final color
	return ((l.AmbientColor * tex + l.DiffuseColor * tex) * NdotL + spec) * att * spotAmount * l.Intensity;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// sample the texture
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float4 mapColor = normalMap.Sample(basicSampler, input.uv);

	// normalize normals
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// calculate normal mapping
	float3 normalFromMap = mapColor.rgb * 2 - 1;

	float3 T = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
	float3 B = cross(T, input.normal);

	float3x3 TBN = float3x3(T, B, input.normal);
	
	// rotate the normal
	input.normal = normalize(mul(normalFromMap, TBN));

	// set up values to use for lighting
	float3 toCamera = normalize(cameraPos - input.worldPos);

	float4 light1Color = calculateSLight(light, input.normal, input.worldPos, shininess, toCamera, surfaceColor);
	//float4 light2Color = calculatePLight(light2, input.normal, input.worldPos, shininess, toCamera, surfaceColor);
	return light1Color; //+ light2Color;
}

