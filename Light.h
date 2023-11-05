#pragma once
#include "GameObject.h"

struct LightData
{
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff;
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int						m_nType;
	float					m_fRange;
	float					padding;
};

struct LightDatas
{
	LightData											m_pLightsDatas[MAX_LIGHT];
	XMFLOAT4											m_xmf4GlobalAmbient;
public:
	void Init();
};

class CLight : public CGameObject
{
public:
	CLight() {};
	~CLight() {};

	virtual void Init();
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);

	void SetPosition(UINT Iindex, XMFLOAT3 Position);
	void SetOffsetPosition(UINT Iindex, XMFLOAT3 Offset);
	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void Release();
private:
	std::shared_ptr<LightDatas>							m_pLightData;
	std::shared_ptr<LightDatas>							m_pcbMappedLights;

	Microsoft::WRL::ComPtr<ID3D12Resource>				m_pd3dcbLights;

	std::vector<XMFLOAT3>								m_vOffsetPositions;
;	std::vector<XMFLOAT3>								m_vPositions;


};

