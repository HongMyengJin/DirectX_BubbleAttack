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

struct VS_IN
{
	float3 posW : POSITION;
	float2 sizeW : SIZE;
};

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