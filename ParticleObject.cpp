#include "ParticleObject.h"
#include "ParticleMeshComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
void CParticleObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, float fLifetime, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size, UINT nMaxParticles)
{
	m_pComponents.resize(4);

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CParticleMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->CreateVertexBuffer(pd3dDevice, pd3dCommandList, xmf3Position, xmf3Velocity, fLifetime, xmf3Acceleration, xmf3Color, xmf2Size);
	dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->CreateStreamOutputBuffer(pd3dDevice, pd3dCommandList, nMaxParticles);

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(2);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;
	m_vTextureType[1] = ResourceTextureType::ResourceTexture2DBUFFER;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(2, 1, m_vTextureType);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, L"Image/RoundSoftParticle.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 1, 0); // 수정 필요

	srand((unsigned)time(NULL));

	XMFLOAT4* pxmf4RandomValues = new XMFLOAT4[1024];
	for (int i = 0; i < 1024; i++) { pxmf4RandomValues[i].x = float((rand() % 10000) - 5000) / 5000.0f; pxmf4RandomValues[i].y = float((rand() % 10000) - 5000) / 5000.0f; pxmf4RandomValues[i].z = float((rand() % 10000) - 5000) / 5000.0f; pxmf4RandomValues[i].w = float((rand() % 10000) - 5000) / 5000.0f; }

	//	m_pRandowmValueTexture = new CTexture(1, RESOURCE_TEXTURE1D, 0, 1);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateBuffer(pd3dDevice, pd3dCommandList, pxmf4RandomValues, 1024, sizeof(XMFLOAT4), DXGI_FORMAT_R32G32B32A32_FLOAT, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_GENERIC_READ, 1, 0);
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 4, 1, 1); // 수정 필요

}

void CParticleObject::OnPostRender()
{
	dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->OnPostRender(0);
}

void CParticleObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, XMFLOAT4X4* pxmf4x4World)
{
	m_pComponents[UINT(ComponentType::ComponentTransform)]->Render(pd3dCommandList);

	UINT nMaterial = static_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->m_nMaterials;
	for (int i = 0; i < nMaterial; i++)
		m_pComponents[UINT(ComponentType::ComponentMaterial)]->Render(pd3dCommandList, pCamera, &i);

	for (int i = 0; i < 2; i++)
	{
		PrepareRender(pd3dCommandList);
		dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->PreRender(pd3dCommandList, i);
		dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->Render(pd3dCommandList, i);
		dynamic_cast<CParticleMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->PostRender(pd3dCommandList, i);
	}

	
}
