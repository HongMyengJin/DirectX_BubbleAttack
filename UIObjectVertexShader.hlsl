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

VS_UI_OUTPUT VSUIObject(uint nVertexID : SV_VertexID)
{
    VS_UI_OUTPUT output;
    float2 fSize = gmtxGameObject._11_12; // 0 ~ 1
    float2 fScreenPos = gmtxGameObject._41_42 * 2.f - 1; // 0 ~ 1 -> -1 ~ 1 ��ȯ

    if (nVertexID == 0) { output.position = float4(-fSize.x, +fSize.y, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    if (nVertexID == 1) { output.position = float4(+fSize.x, +fSize.y, 0.0f, 1.0f); output.uv = float2(1.0f, 0.0f); }
    if (nVertexID == 2) { output.position = float4(+fSize.x, -fSize.y, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    if (nVertexID == 3) { output.position = float4(-fSize.x, +fSize.y, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    if (nVertexID == 4) { output.position = float4(+fSize.x, -fSize.y, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    if (nVertexID == 5) { output.position = float4(-fSize.x, -fSize.y, 0.0f, 1.0f); output.uv = float2(0.0f, 1.0f); }
    // 2
    output.position += float4(fScreenPos, 0.f, 0.f);
    return(output);
}