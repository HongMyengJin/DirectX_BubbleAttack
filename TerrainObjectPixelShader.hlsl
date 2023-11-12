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

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

#include "Light.hlsl"
Texture2D gtxtTexture[7] : register(t6); //Base, Alpha, Detail0, Detail1, Detail2

SamplerState gssWrap : register(s0);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normal : NORMAL;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 normalW : NORMAL;
};

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTexture[0].Sample(gssWrap, input.uv0);
	//	float fAlpha = gtxtTerrainAlphaTexture.Sample(gSamplerState, input.uv0);
	float fAlpha = gtxtTexture[1].Sample(gssWrap, input.uv0).w;

	float4 cDetailTexColors[3];
	cDetailTexColors[0] = gtxtTexture[2].Sample(gssWrap, input.uv1 * 2.0f);
	cDetailTexColors[1] = gtxtTexture[3].Sample(gssWrap, input.uv1 * 0.125f);
	cDetailTexColors[2] = gtxtTexture[4].Sample(gssWrap, input.uv1 * 10.f);

	float4 cColor = cBaseTexColor * cDetailTexColors[0];
	cColor += lerp(cDetailTexColors[1] * 0.25f, cDetailTexColors[2], 1.0f - fAlpha);
	/*
		cColor = lerp(cDetailTexColors[0], cDetailTexColors[2], 1.0f - fAlpha) ;
		cColor = lerp(cBaseTexColor, cColor, 0.3f) + cDetailTexColors[1] * (1.0f - fAlpha);
	*/
	/*
		if (fAlpha < 0.35f) cColor = cDetailTexColors[2];
		else if (fAlpha > 0.8975f) cColor = cDetailTexColors[0];
		else cColor = cDetailTexColors[1];
	*/
	float3 normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, normalW);
	return	(lerp(cColor, cIllumination, 0.2f));
}