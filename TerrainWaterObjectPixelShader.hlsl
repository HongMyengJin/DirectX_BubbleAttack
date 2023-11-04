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

cbuffer cbWaterInfo : register(b5)
{
	matrix		gf4x4TextureAnimation : packoffset(c0);
};


Texture2D gtxtTexture[7] : register(t6);

static matrix<float, 3, 3> sf3x3TextureAnimation = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

SamplerState gssWrap : register(s0);

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
//
//#define _WITH_FULL_TEXTURES
//
//float4 PSTerrainWater(VS_WATER_OUTPUT input) : SV_TARGET
//{
//	float4 cBaseTexColor = gtxtTexture[0].Sample(gssWrap, input.uv);
//	float4 cDetail0TexColor = gtxtTexture[1].Sample(gssWrap, input.uv * 20.0f);
//	float4 cDetail1TexColor = gtxtTexture[2].Sample(gssWrap, input.uv * 20.0f);
//
//	float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
//#ifdef _WITH_BASE_TEXTURE_ONLY
//	cColor = cBaseTexColor;
//#else
//#ifdef _WITH_FULL_TEXTURES
//	cColor = lerp(cBaseTexColor * cDetail0TexColor, cDetail1TexColor.r * 0.5f, 0.35f);
//#else
//	cColor = cBaseTexColor * cDetail0TexColor;
//#endif
//#endif
//
//	return(cColor);
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

float4 PSRippleWater(VS_RIPPLE_WATER_OUTPUT input) : SV_TARGET
{
	float2 uv = input.uv0;

	uv = mul(float3(input.uv0, 1.0f), (float3x3)gf4x4TextureAnimation).xy;
	//	uv = mul(float4(uv, 1.0f, 0.0f), gf4x4TextureAnimation).xy;
	uv.y += gfCurrentTime * 0.1f;


	float4 cBaseTexColor = gtxtTexture[0].SampleLevel(gssWrap, uv, 0);
	float4 cDetail0TexColor = gtxtTexture[1].SampleLevel(gssWrap, uv, 0);
	float4 cDetail1TexColor = gtxtTexture[2].SampleLevel(gssWrap, uv, 0);

	float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	cColor = lerp(cBaseTexColor * cDetail0TexColor, cDetail1TexColor.r * 0.5f, 0.35f);

	return(cColor);
}

