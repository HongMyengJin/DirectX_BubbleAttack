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

struct VS_UI_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_UI_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

SamplerState gssWrap : register(s0);
Texture2D gtxtTexture[7] : register(t6);

float4 PSUIObject(VS_UI_OUTPUT input) : SV_TARGET
{
	float2 fU = gmtxGameObject._12_13;// UV°ª Á¶Àý 
	float2 fV = float2(0.f, 1.f);
	input.uv.x = fU.x + (input.uv.x * (fU.y - fU.x));
	input.uv.y = fV.x + (input.uv.y * (fV.y - fV.x));

    float4 cColor = gtxtTexture[0].Sample(gssWrap, input.uv);

	return cColor;
}
