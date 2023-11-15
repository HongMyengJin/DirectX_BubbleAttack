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
	float4 cIllumination = Lighting(input.posW, input.normalW);
	float3 uvw = float3(input.uv, input.primID % 4); //
	float4 cTexture = gtxtTextureAlbedo.Sample(gssWrap, uvw.xy);
	float4 cColor = cIllumination * cTexture;
	cColor.a = cTexture.a;
	return cTexture;
}
