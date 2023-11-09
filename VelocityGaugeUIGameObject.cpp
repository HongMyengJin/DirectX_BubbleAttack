#include "VelocityGaugeUIGameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"

void CVelocityGaugeUIGameObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap)
{
	m_pComponents.resize(4);

	m_pUIGameObject = std::make_shared<CUIGameObject>();
	m_pUIGameObject->Init(pd3dDevice, pd3dCommandList, pDescriptorHeap, XMFLOAT2(10.f, 10.f), L"Image/SlideBar.dds");
	m_pUIGameObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.9f, FRAME_BUFFER_HEIGHT * 0.5f));
	m_pUIGameObject->SetSize(XMFLOAT2(70.f, 700.f));

	m_pBarGaugeUI = std::make_shared<CBarUIGameObject>();
	m_pBarGaugeUI->Init(pd3dDevice, pd3dCommandList, pDescriptorHeap, XMFLOAT2(10.f, 10.f), L"Image/ValueBar.dds");
	m_pBarGaugeUI->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.9f, FRAME_BUFFER_HEIGHT * 0.53f));
	m_pBarGaugeUI->SetSize(XMFLOAT2(70.f, 700.f));
}

void CVelocityGaugeUIGameObject::Animate(float fTimeElapsed)
{
	if (m_pUIGameObject)
		m_pUIGameObject->Animate(fTimeElapsed);
	if (m_pBarGaugeUI)
		m_pBarGaugeUI->Animate(fTimeElapsed);
}

void CVelocityGaugeUIGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	if (m_pUIGameObject)
		m_pUIGameObject->Render(pd3dCommandList, pCamera, pxmf4x4World);
	if (m_pBarGaugeUI)
		m_pBarGaugeUI->Render(pd3dCommandList, pCamera, pxmf4x4World);
}


void CVelocityGaugeUIGameObject::SetBarValue(float fTotalBarValue, float fCurrentBarValue)
{
	if (m_pBarGaugeUI)
		m_pBarGaugeUI->UpdateBarValue(fTotalBarValue, fCurrentBarValue);
}
