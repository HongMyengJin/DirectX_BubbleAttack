#pragma once
#include "GameObject.h"

class CRippleWaterObject : public CGameObject
{
public:
	CRippleWaterObject() {};
	~CRippleWaterObject() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual void Animate(float fTimeElapsed);
	virtual void Update(float fTimeElapsed, void* pData, void* pData2);

	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	XMFLOAT3 GetScale() { return m_xmf3Scale; }
	float GetWidth() { return (m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return (m_nLength * m_xmf3Scale.z); }

	void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
private:
	UINT		m_nWidth;
	UINT		m_nLength;

	XMFLOAT3	m_xmf3Scale;

	XMFLOAT4X4	m_xmf4x4WaterAnimation;

};

