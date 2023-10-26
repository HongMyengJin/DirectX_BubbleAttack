#include "TextureRectObject.h"
#include "TextureRectMeshComponent.h"
#include "MaterialComponent.h"
#include "TextureObjectTransformComponent.h"

void CTextureRectObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CDescriptorHeap* pDescriptorHeap, XMFLOAT2 fSize)
{
	m_pComponents.resize(4);

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CTextureRectMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTextureObjectTransformComponent>();

	dynamic_cast<CTextureRectMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Init(pd3dDevice, pd3dCommandList, fSize.x, fSize.y, 20.f);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 1);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, L"Image/Explode_8x8.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 1); // 수정 필요
}

void CTextureRectObject::Animate(float fTimeElapsed)
{
	AnimateUV(fTimeElapsed);
}

void CTextureRectObject::AnimateUV(float fTimeElapsed)
{
	m_fAnimateTime += fTimeElapsed;
	if (m_fAnimateLifeTime < m_fAnimateTime)
		m_fAnimateTime = 0.f;
	dynamic_cast<CTextureObjectTransformComponent*>(m_pComponents[UINT(ComponentType::ComponentTransform)].get())->UVAnimate(m_fAnimateTime / m_fAnimateLifeTime);
}

void CTextureRectObject::SetAnimateLifeTime(float fLifeTime)
{
	m_fAnimateLifeTime = fLifeTime;
}
