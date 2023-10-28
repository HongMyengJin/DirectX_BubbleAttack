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