#pragma once
#include "GameObject.h"
class CParticleObject : public CGameObject
{
public:
	CParticleObject() {};
	~CParticleObject() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeapm, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, float fLifetime, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size, UINT nMaxParticles);
	virtual void OnPostRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);
};

