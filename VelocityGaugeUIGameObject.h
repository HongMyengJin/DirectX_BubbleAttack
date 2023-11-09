#pragma once
#include "UIGameObject.h"
#include "BarUIGameObject.h"
class CVelocityGaugeUIGameObject : public CUIGameObject
{
public:
	CVelocityGaugeUIGameObject() {}
	~CVelocityGaugeUIGameObject() {}
public:
	virtual void Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, class CCamera* pCamera, XMFLOAT4X4* pxmf4x4World);

	void SetBarValue(float fTotalBarValue, float fCurrentBarValue);
protected:
	std::shared_ptr<CBarUIGameObject> m_pBarGaugeUI;
	std::shared_ptr<CUIGameObject> m_pUIGameObject;
};

