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

struct VS_SHADOW_MAP_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 uvs[MAX_LIGHTS] : TEXCOORD2;
};

Texture2D gtxtTexture0 : register(t6);
Texture2D gtxtTexture1 : register(t7);
Texture2D gtxtTexture2 : register(t8);
Texture2D gtxtTexture3 : register(t9);
Texture2D gtxtTexture4 : register(t10);

SamplerState gssWrap : register(s0);


float4 PSShadowMapShadow(VS_SHADOW_MAP_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTexture0.Sample(gssWrap, input.uv0);
	float fAlpha = gtxtTexture1.Sample(gssWrap, input.uv0).w;

	float4 cDetailTexColors[3];
	cDetailTexColors[0] = gtxtTexture2.Sample(gssWrap, input.uv1 * 2.0f);
	cDetailTexColors[1] = gtxtTexture3.Sample(gssWrap, input.uv1 * 0.125f);
	cDetailTexColors[2] = gtxtTexture4.Sample(gssWrap, input.uv1 * 10.f);

	float4 cColor = cBaseTexColor * cDetailTexColors[0];
	cColor += lerp(cDetailTexColors[1] * 0.25f, cDetailTexColors[2], 1.0f - fAlpha);

	float3 normalW = normalize(input.normalW);

	float4 cIllumination = Lighting(input.positionW, normalize(input.normalW), true, input.uvs);

	return	(lerp(cColor, cIllumination, 0.2f));

	//return(cIllumination);

	
	//	float fAlpha = gtxtTerrainAlphaTexture.Sample(gSamplerState, input.uv0);
	

	
	/*
		cColor = lerp(cDetailTexColors[0], cDetailTexColors[2], 1.0f - fAlpha) ;
		cColor = lerp(cBaseTexColor, cColor, 0.3f) + cDetailTexColors[1] * (1.0f - fAlpha);
	*/
	/*
		if (fAlpha < 0.35f) cColor = cDetailTexColors[2];
		else if (fAlpha > 0.8975f) cColor = cDetailTexColors[0];
		else cColor = cDetailTexColors[1];
	*/

}
