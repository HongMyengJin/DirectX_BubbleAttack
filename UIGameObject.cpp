#include "UIGameObject.h"
#include "UIMesh.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
void CUIGameObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize, wchar_t* pstrFileName)
{
	m_pComponents.resize(4);

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CUIMesh>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CUIMesh*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList);

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 1, m_vTextureType);
	//pstrFileName
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, pstrFileName);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 1); // 수정 필요
}

void CUIGameObject::Animate(float fTimeElapsed)
{


	XMFLOAT4X4 xmf4x4World;

	xmf4x4World._11 = m_xmf2Size.x / (FRAME_BUFFER_WIDTH);
	xmf4x4World._22 = m_xmf2Size.y / (FRAME_BUFFER_HEIGHT);

	xmf4x4World._12 = 0.f;
	xmf4x4World._13 = 1.f;

	xmf4x4World._41 = (m_xmf2ScreenPosition.x / FRAME_BUFFER_WIDTH);
	xmf4x4World._42 = (m_xmf2ScreenPosition.y / FRAME_BUFFER_HEIGHT);

	dynamic_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World = xmf4x4World;

}

void CUINumberGameObject::UpdateNumber(UINT iNumber)
{
	if (m_iNumber != iNumber) // 자릿수를 벡터에 넣음
	{
		m_iNumber = iNumber;

		int Ten = 0;
		int Number = iNumber;
		while ((Number / 10) >= 1) { Ten++; Number /= 10; }
		m_vecNumber.resize(Ten + 1);
		for (int i = m_vecNumber.size() - 1; i >= 0; i--)
		{
			m_vecNumber[i] = iNumber / ((int)pow(10, i));
			iNumber = iNumber % ((int)pow(10, i));
		}
	}
}

void CUINumberGameObject::UpdateNumberTexture(UINT Number, UINT Order)
{
	XMFLOAT4X4 xmf4x4World;

	xmf4x4World._11 = m_xmf2Size.x / (FRAME_BUFFER_WIDTH);
	xmf4x4World._22 = m_xmf2Size.y / (FRAME_BUFFER_HEIGHT);


	xmf4x4World._12 = Number * 0.1f;
	xmf4x4World._13 = Number * 0.1f + 0.1f;

	xmf4x4World._41 = (m_xmf2ScreenPosition.x / FRAME_BUFFER_WIDTH) + ((m_xmf2Size.x) / (FRAME_BUFFER_WIDTH)*Order);
	xmf4x4World._42 = (m_xmf2ScreenPosition.y / FRAME_BUFFER_HEIGHT);

	dynamic_cast<CTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->m_xmf4x4World = xmf4x4World;
}

void CUINumberGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	for (int i = 0; i < m_vecNumber.size(); i++)
	{
		UpdateNumberTexture(m_vecNumber[m_vecNumber.size() - i - 1], i);
		m_pComponents[UINT(ComponentType::ComponentTransform)]->Render(pd3dCommandList);

		UINT nMaterial = static_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_nMaterials;
		for (int i = 0; i < nMaterial; i++)
		{
			m_pComponents[UINT(ComponentType::ComponentMaterial)]->Render(pd3dCommandList, pCamera, &i);
			m_pComponents[UINT(ComponentType::ComponentMesh)]->Render(pd3dCommandList, pCamera, &i);
		}
	}
	if (m_pSiblingObject)
		m_pSiblingObject->Render(pd3dCommandList, pCamera, pxmf4x4World);
	if (m_pChildObject)
		m_pChildObject->Render(pd3dCommandList, pCamera, pxmf4x4World);
}
