#include "VelocityGaugeUIGameObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"

void CVelocityGaugeUIGameObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap)
{
	m_pComponents.resize(4);

	//std::shared_ptr<CUIGameObject> pUIGameObject = std::make_shared<CUIGameObject>();
	//pUIGameObject->Init(pd3dDevice, pd3dCommandList, pDescriptorHeap, XMFLOAT2(10.f, 10.f), L"Image/SlideBar.dds");
	//pUIGameObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.9f, FRAME_BUFFER_HEIGHT * 0.5f));
	//pUIGameObject->SetSize(XMFLOAT2(70.f, 700.f));

	//m_pUIGameObjects.push_back(pUIGameObject);

	//pUIGameObject = std::make_shared<CUIGameObject>();
	//pUIGameObject->Init(pd3dDevice, pd3dCommandList, pDescriptorHeap, XMFLOAT2(10.f, 10.f), L"Image/SlideBar.dds");
	//pUIGameObject->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.9f, FRAME_BUFFER_HEIGHT * 0.5f));
	//pUIGameObject->SetSize(XMFLOAT2(70.f, 700.f));

	//m_pBarGaugeUI = std::make_shared<CBarUIGameObject>();
	//m_pBarGaugeUI->Init(pd3dDevice, pd3dCommandList, pDescriptorHeap, XMFLOAT2(10.f, 10.f), L"Image/ValueBar.dds", L"Image/Gradation.dds");
	//m_pBarGaugeUI->SetScreenPosition(XMFLOAT2(FRAME_BUFFER_WIDTH * 0.9f, FRAME_BUFFER_HEIGHT * 0.5f));
	//m_pBarGaugeUI->SetSize(XMFLOAT2(70.f, 700.f));
}

void CVelocityGaugeUIGameObject::Animate(float fTimeElapsed)
{
	for(int i = 0; i < m_pUIGameObjects.size(); i++)
	if (m_pUIGameObjects[i])
		m_pUIGameObjects[i]->Animate(fTimeElapsed);

}

void CVelocityGaugeUIGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	for (int i = 0; i < m_pUIGameObjects.size(); i++)
		if (m_pUIGameObjects[i])
			m_pUIGameObjects[i]->Render(pd3dCommandList, pCamera, pxmf4x4World);
}


void CVelocityGaugeUIGameObject::SetBarValue(float fTotalBarValue, float fCurrentBarValue)
{

}
