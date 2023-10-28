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

[maxvertexcount(4)]
void GS(point VS_OUT input[1], uint primID : SV_PrimitiveID,  inout TriangleStream <GS_OUT> outStream)
{
	float3 vUp = float3(0.f, 1.f, 0.f);
	float3 vLook = gvCameraPosition.xyz - input[0].centerW;
	vLook = normalize(vLook);
	float3 vRight = cross(vUp, vLook);
	float fHalfW = input[0].sizeW.x * 0.5f;
	float fHalfH = input[0].sizeW.y * 0.5f;
	float4 pVertices[4];

	pVertices[0] = float4(input[0].centerW + fHalfW * vRight - fHalfH * vUp, 1.f);
	pVertices[1] = float4(input[0].centerW + fHalfW * vRight + fHalfH * vUp, 1.f);
	pVertices[2] = float4(input[0].centerW - fHalfW * vRight - fHalfH * vUp, 1.f);
	pVertices[3] = float4(input[0].centerW - fHalfW * vRight + fHalfH * vUp, 1.f);
	float2 pUVs[4] = { float2(0.f, 1.f), float2(0.f, 0.f), float2(1.f, 1.f), float2(1.f, 0.f) };
	GS_OUT output;
	for (int i = 0; i < 4; i++)
	{
		output.posW = pVertices[i].xyz;
		output.posH = mul(pVertices[i], mul(gmtxView, gmtxProjection));
		output.normalW = vLook;
		output.uv = mul(float3(pUVs[i], 1.0f), (float3x3)(gmtxGameObject)).xy;
		//output.uv = pUVs[i];
		output.primID = primID;
		outStream.Append(output);
	}
}