#include "TerrainObject.h"
#include "TerrainMeshComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "TexturedMeshComponent.h"
void CTerrainObject::Init(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, CDescriptorHeap* pDescriptorHeap, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
	m_pComponents.resize(4);
	m_nWidth = nWidth;
	m_nLength = nLength;

	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	m_xmf3Scale = xmf3Scale;

	m_pHeightMapImage = std::make_shared<CHeightMapImage>();
	m_pHeightMapImage->Init(pFileName, nWidth, nLength, xmf3Scale);

	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	m_pComponents[UINT(ComponentType::ComponentMesh)] = std::make_shared<CTerrainMeshComponent>();
	m_pComponents[UINT(ComponentType::ComponentMaterial)] = std::make_shared<CMaterialsComponent>();
	m_pComponents[UINT(ComponentType::ComponentTransform)] = std::make_shared<CTransformComponent>();

	dynamic_cast<CTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->ResizeMesh(cxBlocks * czBlocks);

	CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			std::shared_ptr<CHeightMapGridMesh> pHeightMapGridMesh = std::make_shared<CHeightMapGridMesh>();
			pHeightMapGridMesh->Init(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color, m_pHeightMapImage.get());
			dynamic_cast<CTerrainMeshComponent*>(m_pComponents[UINT(ComponentType::ComponentMesh)].get())->SetMesh(x + (z * cxBlocks), pHeightMapGridMesh);
		}
	}

	std::vector<ResourceTextureType> m_vTextureType;
	m_vTextureType.resize(1);
	m_vTextureType[0] = ResourceTextureType::ResourceTexture2D;

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->Init(1, 5, m_vTextureType);

	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 0, L"Image/ForestGround01_2K_BaseColor.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 1, L"Image/Detail_Texture_7.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 2, L"Image/Base_Texture.dds"); // 
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 3, L"Image/Grass01_1K_BaseColor.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->LoadTextureFromFile(pd3dDevice, pd3dCommandList, pDescriptorHeap, ResourceTextureType::ResourceTexture2D, 3, 0, 4, L"Image/HeightMap2(Flipped)Alpha.dds");
	dynamic_cast<CMaterialsComponent*>(m_pComponents[UINT(ComponentType::ComponentMaterial)].get())->CreateShaderResourceView(pd3dDevice, pDescriptorHeap, 0, 3, 5); // 수정 필요
}

void CTerrainObject::Animate(float fTimeElapsed)
{
}

void CTerrainObject::Update(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent)
{
}

float CTerrainObject::GetHeight(float x, float z, bool bReverseQuad)
{
	if (m_pHeightMapImage)
		return(m_pHeightMapImage->GetHeight(x, z, bReverseQuad) * m_xmf3Scale.y);
	else
		return 0.f;
}

