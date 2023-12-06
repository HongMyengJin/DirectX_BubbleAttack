#include "Light.hlsl"

Texture2D gtxtTextureAlbedo : register(t6);
Texture2D gtxtTextureSpecular : register(t7);
Texture2D gtxtTextureNormal : register(t8);
Texture2D gtxtTextureMetallic : register(t9);
Texture2D gtxtTextureEmission : register(t10);

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
	float4 cTexture = gtxtTextureAlbedo.Sample(gssWrap, uvw.xy);
	float4 cColor = cIllumination * cTexture;
	cColor.a = cTexture.a;
	return cTexture;
}
