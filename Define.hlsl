//--------------------------------------------------------------------------------------
#define MAX_LIGHTS			2
#define MAX_MATERIALS		512 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
//#define _WITH_REFLECT

#define MAX_DEPTH_TEXTURES		MAX_LIGHTS

#define FRAME_BUFFER_WIDTH		1920
#define FRAME_BUFFER_HEIGHT		1080

#define _DEPTH_BUFFER_WIDTH        (FRAME_BUFFER_WIDTH * 4)
#define _DEPTH_BUFFER_HEIGHT		(FRAME_BUFFER_HEIGHT * 4)

#define DELTA_X                    (1.0f / _DEPTH_BUFFER_WIDTH)
#define DELTA_Y                    (1.0f / _DEPTH_BUFFER_HEIGHT)

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



cbuffer cbWaterInfo : register(b5)
{
	matrix		gf4x4TextureAnimation : packoffset(c0);
};


cbuffer cbFrameworkInfo : register(b12)
{
	float		gfCurrentTime : packoffset(c0.x);
	float		gfElapsedTime : packoffset(c0.y);
	float		gfSecondsPerFirework : packoffset(c0.z);
	int			gnFlareParticlesToEmit : packoffset(c0.w);;
	float3		gf3Gravity : packoffset(c1.x);
	int			gnMaxFlareType2Particles : packoffset(c1.w);;
	bool		gbTessellationWire : packoffset(c2.x);
};


struct CB_TOOBJECTSPACE
{
	matrix		mtxToTexture;
	float4		f4Position;
};

cbuffer cbToLightSpace : register(b3)
{
	CB_TOOBJECTSPACE gcbToLightSpaces[MAX_LIGHTS];
};


struct LIGHT
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular;
	float3					m_vPosition;
	float 					m_fFalloff;
	float3					m_vDirection;
	float 					m_fTheta; //cos(m_fTheta)
	float3					m_vAttenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int 					m_nType;
	float					m_fRange;
	float					padding;
};


cbuffer cbLights : register(b4)
{
	LIGHT					gLights[MAX_LIGHTS];
	float4					gcGlobalAmbientLight;
};
Texture2D<float> gtxtDepthTextures[MAX_DEPTH_TEXTURES] : register(t17);
SamplerComparisonState gssComparisonPCFShadow : register(s2);

