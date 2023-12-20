#include "Light.hlsl"

struct DS_TERRAIN_TESSELLATION_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float4 tessellation : TEXCOORD2;
	float3 normalW : NORMAL;
};

Texture2D gtxtTexture0 : register(t6);
Texture2D gtxtTexture1 : register(t7);
Texture2D gtxtTexture2 : register(t8);
Texture2D gtxtTexture3 : register(t9);
Texture2D gtxtTexture4 : register(t10);

SamplerState gssWrap : register(s0);

float4 PSTerrainTessellation(DS_TERRAIN_TESSELLATION_OUTPUT input) : SV_TARGET
{
	float4 cColor = float4(0.f, 0.f, 0.f, 1.f);

	if (input.tessellation.w <= 5.0f) cColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	else if (input.tessellation.w <= 10.0f) cColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (input.tessellation.w <= 20.0f) cColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
	else if (input.tessellation.w <= 30.0f) cColor = float4(1.0f, 0.0f, 1.0f, 1.0f);
	else if (input.tessellation.w <= 40.0f) cColor = float4(1.0f, 1.0f, 0.0f, 1.0f);
	else if (input.tessellation.w <= 50.0f) cColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	else if (input.tessellation.w <= 55.0f) cColor = float4(0.2f, 0.2f, 0.72f, 1.0f);
	else if (input.tessellation.w <= 60.0f) cColor = float4(0.5f, 0.75f, 0.75f, 1.0f);
	else cColor = float4(0.87f, 0.17f, 1.0f, 1.0f);
	
	return cColor;


}

/*

		float4 cBaseTexColor = gtxtTexture0.Sample(gssWrap, input.uv0);
		//	float fAlpha = gtxtTerrainAlphaTexture.Sample(gSamplerState, input.uv0);
		float fAlpha = gtxtTexture1.Sample(gssWrap, input.uv0).w;

		float4 cDetailTexColors[3];
		cDetailTexColors[0] = gtxtTexture2.Sample(gssWrap, input.uv1 * 2.0f);
		cDetailTexColors[1] = gtxtTexture3.Sample(gssWrap, input.uv1 * 0.125f);
		cDetailTexColors[2] = gtxtTexture4.Sample(gssWrap, input.uv1 * 10.f);

		float4 cColor = cBaseTexColor * cDetailTexColors[0];
		cColor += lerp(cDetailTexColors[1] * 0.25f, cDetailTexColors[2], 1.0f - fAlpha);

		float4 uvs[MAX_LIGHTS];
		float3 normalW = normalize(input.normalW);
		float4 cIllumination = Lighting(input.position, normalW, false, uvs);

		return	(lerp(cColor, cIllumination, 0.2f));
	//}
	*/