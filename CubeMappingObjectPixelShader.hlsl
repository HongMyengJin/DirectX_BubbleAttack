
struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	matrix		gmtxInverseView : packoffset(c8);
	float3		gvCameraPosition : packoffset(c12);
};


cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	MATERIAL	gMaterial : packoffset(c4);
	uint		gnTexturesMask : packoffset(c8);
};

#include "Light.hlsl"

struct VS_LIGHTING_INPUT
{
	float3	position    : POSITION;
	float3	normal		: NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
	float4	position    : SV_POSITION;
	float3	positionW   : POSITION;
	float3	normalW		: NORMAL;
};

SamplerState gssWrap : register(s0);
TextureCube gtxtCubeMap : register(t6);

float4 PSCubeMapping(VS_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);

	float4 uvs[MAX_LIGHTS];

	float4 cIllumination = Lighting(input.positionW, input.normalW, false, uvs);

	float3 f3FromCamera = normalize(input.positionW - gvCameraPosition.xyz);
	float3 f3Reflected = normalize(reflect(f3FromCamera, input.normalW));
	float4 cCubeTextureColor = gtxtCubeMap.Sample(gssWrap, f3Reflected);

	//	return(float4(vReflected * 0.5f + 0.5f, 1.0f));
		return(cCubeTextureColor);
		//	return(cIllumination * cCubeTextureColor);
}
