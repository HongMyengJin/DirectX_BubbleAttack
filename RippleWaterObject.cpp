#include "RippleWaterObject.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "TerrainMeshComponent.h"

void CRippleWaterObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
	m_xmf4x4WaterAnimation = Matrix4x4::Identity();;
	m_pComponents.resize(4);
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	m_xmf3Scale = xmf3Scale;

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CTerrainMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->ResizeMesh(cxBlocks * czBlocks);

	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			std::shared_ptr<CGridMeshComponent> pHeightMapGridMesh = std::make_shared<CGridMeshComponent>();
			pHeightMapGridMesh->Init(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, NULL);
			dynamic_cast<CTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
		}
	}

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 3, m_vTextureType);


	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, L"Image/Water_Base_Texture_0.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 4, 0, 1, L"Image/Water_Detail_Texture_0.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 5, 0, 2, L"Image/Lava(Diffuse).dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 3); // 수정 필요

}

void CRippleWaterObject::Animate(float fTimeElapsed)
{
	m_xmf4x4WaterAnimation._31 += fTimeElapsed * 0.0125f;
	m_xmf4x4WaterAnimation._32 += fTimeElapsed * 0.00125f;
}

void CRippleWaterObject::Update(float fTimeElapsed, void* pData, void* pData2)
{
}

void CRippleWaterObject::PostRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	UpdateShaderVariables(pd3dCommandList);
}

void CRippleWaterObject::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CRippleWaterObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4WaterAnimation;
	XMStoreFloat4x4(&xmf4x4WaterAnimation, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4WaterAnimation)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(10, 16, &xmf4x4WaterAnimation, 0);
}
