
// How many lights could we handle?
#define MAX_LIGHTS 128

#define LIGHT_TYPE_DIRECTIONAL	0
#define LIGHT_TYPE_POINT		1
#define LIGHT_TYPE_SPOT			2

struct Light
{
	int		Type;
	float3	Direction;	// 16 bytes

	float	Range;
	float3	Position;	// 32 bytes

	float	Intensity;
	float3	Color;		// 48 bytes

	float	SpotFalloff;
	float3	Padding;	// 64 bytes
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
};



cbuffer lightData : register(b1)
{
	// An array of light data
	Light Lights[MAX_LIGHTS];

	// The amount of lights THIS FRAME
	int LightCount;

	float3 cameraPos;
	float shininess;
	float3 specularColor;
};

// texture related variables
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
Texture2D roughnessMap		: register(t2);
Texture2D metalnessMap		: register(t3);
SamplerState basicSampler	: register(s0);



/*
float4 calculateDLight(Light l, float3 norm, float shine, float3 toCam, float4 tex)
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
	return l.Color * tex + l.Color * tex * brightness + specDir * l.Intensity;
}

float4 calculatePLight(Light l, float3 norm, float3 pos, float shine, float3 toCam, float4 tex)
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

float4 calculateSLight(Light l, float3 norm, float3 pos, float3 shine, float3 toCam, float4 tex)
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


*/



float SpecDistribution(float3 n, float3 h, float roughness)
{
	// pre-calculations
	float NdotH = saturate(dot(n, h));
	float NdotH2 = NdotH * NdotH;
	float a = roughness * roughness;
	// remap
	float a2 = max(a * a, 0.000001f);

	float denomToSquare = NdotH2 * (a2 - 1.0f) + 1.0f;

	// final value
	return a2 / (3.14159265f * denomToSquare * denomToSquare);
}

float3 Fresnel(float3 v, float3 h, float3 f0)
{
	// pre-calculations
	float VdotH = saturate(dot(v, h));

	// final value - Shlick
	return f0 + (1.0f - f0) * pow(1.0f - VdotH, 5);
}

float GeometricShadowing(float3 n, float3 v, float3 h, float roughness)
{
	// end result of remapping
	float k = pow(roughness + 1.0f, 2) / 8.0f;
	float NdotV = saturate(dot(n, v));

	// final value
	return NdotV / (NdotV * (1.0f - k) + k);
}

// n = surface normal
// l = toLight
// v = toCamera
// h = half angle (v + l / 2)
float3 microfacetBRDF(float3 n, float3 l, float3 v, float3 h, float roughness, float3 specColor)
{
	float D = SpecDistribution(n, h, roughness);
	float3 F = Fresnel(v, h, specColor);
	float G = GeometricShadowing(n, v, h, roughness) * GeometricShadowing(n, l, h, roughness);

	return (D * F * G) / (4.0f * max(dot(n, v), dot(n,l)));
}

float diffuseBRDF(float3 n, float3 l)
{
	return saturate(dot(n, l));
}

float diffuseBRDFSpotlight(float3 n, float3 l, Light light, float3 pos)
{
	// first do point light calculation
	float NdotL = saturate(dot(n, l));

	// calculate the spot light falloff
	float angleFromCenter = max(dot(-l, light.Direction), 0.0f);
	float spotAmount = pow(angleFromCenter, light.SpotFalloff);

	// calculate attenuation
	float dist = distance(light.Position, pos);
	float range = light.Range;
	float att = saturate(1.0f - (dist * dist / (range * range)));

	return saturate(spotAmount * att);
}

float3 diffuseEnergyConserve(float diffuse, float3 specular, float metalness)
{
	return diffuse * ((1.0f - saturate(specular)) * (1.0f - metalness));
}

