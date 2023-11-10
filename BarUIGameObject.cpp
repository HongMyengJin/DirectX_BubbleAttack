#include "BarUIGameObject.h"
#include "UIGameObject.h"
#include "UIMesh.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"

void CBarUIGameObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize, wchar_t* pstrFileName, wchar_t* pstrFileName2)
{
	m_pComponents.resize(4);

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CUIMesh>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CUIMesh*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList);

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(2);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 2, m_vTextureType);
	//pstrFileName
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, pstrFileName);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 1, pstrFileName2);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 2); // 수정 필요
}

void CBarUIGameObject::Animate(float fTimeElapsed)
{
	m_xmf2Alpha.x = 0.f;
	CUIGameObject::Animate(fTimeElapsed);
}

void CBarUIGameObject::UpdateBarValue(float fTotalBarValue, float fCurrentBarValue)
{
	m_fTotalBarValue = fTotalBarValue;
	m_fCurrentBarValue = fCurrentBarValue;
	m_xmf2Alpha.y = m_fCurrentBarValue / m_fTotalBarValue;
}

