#include "Light.hlsl"

Texture2D gtxtTextureMap : register(t17);

SamplerState gssWrap : register(s0);

struct GS_OUT
{
	float4 posH :SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
	uint primID : SV_PrimitiveID;
};

float4 PS(GS_OUT input) : SV_Target
{
	float4 uvs[MAX_LIGHTS];
	float4 cIllumination = Lighting(input.posW, input.normalW, false, uvs);
	float3 uvw = float3(input.uv, input.primID % 4); //

	float3 f3FromCamera = normalize(input.posW - gvCameraPosition.xyz);
	float3 f3Reflected = normalize(reflect(f3FromCamera, input.normalW));
	float4 cTexture = gtxtTextureMap.Sample(gssWrap, f3Reflected);
	return cTexture;
}
