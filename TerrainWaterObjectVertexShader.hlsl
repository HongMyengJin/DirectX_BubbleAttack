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

cbuffer cbFrameworkInfo : register(b12)
{
	float		gfCurrentTime : packoffset(c0.x);
	float		gfElapsedTime : packoffset(c0.y);
	float		gfSecondsPerFirework : packoffset(c0.z);
	int			gnFlareParticlesToEmit : packoffset(c0.w);;
	float3		gf3Gravity : packoffset(c1.x);
	int			gnMaxFlareType2Particles : packoffset(c1.w);;
};


//struct VS_WATER_INPUT
//{
//	float3 position : POSITION;
//	float2 uv : TEXCOORD0;
//};
//
//struct VS_WATER_OUTPUT
//{
//	float4 position : SV_POSITION;
//	float2 uv : TEXCOORD0;
//};

//VS_WATER_OUTPUT VSTerrainWater(VS_WATER_INPUT input)
//{
//	VS_WATER_OUTPUT output;
//
//	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
//	output.uv = input.uv;
//
//	return(output);
//}

struct VS_RIPPLE_WATER_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_RIPPLE_WATER_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

VS_RIPPLE_WATER_OUTPUT VSRippleWater(VS_RIPPLE_WATER_INPUT input)
{
	VS_RIPPLE_WATER_OUTPUT output;

	//	input.position.y += sin(gfCurrentTime * 0.5f + input.position.x * 0.01f + input.position.z * 0.01f) * 35.0f;
	//	input.position.y += sin(input.position.x * 0.01f) * 45.0f + cos(input.position.z * 0.01f) * 35.0f;
	//	input.position.y += sin(gfCurrentTime * 0.5f + input.position.x * 0.01f) * 45.0f + cos(gfCurrentTime * 1.0f + input.position.z * 0.01f) * 35.0f;
	//	input.position.y += sin(gfCurrentTime * 0.5f + ((input.position.x * input.position.x) + (input.position.z * input.position.z)) * 0.01f) * 35.0f;
	//	input.position.y += sin(gfCurrentTime * 1.0f + (((input.position.x * input.position.x) + (input.position.z * input.position.z)) - (1000 * 1000) * 2) * 0.0001f) * 10.0f;

	//	input.position.y += sin(gfCurrentTime * 1.0f + (((input.position.x * input.position.x) + (input.position.z * input.position.z))) * 0.0001f) * 10.0f;
	input.position.y += sin(gfCurrentTime * 0.35f + input.position.x * 0.35f) * 2.95f + cos(gfCurrentTime * 0.30f + input.position.z * 0.35f) * 2.05f;
	output.position = mul(float4(input.position, 1.0f), gmtxGameObject);
	if (155.0f < output.position.y) output.position.y = 155.0f;
	output.position = mul(mul(output.position, gmtxView), gmtxProjection);

	//	output.color = input.color;
	output.color = (input.position.y / 200.0f) + 0.55f;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return(output);
}