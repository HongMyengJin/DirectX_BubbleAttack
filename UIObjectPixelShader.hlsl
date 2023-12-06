#include "Define.hlsl"

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
Texture2D gtxtTexture0 : register(t6);
Texture2D gtxtTexture1 : register(t7);
Texture2D gtxtTexture2 : register(t8);
Texture2D gtxtTexture3 : register(t9);
Texture2D gtxtTexture4 : register(t10);

float4 PSUIObject(VS_UI_OUTPUT input) : SV_TARGET
{
	float2 fU = gmtxGameObject._21_22;// UV°ª Á¶Àý 
	float2 fV = gmtxGameObject._23_24;

	input.uv.x = fU.x + (input.uv.x * (fU.y - fU.x));
	input.uv.y = fV.x + (input.uv.y * (fV.y - fV.x));

    float4 cColor = gtxtTexture0.Sample(gssWrap, input.uv);
	float4 cColor2 = gtxtTexture1.Sample(gssWrap, input.uv);
	if (!(cColor2.a >= gmtxGameObject._13) || !(cColor2.a <= gmtxGameObject._14))
		discard;

	return cColor;
}
