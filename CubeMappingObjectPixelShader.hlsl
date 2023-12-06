#include "Light.hlsl"

struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

SamplerState gssWrap : register(s0);
TextureCube gtxtCubeMap : register(t17);

float4 PSCubeMapping(VS_STANDARD_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);

	float4 uvs[MAX_LIGHTS];

	float4 cIllumination = Lighting(input.positionW, input.normalW, false, uvs);

	float3 f3FromCamera = normalize(input.positionW - gvCameraPosition.xyz);
	float3 f3Reflected = normalize(reflect(f3FromCamera, input.normalW));
	float4 cCubeTextureColor = gtxtCubeMap.Sample(gssWrap, f3Reflected);

	return(cCubeTextureColor);

}
