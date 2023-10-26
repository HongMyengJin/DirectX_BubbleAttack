#pragma once
#include "GameObject.h"

class CTextureRectObject : public CGameObject
{
public:
	CTextureRectObject() {};
	~CTextureRectObject() {};

	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize);
	virtual void Animate(float fTimeElapsed);
	void AnimateUV(float fTime);

	void SetAnimateLifeTime(float fLifeTime);
protected:
	float m_fAnimateTime = 0.f;
	float m_fAnimateLifeTime = 5.f;
};

