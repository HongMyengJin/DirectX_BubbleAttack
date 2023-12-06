
struct VS_IN
{
	float3 posW : POSITION;
	float2 sizeW : SIZE;
};
#include "Define.hlsl"

struct VS_OUT
{
	float3 centerW : POSITION;
	float2 sizeW : SIZE;
};

struct GS_OUT
{
	float4 posH :SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
	uint primID : SV_PrimitiveID;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	output.centerW = gmtxGameObject._41_42_43;
	output.sizeW = input.sizeW;
	return output;
}