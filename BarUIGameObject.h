#pragma once
#include "UIGameObject.h"
class CBarUIGameObject : public CUIGameObject
{
public:
	CBarUIGameObject() {}
	~CBarUIGameObject() {}

public:
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize, wchar_t* pstrFileName, wchar_t* pstrFileName2);
	virtual void Animate(float fTimeElapsed);
	void UpdateBarValue(float fTotalBarValue, float fCurrentBarValue);

protected:
	float	m_fTotalBarValue = 0.f;
	float	m_fCurrentBarValue = 0.f;
};

