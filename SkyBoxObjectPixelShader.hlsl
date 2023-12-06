#include "Define.hlsl"

TextureCube gtxtSkyBoxTexture : register(t6);

SamplerState gssWrap : register(s0);

struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3 position : POSITION;
};

struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
};

float4 PSCubeMap(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{
	return gtxtSkyBoxTexture.Sample(gssWrap, input.positionL);
}