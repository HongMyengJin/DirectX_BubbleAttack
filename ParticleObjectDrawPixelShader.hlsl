#include "Define.hlsl"

struct GS_PARTICLE_DRAW_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXTURE;
	uint type : PARTICLETYPE;
};

Texture2D gtxtTexture[7] : register(t6);
SamplerState gssWrap : register(s0);

float4 PSParticleDraw(GS_PARTICLE_DRAW_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture[0].Sample(gssWrap, input.uv);
	cColor *= input.color;

	return(cColor);
}
