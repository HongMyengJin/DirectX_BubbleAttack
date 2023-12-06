#include "Define.hlsl"

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

SamplerState gssWrap : register(s0);
Texture2D gtxtTextureMap : register(t17);

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTextureMap.Sample(gssWrap, input.uv);

	return(cColor);
}