float4 calculatePBR(float3 n, float3 l, float3 v, float3 h, float roughness, float metalness, float3 specColor, float3 tex, float3 lightA, float3 lightD)
{
	float diffuse = diffuseBRDF(n, l);
	float3 specular = microfacetBRDF(n, l, v, h, roughness, specColor);
	float3 diffuseAdjusted = diffuseEnergyConserve(diffuse, specular, metalness);
	float3 result = (lightA * tex) + diffuseAdjusted * tex * lightD + specular;
	// gamma correct
	result = pow(result, 1.0f / 2.2f);
	return float4(result, 1);
}

float4 calculatePBRSpotlight(float3 n, float3 l, float3 v, float3 h, float roughness, float metalness, float3 specColor, float3 tex, Light light, float3 pos)
{
	// run normal pbr
	float diffuse = diffuseBRDF(n, l);
	float3 specular = microfacetBRDF(n, l, v, h, roughness, specColor);
	float3 diffuseAdjusted = diffuseEnergyConserve(diffuse, specular, metalness);
	// spot light calculation
	// calculate the spot light falloff
	float angleFromCenter = max(dot(-l, light.Direction), 0.0f);
	float spotAmount = pow(angleFromCenter, light.SpotFalloff);

	// calculate attenuation
	float dist = distance(light.Position, pos);
	float range = light.Range;
	float att = saturate(1.0f - (dist * dist / (range * range)));

	// calculate final result
	float3 result = ((light.Color * tex) + diffuseAdjusted * tex * light.Color + specular) * att * spotAmount * light.Intensity;
	// gamma correct
	result = pow(result, 1.0f / 2.2f);
	return float4(result, 1);
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
	float4 roughness = roughnessMap.Sample(basicSampler, input.uv);
	float4 metalness = metalnessMap.Sample(basicSampler, input.uv);

	// gamma correct texture
	surfaceColor = float4(pow(surfaceColor.rgb, 2.2f), 1);

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

	//// set up values to use for lighting
	//float3 v = normalize(cameraPos - input.worldPos);	// to camera
	////float3 l = normalize(-light2.Direction);			// to light
	//float3 l = normalize(light.Position - input.worldPos);
	//float3 h = normalize((v + l) / 2.0f);				// half vector
	//float3 h = (v + l) / 2;
	//float4 light1Color = calculatePBRSpotlight(input.normal, l, v, h, roughness.r, metalness.r, specularColor, surfaceColor.rgb, light, input.worldPos);
	//float4 light2Color = calculatePBR(input.normal, l, v, h, roughness.r, metalness.r, specularColor, surfaceColor.rgb, light2.AmbientColor.rgb, light2.DiffuseColor.rgb);
	//return light1Color; //+*/ light2Color;

		// Total color for this pixel
	float3 totalColor = float3(0, 0, 0);
	// Loop through all lights this frame
	for (int i = 0; i < LightCount; i++)
	{
		// set up values to use for lighting
		float3 v = normalize(cameraPos - input.worldPos);	// to camera
		//float3 l = normalize(-light2.Direction);			// to light
		float3 l = normalize(Lights[i].Position - input.worldPos);
		float3 h = normalize((v + l) / 2.0f);				// half vector

		// Which kind of light?
		switch (Lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			//totalColor += DirLight(Lights[i], input.normal, input.worldPos, CameraPosition, Shininess, roughness, surfaceColor.rgb);
			break;

		case LIGHT_TYPE_POINT:
			//totalColor += PointLight(Lights[i], input.normal, input.worldPos, CameraPosition, Shininess, roughness, surfaceColor.rgb);
			break;

		case LIGHT_TYPE_SPOT:
			totalColor += calculatePBRSpotlight(input.normal, l, v, h, roughness.r, metalness.r, specularColor, surfaceColor.rgb, Lights[i], input.worldPos);
			break;
		}
	}

	//  pow(totalColor, 1.0f / 2.2f)
	// Adjust the light color by the light amount
	return float4(totalColor, 1);
}

